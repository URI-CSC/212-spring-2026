# Lab 04 - Heaps and Priority Queues

## Overview

In this lab, you will design and implement a **max-priority queue** backed by a binary max-heap, then use that same heap insight to implement **buildHeap** (an algorithm that constructs a heap in linear time).

You will work with **strings** throughout. The max-priority queue returns the alphabetically greatest string. The same underlying heap mechanics drive all three tasks.

All tasks are expected to be completed during the lab session. However, if you run out of time, please make every effort to complete them outside of lab hours.

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. In particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE while working on this lab. You may ask specific questions to LLMs about concepts or language syntax, but we strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> For all tasks below, you must read the specifications and all the code provided carefully. This lab is designed to make you think critically about the design and implementation of heap-based data structures, in addition to learning best practices in C++.

## Task 1: MaxPQueue

In this task you will implement a **max-priority queue of strings**. A max priority queue is an abstract data type that supports three core operations: insert an element (`push`), inspect the maximum element (`top`), and remove the maximum element (`pop`). The underlying data structure is a **binary max-heap** stored in a fixed-capacity array.

### The Max-Heap

A **binary max-heap** is a complete binary tree that satisfies the **heap invariant**: every node is greater than or equal to both of its children. As a consequence, the largest element is always at the root.

We store the heap in a **0-indexed array** `data[0 .. capacity-1]`. The root lives at index 0. The mapping from tree positions to array indices is:

| Relationship | Formula |
|---|---|
| Left child of node `i` | `2 * i + 1` |
| Right child of node `i` | `2 * i + 2` |
| Parent of node `i` | `(i - 1) / 2` |

`upHeap(pos)` is used after an insertion: a new element is placed at the next available position (`data[sz]`) and then "bubbled up" by repeatedly swapping with its parent until the heap invariant is restored.

`downHeap(pos)` is used after a removal: the last element is moved to position 0 (the root) and then "sunk down" by swapping with the larger of its two children until the invariant is restored.

Both `upHeap` and `downHeap` are **provided for you** in `maxpqueue.cpp`.

### Files

You are given `maxpqueue.h` (complete class declaration, **do not modify**) and `maxpqueue.cpp` (partially implemented; the constructor, destructor, `top`, `size`, and `empty` are already provided; implement the four methods still marked `TODO`).

### Interface

| Method | Role | Status |
|--------|------|--------|
| `upHeap(int pos)` | Restores heap order by bubbling `data[pos]` up toward the root | Provided |
| `downHeap(int pos)` | Restores heap order by sinking `data[pos]` down toward the leaves | Provided |
| `MaxPQueue(int capacity)` | Creates an empty queue with the given maximum capacity | Provided |
| `~MaxPQueue()` | Releases all resources owned by the queue | Provided |
| `MaxPQueue(const MaxPQueue&)` | Creates an independent deep copy of another queue | **TODO** |
| `operator=(const MaxPQueue&)` | Replaces contents with an independent deep copy | **TODO** |
| `push(const std::string&)` | Inserts a string while preserving the max-heap property | **TODO** |
| `pop()` | Removes the maximum element and restores the heap property | **TODO** |
| `top() const` | Returns a read-only reference to the maximum element | Provided |
| `size() const` | Returns the number of elements in the queue | Provided |
| `empty() const` | Returns `true` if the queue holds no elements | Provided |

### Testing

`testmaxpqueue.cpp` is provided and covers Tasks 1 and 2. After completing Task 1, compile and run. Task 2 tests will print `[SKIP]` until you reach that task.

```bash
$ g++ -std=c++11 -Wall -Werror maxpqueue.cpp testmaxpqueue.cpp -o testmaxpqueue
$ ./testmaxpqueue
```

> [!NOTE]
> The heap uses a **0-indexed array**: the root is at `data[0]` and the active elements occupy `data[0 .. sz-1]`. You allocate exactly `capacity` strings.

> [!WARNING]
> Because `MaxPQueue` owns a dynamically allocated array, failing to implement the **Rule of Three** properly leads to subtle bugs. Without a custom copy constructor, `MaxPQueue b = a;` would make both `a` and `b` point to the *same* underlying array. Modifying `b` would corrupt `a`, and when both destructors run they would attempt to `delete[]` the same memory causing undefined behavior. Always implement destructor, copy constructor, and copy assignment together.

## Task 2: buildHeap

You have a `MaxPQueue` that supports `push` in $\Theta(\log n)$ time. If you want to initialize a queue from an existing collection of $n$ strings by pushing them one at a time, you pay $\Theta(n \log n)$ total. Can we do better?

Yes, **buildHeap** constructs a valid max-heap from an unsorted array in $\Theta(n)$ time.

### The Naive Approach

Calling `push` $n$ times works, but each call may trigger `upHeap`, which climbs $\Theta(\log n)$ levels in the worst case. The total time is $\Theta(n \log n)$.

### buildHeap

Instead of inserting one element at a time, buildHeap works in two steps:

1. **Copy** all $n$ elements directly into positions `data[0 .. n-1]` of the array (no ordering yet).
2. **Call `downHeap`** on every non-leaf node, starting from the **last non-leaf node** (index $n/2 - 1$) and working backward toward the root (index 0).

After step 2, every subtree satisfies the max-heap property so the entire array is a valid max-heap.

Why is this $\Theta(n)$? About half the nodes are leaves and require zero work. Roughly $n/4$ nodes are at height 1 and need at most 1 swap, $n/8$ nodes at height 2 need at most 2 swaps, and so on. The total work is bounded by the sum $\sum_{h=0}^{\lfloor \log n \rfloor} \frac{n}{2^{h+1}} \cdot h$, which converges to $\Theta(n)$.

### A Worked Example

Consider building a heap from `["cat", "ant", "fox", "bee", "dog", "eel", "gnu"]`.

**Copy** into positions 0-6:

```
Index:   0       1       2       3       4       5       6
Value: "cat"   "ant"   "fox"   "bee"   "dog"   "eel"   "gnu"

Tree view:
              cat(0)
            /        \
        ant(1)      fox(2)
        /   \       /   \
    bee(3) dog(4) eel(5) gnu(6)
```

Last non-leaf node: $n/2 - 1 = 7/2 - 1 = 2$ (index 2, "fox").

**downHeap(2):** left = ("eel"), right = ("gnu"); "gnu" > "fox". Swap indices 2 and 6.
```
0:"cat"  1:"ant"  2:"gnu"  3:"bee"  4:"dog"  5:"eel"  6:"fox"
```

**downHeap(1):** left = ("bee"), right = ("dog"); "dog" > "ant". Swap indices 1 and 4.
```
0:"cat"  1:"dog"  2:"gnu"  3:"bee"  4:"ant"  5:"eel"  6:"fox"
```

**downHeap(0):** left = ("dog"), right = ("gnu"); "gnu" > "cat". Swap indices 0 and 2. Continue sinking "cat" from index 2: left = ("eel"), right = ("fox"); "fox" > "cat". Swap indices 2 and 6.
```
0:"gnu"  1:"dog"  2:"fox"  3:"bee"  4:"ant"  5:"eel"  6:"cat"

Final tree:
              gnu(0) 
            /        \
        dog(1)      fox(2)
        /   \       /   \
    bee(3) ant(4) eel(5) cat(6)
```

`top()` returns `"gnu"`, the alphabetically greatest element.

### Files

The buildHeap constructor is already declared in `maxpqueue.h` and stubbed in `maxpqueue.cpp`. **Replace the stub** with your implementation.

### Interface

| Method | Role | Status |
|--------|------|--------|
| `MaxPQueue(const std::vector<std::string>&)` | Builds a valid max-heap from an existing collection in $\Theta(n)$ time | **TODO** |

### Testing

Recompile and run `testmaxpqueue`. The Task 2 section will now execute instead of printing `[SKIP]`. Verify that the final output shows `13/13 points`.

```bash
$ g++ -std=c++11 -Wall -Werror maxpqueue.cpp testmaxpqueue.cpp -o testmaxpqueue
$ ./testmaxpqueue
```

> [!NOTE]
> `downHeap` is already part of your `MaxPQueue` class. You do not need to write a new version. The buildHeap constructor calls the *same* private method used by `pop`. This is the elegance of buildHeap: one well-designed helper does all the work.

## Task 3: The Job Queue

You now have a working `MaxPQueue`. In this task you will use it to solve a concrete scheduling problem, and in doing so, exercise the **copy constructor** and **assignment operator** you implemented in Task 1.

### Setup

A tech recruiting firm stores applicant names as plain strings. Their scheduling system feeds names into a `MaxPQueue` and interviews applicants in the order they are popped out. No numeric score, no external weight, the name itself is the priority.

Two departments **Engineering** and **Product** both want to interview the same eight applicants independently. The system builds one master queue and then hands a copy to each department. Each department pops from its own queue without affecting the other or the master.

The eight applicants are:

```
Alice, Zoe, Marcus, Beth, Yara, Carlos, Noah, Priya
```

### What to Implement

Create `jobqueue.cpp` using `maxpqueue.h` and `maxpqueue.cpp`. Your program must:

1. Push all eight names into a `master` queue of capacity 10. Print its size.
2. **Copy constructor:** create `engineering` as a copy of `master`. Pop and print Engineering's schedule. Verify `master` is still intact by printing its size.
3. **Assignment operator:** declare `product` with capacity 3, then assign `master` to it (`product = master`). Pop and print Product's schedule. Verify `master` is still intact.

> [!NOTE]
> Deliberately initializing `product` with capacity 3 (too small for 8 elements) before the assignment tests that your `operator=` correctly deallocates the old array and allocates a fresh one of the right size, not just overwriting.

### Expected Output

```
Master queue loaded: 8 applicants

Engineering schedule (copy constructor):
  -> Zoe
  -> Yara
  -> Priya
  -> Noah
  -> Marcus
  -> Carlos
  -> Beth
  -> Alice
Master still intact: 8 applicants

Product schedule (assignment operator):
  -> Zoe
  -> Yara
  -> Priya
  -> Noah
  -> Marcus
  -> Carlos
  -> Beth
  -> Alice
Master still intact: 8 applicants
```

### Compile and Run

```bash
$ g++ -std=c++11 -Wall -Werror maxpqueue.cpp jobqueue.cpp -o jobqueue
$ ./jobqueue
```

> [!NOTE]
> String comparison in C++ is **lexicographic**: `"Zoe" > "Yara" > "Priya"` and so on, exactly like dictionary order read from back to front. The `MaxPQueue` does not know anything about "priority" in a human sense, it simply enforces that the string comparing greatest sits at `data[0]`.

## Submission

After you have completed all the tasks, submit the files below to Gradescope. Your code should be well-formatted and easy to read. Make sure to test your code thoroughly before submitting it. Full credit for this lab requires **attendance**, **collaboration with your group**, and **active participation**. No remote submissions will be accepted. If you have any questions, please ask the instructor or the TA for help.

- `maxpqueue.h` *(provided, do not modify)*
- `maxpqueue.cpp`
- `testmaxpqueue.cpp` *(provided, do not modify)*
- `jobqueue.cpp`
