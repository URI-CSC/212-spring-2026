# Lab 08 - Binary Search Trees

## Overview

In this lab you will build a **Binary Search Tree (BST)** from scratch, visualize why even randomly built trees are remarkably efficient, and then apply the same BST concept to a real problem from modern AI: converting raw text into the integer token IDs that a large language model actually processes.

The lab has three parts:

- **Part 1**: Implement `insert`, `remove`, and `search` on a BST; validate with a provided test suite.
- **Part 2**: Run a provided performance visualizer and discover that random BSTs track $$O(log_2 N)$$ closely.
- **Part 3**: Complete a tiny LLM tokenizer that performs vocabulary lookup via a string-keyed BST.

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. You may ask conceptual questions ("what are the three cases for BST remove?"), but we strongly discourage using AI tools to write the implementation for you. Understanding these algorithms by hand is the whole point.

> [!IMPORTANT]
> Read all provided code and comments carefully before writing a single line. The TODO comments in `bst.cpp` are your specification, treat them like a contract.

## Background

A **Binary Search Tree** is a linked structure in which every node holds a key and two child pointers. The BST invariant guarantees that for any node N:

```
all keys in N->left subtree  <  N->key  <  all keys in N->right subtree
```

This invariant is what makes search fast: at each node you eliminate half the remaining tree, just like binary search on a sorted array, except the tree can **grow and shrink dynamically without shifting elements in an array**.

```
              50         <- root
             /  \
           30    70
          /  \     \
        20   40    90
```

In-order traversal (left => node => right) visits keys in sorted order: `20 30 40 50 70 90`.

## Files

| File | Role | Status |
|------|------|--------|
| `bst.h` | Complete class declaration | Do not modify |
| `bst.cpp` | Provided helpers + three stubs | **Your work** |
| `testbst.cpp` | Automated test suite (17 points) | Do not modify |
| `performance.cpp` | BST performance visualizer | Provided, run and reflect |
| `tokenizer.cpp` | Tiny LLM tokenizer application | **One TODO** |

## Part 1: BST Implementation

### Interface

| Method | Description | Status |
|--------|-------------|--------|
| `BST()` | Constructs an empty tree | Provided |
| `~BST()` | Calls `clear()` recursively to free all nodes | Provided |
| `insert(key)` | Inserts key; no-op if key already exists | **TODO** (private helper) |
| `remove(key)` | Removes key; no-op if key not found | **TODO** (private helper) |
| `search(key)` | Searches for key; prints a memory diagnostic log | **TODO** |
| `inorder()` | Prints all keys in ascending order | Provided |
| `height()` | Returns height of the tree (-1 if empty) | Provided |
| `empty()` | Returns `true` if the tree has no nodes | Provided |
| `searchDepth(key)` | Returns depth of key (root = 1); used by `performance.cpp` | Provided |

Open `bst.cpp`. Three method stubs are marked `TODO`. Read the block comment above each stub before writing any code.

### What to implement

#### `insert` (private recursive helper)

The recursive structure mirrors the BST invariant directly:

- **Base case**: if `node` is `nullptr`, you have found the right spot; allocate `new Node(key)` and return it.
- **key < node->key**: recurse into the left subtree and store the result back into `node->left`.
- **key > node->key**: recurse into the right subtree and store the result back into `node->right`.
- **key == node->key**: duplicate; return `node` unchanged.

Always return `node` at the end so the parent's pointer stays **connected**. The public one-liner `void BST::insert(int key) { root_ = insert(root_, key); }` is already provided. Your job is the private helper.

#### `remove` (private recursive helper)

This is the hardest method. Navigate to the target node first (same left/right logic as insert), then handle one of three cases:

**Case 1: Leaf (no children):**
```text
   delete node
   return nullptr
```

**Case 2: One child:**
```text
   save the non-null child pointer
   delete node
   return the saved child
```

**Case 3: Two children (the tricky one):**

You cannot simply delete the node because both its children would be orphaned. Instead, find the **in-order successor**, i.e., the smallest key in the *right* subtree (use the provided `minNode()` helper). Copy that successor's key into the current node, then recursively remove the successor from the right subtree. The successor is guaranteed to have at most one child (it has no left child, by definition of minimum), so this recursive call will hit Case 1 or 2.

```c++
   node->key = minNode(node->right)->key;
   node->right = remove(node->right, node->key);   // remove the successor
   return node;
```

> [!WARNING]
> In Case 3, do **not** `delete node`. You are reusing the node's memory and only removing the successor. Deleting `node` here is a common bug that corrupts the tree.

#### `search` (public, iterative)

Walk the tree from `root_` using the BST ordering property. When the search terminates, either by finding the key or falling off the tree (`node == nullptr`), print this diagnostic log:

```text
[SEARCH] key = 40  ->  FOUND
  node  addr : 0x6000024b80d0
  left  addr : null
  right addr : null
  node  size : 24 bytes
```

```
[SEARCH] key = 99  ->  NOT FOUND
  node  addr : null
  left  addr : null
  right addr : null
  node  size : 24 bytes
```

Two hints for printing addresses:

1. Cast any `Node*` to `void*` before streaming it: `std::cout << static_cast<void*>(node)` to get the hex address.
2. `sizeof(Node)` gives the byte count of a node without needing a live instance; it is always safe to print even when `node == nullptr`.

The `node  size` field reveals an interesting memory layout fact: a `Node` holds one `int` (4 bytes) and two pointers (8 bytes each on a 64-bit system), for a natural total of 20 bytes. Your compiler will likely report 24 due to alignment padding. Discuss with your group: *why does the compiler add padding?*

### Testing

Once you have implemented all three stubs, compile and run the test suite:

```bash
$ g++ -std=c++11 -Wall -Werror bst.cpp testbst.cpp -o testbst
$ ./testbst
```

A fully correct implementation prints `17/17 points`. The test runner crashes with an assertion error at the first failing check, so if it aborts partway through, you can identify which case still has a bug.

> [!NOTE]
> The test harness suppresses the `search()` diagnostic log during automated checks so it does not clutter the score output. A visual demo at the top of the test run prints two sample searches un-suppressed so you can inspect your formatting by eye.

## Part 2: Performance Visualization

The worst-case BST is a fully sorted insertion. You get a linked list with $$O(N)$$ height. But what about a **random** BST? Is it any good?

Theory says the expected average search depth in a randomly built BST is approximately $$1.39 \cdot \log_2(N)$$. In other words, a random BST is only 39% deeper than a theoretically perfect balanced tree, and it is still firmly $$O(\log_2 N)$$.

`performance.cpp` is fully provided. Compile and run it:

```bash
$ g++ -std=c++11 -O2 bst.cpp performance.cpp -o performance
$ ./performance
```

The program produces three outputs:

**Part A (Scaling table)**: Average search depth vs. $$\log_2(N)$$ for trees of size 16 through 4096. Watch the ratio column.

**Part B (Depth histogram)**: A bar chart of search depths for N = 1024. Example shape:

```
  depth  1 | (1)
  depth  2 | ## (3)
  depth  3 | ##### (9)
  ...
  depth 13 | ######################################## (most common)
  ...
  depth 24 | # (1)
```

**Part C (Sorted vs. random)**: Confirms that sorted insertion produces a degenerate tree with height $$N - 1$$.

### Discussion questions

Discuss the following with your group before moving on.

1. In Part A, does the ratio column stabilize? At what value does it converge?
2. In Part B, what is the most common search depth? How does it compare to $$\log_2(1024)=10$$?
3. In Part C, the sorted-insertion height equals $$N - 1$$. What is the search depth for the maximum key in such a tree?
4. If you wanted to guarantee O(log N) regardless of insertion order, what kind of tree would you need? (Think about what "self-balancing" means.)

## Part 3: LLM Tokenizer

### Background

Before any text reaches a large language model, it is converted to a sequence of integer **token IDs**. GPT-4 uses a vocabulary of roughly 100000 tokens; Llama-3 uses 128000. When you type a prompt, the model never sees the letters, only numbers.

The lookup process is straightforward: given a word, find its ID in the vocabulary. With 100000 entries, a BST lookup takes about $$\log_2(100000)\approx 17$$ comparisons. A linear scan would need up to 100000. The difference compounds billions of times during training.

`tokenizer.cpp` implements a miniature version of this pipeline. A `TokenBST` class (fully provided, string keys, integer values) holds our 68-word vocabulary. Your job is to implement the `tokenize()` function that splits a sentence and looks up each word.

### What to implement

Open `tokenizer.cpp`. Find the one stub marked `TODO` inside `tokenize()`. Read the comment block above it carefully.

Your implementation should:

1. Wrap `text` in a `std::istringstream` and extract whitespace-separated words with `>>`.
2. Call `normalize()` on each word to lowercase it and strip punctuation.
3. Skip words that become empty after normalization (e.g. a bare comma).
4. Call `vocab.lookup()` to get the token ID (returns `-1` if the word is not in the vocabulary).
5. Push `{normalized_word, id}` onto `tokens` and return it.

### Testing

```bash
$ g++ -std=c++11 tokenizer.cpp -o tokenizer
$ ./tokenizer
```

> [!NOTE]
> `tokenizer.cpp` does not include `bst.h` and does not need `bst.cpp` on the command line. `TokenBST` is defined entirely within `tokenizer.cpp`. This is intentional: it shows the same BST concept working for a different key type in a self-contained setting.

Expected output (once `tokenize()` is correctly implemented):

```
Input  : "the large language model can predict the next word"
Tokens : the(53) large(35) language(34) model(39) can(15) predict(50) the(53) next(44) word(66)
Coverage: 9/9 tokens recognized  (100%)

Input  : "we train neural networks on text data"
Tokens : we(63) train(59) neural(42) [UNK] on(47) text(52) data(16)
Coverage: 6/7 tokens recognized  (86%)
```

The `[UNK]` for "networks" is intentional. It is not in our tiny vocabulary. Discuss with your group: *what would a real tokenizer do with an unknown word?* (Hint: look up subword tokenization and Byte-Pair Encoding.)

At scale, a real LLM's vocabulary lookup is one of the first operations executed for every single prompt, billions of times per day.

## Submission

After completing all three tasks, submit the files below to Gradescope. Your code should be well-formatted and easy to read. Test thoroughly before submitting. Full credit requires **attendance**, **collaboration with your group**, and **active participation**.

- `bst.cpp`
- `tokenizer.cpp`

> [!IMPORTANT]
> Do **not** submit `performance.cpp` or any of the provided files.