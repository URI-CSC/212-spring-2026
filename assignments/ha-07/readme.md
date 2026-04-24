# Homework Assignment 07 (optional)

## Overview

You now know the two workhorses of data-structure design:
**hash tables** (`unordered_map`, `unordered_set`) and
**red-black trees** (`map`, `set`, `multimap`).
This assignment asks you to choose between them deliberately,
and to justify each choice based on the query it must serve.

The application is a real computer-vision problem:
collapsing thousands of overlapping bounding-box candidates into
a compact set of object detections using **Non-Maximum Suppression (NMS)**.

**Part 1** builds the foundational data structures:
a `BBox` struct with geometric operations and a `SpatialIndex`
that stores boxes under two simultaneous access patterns, each
backed by the right container.

**Part 2** implements the full NMS pipeline in two ways: BFS with a queue, and DFS with a stack; and provides four query functions over the survivor list. Survivors are the boxes that remain after NMS, one per distinct object.

> [!CAUTION]
> **Disable ALL AI autocomplete in your editor before you start.**
> You may use an LLM to ask about syntax or to understand a concept --
> for example *what does `std::multimap::lower_bound` do?* or
> *what is IoU?* but not to write or fix code.
> Submitting generated code without understanding it defeats the purpose
> of this assignment and will be evident in your submitted work.

> [!IMPORTANT]
> Read every specification and all provided code before writing a
> single line.  The algorithms are fully described in English inside
> this file and at the tops of the source files.

## Background: Non-Maximum Suppression

Each CSV file in `images/` is the raw output of an object detector, based on neural networks, applied to one photograph. Every row is a **candidate bounding box**, a rectangle drawn around a region of the image where the detector thinks an object might be, along with a `category_id` (what type of object) and a confidence `score` in `[0, 1]`.

### Image coordinates and bounding boxes

Computer-vision code uses a coordinate system that surprises many students at first:

- The **origin `(0,0)` is the top-left corner** of the image.
- **`x` increases to the right.**
- **`y` increases downward**, the opposite of a standard math or data plot.

Each bounding box is described by four numbers:

| Field | Meaning |
|-------|---------|
| `lt_x` | x-coordinate of the **left** edge of the box |
| `lt_y` | y-coordinate of the **top** edge of the box |
| `width` | horizontal size in pixels |
| `height` | vertical size in pixels |

The **top-left corner** of the box is given by `lt_x, lt_y` (hence "lt" = left-top). The remaining corners follow from it:

| Corner | Coordinates |
|--------|-------------|
| top-left | `(lt_x, lt_y)` |
| top-right | `(lt_x + width, lt_y)` |
| bottom-left | `(lt_x, lt_y + height)` |
| bottom-right | `(lt_x + width, lt_y + height)` |

The image below illustrates the coordinate system and the box parameters. It corresponds to a single bounding box with `lt_x = 99`, `lt_y = 112`, `width = 446`, and `height = 278`. The image also shows the category name (zebra) and the confidence score (0.937) for this box, but those are not part of the coordinate system, they are just labels.

<p align="center">
  <img src="zebra.png" width="60%">
</p>

### Why thousands of boxes?

An object detector does not output one tidy box per object. It scans the image at many positions and scales simultaneously, producing **thousands of overlapping candidate boxes**. The same person in a photo might generate 80 slightly shifted rectangles all scored around 0.85, one per scan position that happened to land on that person.

For example, open `000000191845.csv`. It has **2,674 candidate boxes** for a single crowded-crosswalk photograph. Most of them describe the same handful of people and umbrellas, just slightly shifted. Your job is to keep **one box per distinct object**, the one the detector is most confident about, and discard the rest. The image below shows the raw image on the left, and the 2,674 candidates drawn as green outlines on the right.

<p align="center">
  <img src="./images/000000191845.jpg" width="45%">
  <img src="boxes.png" width="45%">
</p>

### Non-Maximum Suppression (NMS)

The key idea behind NMS is simple: two boxes that cover mostly the same pixels almost certainly describe the same object. Keep the higher-scoring one and suppress the other. The three steps below make this concrete:

#### Step 1: Measuring overlap via Intersection over Union (IoU)

Two boxes "belong to the same object" when they cover mostly the same
pixels.  The standard measure is **Intersection over Union (IoU)**, defined as the area of their intersection divided by the area of their union. IoU = 0 means no shared pixels at all.  IoU = 1 means the boxes are
identical.  We use `IOU_THRESHOLD = 0.50`: any two boxes that share more
than half their combined area are considered duplicates of the same object.

<p align="center">
  <img src="iou_diagram.svg" width="400">
</p>

```text
intersection area = width_overlap * height_overlap
       union area = area(A) + area(B) - intersection area
              IoU = intersection area / union area
```

#### Step 2: Grouping overlapping boxes

The overlap relationship is **transitive**: if A overlaps B and B overlaps C, all three describe the same object even if A and C don't overlap directly. A key ideas is to group overlapping boxes together, and then pick the single highest-scoring box from each group as the survivor. The survivor is the one the detector is most confident about, and the rest are suppressed. See the example below with five candidates sorted by score, and the two groups that emerge from their overlaps:

```
  idx   score   overlaps directly with
  [0]   0.92    [1]
  [1]   0.81    [0], [2]
  [2]   0.74    [1]
  [3]   0.67    [4]
  [4]   0.55    [3]

  Group A: {[0],[1],[2]}   survivor = [0] (score 0.92)
  Group B: {[3],[4]}       survivor = [3] (score 0.67)
```

Notice that [0] and [2] belong to the same group even though they do not overlap each other, they are connected *through* [1].

#### Step 3: The algorithm

**Phase 1: Sort by score.** Use `std::priority_queue<BBox>` to pull all candidates out in descending score order into a flat array `sorted[0 .. n-1]`. Note that after this step, `sorted[0]` is the most confident candidate.

**Phase 2: Build the overlap graph.** For every pair of boxes `(i, j)` with `i < j`, if `IoU(sorted[i], sorted[j]) > IOU_THRESHOLD`, add an undirected edge. Return an adjacency list `unordered_map<int, unordered_set<int>>`.

Consider the example above with five candidates.  The overlap graph has two connected components, one for each group:

```mermaid
graph LR
    0["[0] 0.92"] --- 1["[1] 0.81"]
    1             --- 2["[2] 0.74"]
    3["[3] 0.67"] --- 4["[4] 0.55"]
```

The corresponding adjacency list stored by `build_overlap_graph`:

```
adj[0] = {1}
adj[1] = {0, 2}
adj[2] = {1}
adj[3] = {4}
adj[4] = {3}
```

Every index `0..n-1` has an entry even if its neighbor set is empty (an
isolated node represents a box that overlaps nothing).

Using `unordered_set<int>` for each neighbor list is a deliberate choice over `vector<int>`:
- Membership test `adj[i].count(j)` is O(1) average -- contrast with `vector<int>` where you would need an O(degree) linear scan.
- Duplicate edges are impossible (set ignores re-inserts).
- Iteration with range-for works identically to a vector.

**Phase 3: Find connected components.**
- *Input:* `sorted[0..n-1]` and `adj` (the overlap graph built in Phase 2).
- *Output:* `survivors` (one box per connected component, namely the highest-scoring box in that component).

Walk `sorted[]` left to right with index `i`.  Because the array is sorted
descending, the first time you encounter an **ungrouped** index `i`, you know
`sorted[i]` is the highest-scoring box that has not yet been assigned to any
component, it wins its component by definition.  Add `sorted[i]` to
`survivors` **exactly once**, right here.

Then use a **queue** or **stack** as a frontier to flood-fill every other box
that belongs to the same component.  Those boxes are **suppressed**: they get
marked as grouped (so they are skipped when the outer loop reaches them) but
are never added to `survivors`.

```
  for i = 0 to n-1:
      if i is already grouped: continue   // skip suppressed boxes
      Mark i as grouped.  Add sorted[i] to survivors.   // i is the winner
      Place i in the frontier.
      While the frontier is not empty:
          Remove one index j from the frontier.
          For each neighbor k in adj[j]:
              if k is not yet grouped:
                  mark k as grouped         // k is suppressed, not a winner
                  add k to the frontier     // explore k's neighbors next
```

When the frontier empties, the entire component of `sorted[i]` has been
marked.  The outer loop then continues from index `i+1`, looking for the
next ungrouped box to start a new component.

#### Queue vs Stack: same result, different order

The choice of frontier data structure determines the *order* in which boxes
inside a component are visited, but not which boxes are found.

- **Queue -> Breadth-First Search (BFS).** A queue gives every node added at the same time an equal turn before
moving deeper.  Concretely: when you enqueue several neighbors of the
current node, the neighbor enqueued *first* is dequeued and processed
*first*, before any of its own neighbors.  The component is therefore
explored **level by level** -- all boxes one hop from the winner, then
all boxes two hops away, then three hops, and so on.  Imagine dropping a
stone in a pond: the ripple expands outward in concentric rings.
This is why queue-based traversal is called **breadth-first**, it
exhausts the current "breadth" of the frontier before going deeper.

- **Stack -> Depth-First Search (DFS).** A stack always processes the *most recently added* item next.  When you
push several neighbors of the current node, the last one pushed is popped
and explored immediately, along with *its* neighbors, and *their*
neighbors. The traversal dives down one overlap chain as deep as it can
go before backtracking to try another branch.  This is **depth-first**:
the algorithm exhausts one path before widening.

Because both exhaustively process
every box reachable from the winner.  The *order* in which boxes inside a
group are visited differs, but the final survivor set is identical.

#### Complexity

The overall complexity of the NMS pipeline is dominated by the pairwise IoU checks in Phase 2, which can be $$O(n^2)$$ in the worst case. Queue and stack have the **same asymptotic complexity**. $$n$$ is the number of candidates. The overlap graph can be dense in the worst case (every box overlaps every other), so $$E$$ can be $$O(n^2)$$. The table below breaks down the cost of each phase:

| Phase | Cost |
|-------|------|
| Sort with priority queue | $$O(n \log n)$$ |
| Build overlap graph | $$O(n^2)$$ |
| Group expansion (BFS or DFS via adjacency list) | $$O(V + E) \leq O(n^2)$$ |


#### Why run NMS per category?

`nms_queue` and `nms_stack` take candidates that describe the **same type
of object**.  Two boxes from *different* categories can overlap heavily
(a person holding a tennis racket) but must both survive.  Always call
the NMS functions separately per `category_id`.

## Part 1: BBox Primitives and SpatialIndex

### Section 1: BBox struct  (10 pts)

Complete `part1/bbox.cpp`. The struct declaration is in `part1/bbox.h` (provided -- do not modify).

| Method | Behaviour |
|--------|-----------|
| `area` | Returns the area of the box. |
| `iou` | Returns IoU with `other`.  Returns `0.0` when the boxes do not overlap. |
| `operator<` | Returns `true` when `this->score < other.score`.  Used by `std::priority_queue` to build a max-heap. |

After implementing these methods, run the tests:

```bash
g++ -std=c++17 -Wall -Werror bbox.cpp test_bbox.cpp -o test_bbox
./test_bbox
```

### Section 2: SpatialIndex  (30 pts)

Complete `part1/index.cpp`. `SpatialIndex` stores bounding boxes in **two containers at once**, each chosen deliberately for a different query pattern:

```
by_category:  unordered_map<int, vector<BBox>>
              Key = category_id.
              Constant-time insert and lookup by category_id.
              "Give me all boxes for category 3."

by_score:     multimap<float, BBox, greater<float>>
              begin() is always the highest-scoring box.
              Logarithmic-time insertion by score, and linear-time iteration from highest to lowest.
              Range query: O(log n + k) via lower_bound/upper_bound.
              "Give me the 5 most confident boxes."
              "Give me every box with score in [0.4, 0.8]."
```

Notice that the two containers are **redundant**: they store the same boxes, just organized differently.  This is a common design pattern in real systems: you maintain multiple indexes to support different query patterns efficiently.  The tradeoff is that every update (insertion) must be applied to both containers, and you must ensure they stay consistent.  In this case, `insert(b)` must update **both** containers.

**Why `multimap` and not `map`?**
Two boxes can have identical scores.  `map` would silently drop one;
`multimap` keeps both.

**Why `greater<float>` as comparator?**
Without it, `begin()` points to the *lowest* score.  Flipping the order
makes iteration from highest to lowest natural.

**Why an ordered tree for `score_range` but not for `get_category`?**
An ordered tree supports O(log n) range boundaries via `lower_bound` /
`upper_bound`.  A hash map has no ordering, a range query would require
a full O(n) scan.

| Method | Behaviour |
|--------|-----------|
| `insert` | Inserts `b` into **both** `by_category` and `by_score`. Every update must be applied to both containers. |
| `get_category` | Returns the vector of all boxes stored under `cat_id`. Precondition: `has_category(cat_id) == true`. |
| `has_category` | Returns `true` iff at least one box with `cat_id` has been inserted. |
| `category_ids` | Returns a vector of every distinct `category_id` inserted. Order is unspecified. |
| `top_k` | Returns up to `k` boxes with the highest scores in descending order. If `total() <= k`, returns all boxes. |
| `score_range` | **Provided as a worked example: read it before you write anything else.** Returns every box with score in `[lo, hi]` (inclusive) in descending order, using `lower_bound`/`upper_bound` on the descending `by_score` tree. |
| `total` | Returns the total number of boxes inserted across all categories. |
| `num_categories` | Returns the number of distinct `category_id` values currently in the index. |

After implementing the methods, run the tests:

```bash
g++ -std=c++17 -Wall -Werror bbox.cpp index.cpp test_index.cpp -o test_index
./test_index
```

## Part 2: NMS Pipeline

Part 2 is self-contained.  `part2/` provides complete implementations of
`BBox`, the CSV parser, SVG helpers, and `main.cpp`.
You write `nms.cpp` and `query.cpp`.

### Section 1: Overlap graph + NMS traversal  (35 pts required + 15 pts bonus)

Complete `part2/nms.cpp`. Read the full algorithm description at the top of `nms.cpp` before
writing a single line.

| Method | Behaviour |
|--------|-----------|
| `build_overlap_graph` | Given candidates already sorted by score descending, returns an adjacency list where `adj[i]` is the set of indices `j` with `IoU(sorted[i], sorted[j]) > IOU_THRESHOLD`. The graph is undirected: if `j` is in `adj[i]` then `i` is in `adj[j]`. Every index `0 .. n-1` has an entry even if its neighbor set is empty. |
| `nms_queue` | Runs the full three-phase NMS pipeline (sort, build graph, expand components) using `std::queue<int>` as the traversal frontier, giving BFS order. Returns one survivor per connected component, the highest-scoring box in that component. |
| `nms_stack` | **Bonus (15 pts).** Same logic as `nms_queue`; replace `std::queue<int>` with `std::stack<int>` for DFS order. Must return the same survivor set as `nms_queue`. Attempt only after `nms_queue` passes all tests. |

After implementing the methods, run the tests:

```bash
g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp test_nms.cpp -o test_nms
./test_nms
```

### Section 2: Query functions  (13 pts)

Complete `part2/query.cpp`. Four functions that answer different questions about the survivor list.
The container choice is the exercise. `category_names.h` provides `category_name(int id)` for label lookup.

| Function | Returns | Container |
|----------|---------|-----------|
| `best_per_category` | Best survivor per `category_id` | `unordered_map<int, BBox>` (hash table) |
| `top_k_global` | Top-k survivors by score, descending | `multimap<float, BBox, greater<float>>` (RB-tree) |
| `find_by_name` | Survivors whose category name contains `substr` | `map<string, int>` (RB-tree ordered by name) |
| `score_range` | Survivors with score in [lo, hi], descending | `multimap<float, BBox, greater<float>>` + `lower_bound`/`upper_bound` |

### Section 3: Full Pipeline  (35 pts total, via `test_pipeline`)

`main.cpp` is **fully provided** -- you do not write it.
Read it carefully: it shows you how the pieces fit together.
Run it and look at the SVG output before submitting.

To compile and run the full pipeline on one of the provided CSV files:

```bash
g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp csv.cpp query.cpp main.cpp -o nms
./nms ../images/000000074646.csv out.svg
open out.svg

# Time queue vs stack on the large CSV:
./nms --compare ../images/000000191845.csv
```

To run the full pipeline with the autograder tests:

```bash
g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp csv.cpp query.cpp \
    test_pipeline.cpp -o test_pipeline
./test_pipeline
```

## Submission and Grading

The 10-point buffer (110 required, capped at 100) lets you miss a few required
tests and still reach full code score.  Attempt `nms_stack` only after
everything else passes. Each `.cpp` you submit compiles cleanly with `g++ -std=c++17 -Wall -Werror`.

Submit exactly these files on Gradescope:

- `bbox.cpp`  
- `index.cpp` 
- `nms.cpp`
- `query.cpp`
- `llm-usage.txt`

Do **not** submit `main.cpp` (it is provided and unchanged).

Do **not** modify the provided files (`bbox.h`, `index.h`, `nms.h`,
`query.h`, `svg.h`, `csv.h`, `csv.cpp`, `bbox.cpp` in part2,
or any `test_*.cpp`).

`llm-usage.txt` must list every LLM prompt you entered and the responses
you received.  Write "No LLM used" if you did not use one.

> [!CAUTION]
> Academic integrity is paramount.  Submitting generated or copied code
> will result in forfeiture of credit and may be referred for disciplinary action.
