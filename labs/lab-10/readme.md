# Lab 10 - HashSet with Separate Chaining

## Overview

This lab builds a **templated hash set** from scratch, then puts it to work in a real application. By the end you will understand why hash tables offer $$O(1)$$ average-case membership testing, and you will have written every layer yourself: hash-function dispatch, bucket management, collision resolution via chaining, and load-factor-driven rehashing.

The lab has two parts:

- **Part 1**: Implement `HashSet<T>` in `hashset.hpp`, then complete the unit tester in `tester.cpp`
- **Part 2**: Build a genome k-mer counter in `app.cpp` using your `HashSet<T>`

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. In
> particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE.
> You may ask specific questions to LLMs about concepts or language syntax, but we
> strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> **Read each concept section fully before writing any code.** The design decisions here are
> not arbitrary - understanding *why* each piece exists is the point of the exercise.
> Discuss every method contract with your group before writing a single line.

## Part 1: HashSet with Separate Chaining

### What is a hash table?

A **hash table** maps each value to a **bucket** using a **hash function**: a deterministic function that converts a value into an integer index in $$[0, \text{bucketcount})$$. Ideally every value lands in a different bucket, giving $$O(1)$$ average insert, lookup, and remove. In practice, two distinct values may produce the same bucket index, this is called a **collision**.

### Set vs. Map

A **set** stores *values only*. It answers one question: **is this element present?** A **map** stores key-value pairs and answers: **given this key, what value is associated with it?**

Your `HashSet<T>` is a **set**. It stores elements of type `T` with no associated payload. `std::unordered_set<T>` in the STL is the library equivalent of what you are building.

> [!NOTE]
> Sets are the right tool whenever you need fast membership testing and don't need to look
> anything up by key. Classic uses: deduplication, visited-node tracking, dictionary lookup
> for a spell checker.

### Separate chaining

**Separate chaining** resolves collisions by keeping a linked list at each bucket. When two values hash to the same bucket they simply join the same chain. The bucket array is never reorganized.

```text
bucket 0 -> []
bucket 1 -> ["cat"] -> ["hat"] -> []
bucket 2 -> ["dog"] -> []
bucket 3 -> []
```

Searching for a value hashes it to a bucket, then scans only that chain. As long as chains stay *short*, average cost stays $$O(1)$$.

### Load factor and rehashing

The **load factor** is `size / bucket_count`. It measures the average chain length. When it grows too large, chains lengthen and performance approaches $$O(n)$$. **Rehashing** allocates a fresh bucket array with a different number of buckets and re-inserts every element under the new mapping. This lab's class does **not** rehash automatically, you call `rehash()` explicitly.

### Containers you'll use

| Member | Type | Role |
|--------|------|------|
| `buckets` | `std::vector<std::list<T>>` | Bucket array; `buckets[i]` is the chain for index `i` |
| `n_elem` | `size_t` | Number of elements currently in the set |

`std::hash<T>` is the standard-library hash functor. `std::hash<std::string>{}("hello")` returns a `size_t`. Taking it modulo `buckets.size()` maps any value to a valid bucket index.

> [!NOTE]
> `std::list` is a doubly-linked list. You only need `push_front`, iteration, and `erase`.
> `erase` takes an iterator and runs in $$O(1)$$, far better than searching and then
> shifting elements as you would in a `std::vector`.

> [!NOTE]
> Because `HashSet` is a **template class**, the entire implementation, including all
> method bodies, must live in `hashset.hpp`. The compiler needs to see the complete
> definition each time it instantiates `HashSet<int>`, `HashSet<std::string>`, etc.

### Your work: HashSet

Implement every method marked `// TODO` in `hashset.hpp`. All data members are already declared for you. The section below restates each method's contract. Treat it as a specification, not a step-by-step guide.

**`HashSet(size_t num_buckets = 16)`**
Constructs a set with `num_buckets` empty chains and `n_elem = 0`. `num_buckets` is always >= 1.

**`size_t _bucket_index(const T& value) const`** *(private)*
Use the standard-library hash functor to calculate a hash value and then use modulo to map it to a valid bucket. Return the resulting bucket index. All other methods use this to map a value to its bucket.

**`void insert(const T& value)`**
If `value` is not in the set, add it to its bucket chain and increment `n_elem`. If it is already present, do nothing, sets have no duplicates.

**`bool contains(const T& value) const`**
Return `true` if `value` is in the set, `false` otherwise. Do not modify the set.

**`bool remove(const T& value)`**
If `value` is in the set, remove it from its chain, decrement `n_elem`, and return `true`. If not found, return `false` and leave the set unchanged.

**`size_t size() const`**
Return the number of elements.

**`bool empty() const`**
Return `true` if the set is empty.

**`float load_factor() const`**
Return the current load factor.

**`size_t bucket_count() const`**
Return the number of buckets.

**`void rehash(size_t new_bucket_count)`**
Rebuild the table with `new_bucket_count` buckets. Re-insert every existing element under the new mapping. `size()` must be unchanged after the call.

### Your work: Unit Tester

Open `tester.cpp`. The test framework (the `CHECK` macro and section functions) is already in place. For every `CHECK` call, replace the placeholder `false` with a boolean expression that verifies the described behavior. Read the comment above each `CHECK` before writing anything.

Run the tester frequently, after each method you implement, not only at the end.

### Compile and run

```bash
# Compile
$ g++ -std=c++17 -Wall -Werror -o tests tester.cpp

# Run
$ ./tests
```

> [!WARNING]
> The autograder will use a **different tester** compiled against the **same `hashset.hpp`**.
> Make sure your implementation satisfies the contracts in the header, not just these specific
> checks. Do **not** modify any declaration, signature, data member, or `#include` in
> `hashset.hpp` - only write code inside the `// TODO` method bodies.

**Expected output (all passing):**

```
insert / contains tests
[PASS] insert: single element is found (2/2 pts)
[PASS] insert: duplicate does not increase size (2/2 pts)
[PASS] contains: absent element returns false (3/3 pts)
[PASS] contains: multiple elements, all found (3/3 pts)

remove tests
[PASS] remove: existing element returns true (2/2 pts)
[PASS] remove: element no longer found after remove (3/3 pts)
[PASS] remove: absent element returns false (3/3 pts)

size / empty tests
[PASS] size: starts at zero (2/2 pts)
[PASS] size: tracks insertions and removals (2/2 pts)
[PASS] empty: returns true on fresh set (2/2 pts)

load_factor tests
[PASS] load_factor: correct after insertions (3/3 pts)
[PASS] load_factor: zero on empty set (3/3 pts)

rehash tests
[PASS] rehash: all elements survive (3/3 pts)
[PASS] rehash: bucket count changes (3/3 pts)

edge case tests
[PASS] edge: many elements, all retrievable (2/2 pts)
[PASS] edge: insert-remove-reinsert (2/2 pts)

16/16 tests passed.
Score: 40/40 pts
```

## Part 2: Genome k-mer Counter

### Background

A **k-mer** is any substring of length `k` extracted from a DNA sequence. Bioinformaticians count distinct k-mers to measure how repetitive or complex a genome is. A genome made entirely of one repeated pattern has very few distinct k-mers; a complex genome approaches the theoretical maximum of $$4^k$$ (since DNA has four bases: `A`, `T`, `C`, `G`).

For example, with `k = 3` and the sequence `ATCGATCG`:

```text
Substrings of length 3:    ATC  TCG  CGA  GAT  ATC  TCG
Distinct k-mers:           ATC  TCG  CGA  GAT  (4 unique)
Total possible (4 ** 3):   64
Complexity:                4 / 64 = 0.0625
```

The `HashSet<std::string>` is a natural fit: insert every k-mer as you slide a window across the sequence, then read `size()` for the distinct count. No frequencies or ordering needed, only membership.

### Your work

Implement the genome k-mer counter in a new file called `app.cpp`.

**Input format:** two lines from `std::cin`:
```text
<k>
<DNA sequence>
```

The sequence contains only the characters `A`, `T`, `C`, `G` (uppercase). `k` is a positive integer less than the length of the sequence.

**Required output:**
```text
Sequence length : <L>
k               : <k>
Total k-mers    : <L - k + 1>
Distinct k-mers : <size() of your HashSet>
Possible k-mers : <4^k>
Complexity      : <distinct / possible, 4 decimal places>
```

**Sample run:**

```text
$ ./app < data/small.txt
Sequence length : 8
k               : 3
Total k-mers    : 6
Distinct k-mers : 4
Possible k-mers : 64
Complexity      : 0.0625
```

> [!NOTE]
> Use a sliding window of width `k` over the sequence. At each position `i` (from `0`
> to `L - k`), extract `sequence.substr(i, k)` and insert it into your `HashSet`.
> After the loop, `size()` is your distinct k-mer count.

> [!NOTE]
> $$4^k$$ grows fast. Store it in a `long long`. For `k = 10` the possible count is
> already 1,048,576; for `k = 20` it exceeds a trillion.

**Compile and run:**

```bash
$ g++ -std=c++17 -Wall -Werror -o app app.cpp
$ ./app
```

Two sample input files are provided under `data/`. You can pipe them directly into your program to test without typing:

```bash
$ ./app < data/small.txt
$ ./app < data/medium.txt
```

`data/small.txt` uses `k=3` on the sequence `ATCGATCG` and matches the worked example above, so you can verify your output by hand. `data/medium.txt` uses `k=4` on a longer sequence where tracing by hand is impractical.

## Submission

Submit the three files below to Gradescope. All files must compile cleanly with `-std=c++17 -Wall -Werror`.

| File | Description |
|------|-------------|
| `hashset.hpp` | `HashSet<T>` implementation with separate chaining |
| `tester.cpp` | Your completed unit tester |
| `app.cpp` | Genome k-mer counter |

Code should be well-formatted and easy to read. Full credit requires **attendance**, **collaboration with your group**, and **active participation**. No remote submissions will be accepted. If you have questions, ask the instructor or TA.
