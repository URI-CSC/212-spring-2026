# Lab 09 - Autocomplete

## Overview

This lab builds two components that power real search boxes: a prefix autocomplete engine and an LRU cache. By the end, your REPL will behave exactly like a search box: fast on the first query, instant on repeated ones.

The lab has two parts:

- **Part 1**: Build prefix-based autocomplete using `std::set`, `std::map`, and `std::priority_queue`
- **Part 2**: Wrap the search in an LRU cache using `std::list` and `std::unordered_map`

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. In
> particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE.
> You may ask specific questions to LLMs about concepts or language syntax, but we
> strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> **Read each concept section fully before writing any code.** The container choices here are
> not arbitrary, understanding *why* each one is used is the point of the exercise. Discuss
> every exercise with your group before writing a single line.

## Part 1: Autocomplete

### What is prefix search?

Every time you type `"pre"` into a search bar, the engine jumps directly to the matching range in a sorted word list and scores every match by frequency. The naive approach, scan every word and keep the best, works but costs $$O(n)$$ per query. You can do better.

Storing words in a **sorted set** means all words sharing a prefix live in a contiguous range. `lower_bound` is a binary search that jumps directly to that range in $$O(\log n)$$, and the walk itself is $$O(k)$$ where $$k$$ is the number of matching words, which is usually tiny. A **priority queue** then picks the top-N matches without sorting all of them.

The provided file `data/words.txt` uses the same format as the Google Web Trillion Word Corpus, one `word frequency` pair per line. Higher frequency means the word appears more often on the web.

**Containers you'll use:**

| Container | Header | Role |
|-----------|--------|------|
| `std::set<std::string>` | `<set>` | Keeps words sorted; `lower_bound` gives $$O(\log n)$$ prefix jump |
| `std::map<std::string, int>` | `<map>` | Maps each word to its frequency |
| `std::priority_queue<std::pair<int,std::string>>` | `<queue>` | Max-heap; pops highest-frequency matches first |

### Your work: Autocomplete

Implement the following two functions in `autocomplete.cpp`.

**`void loadDictionary(filename, words, freq)`**: Opens `filename`, reads every `word frequency` pair, inserts each word into `words`, and stores its frequency in `freq`. If the file cannot be opened, print to `std::cerr` and return. Skip blank or malformed lines silently.

**`std::vector<std::string> autocomplete(prefix, words, freq, n)`**: Returns up to `n` words from `words` that begin with `prefix`, ordered by frequency descending. Returns an empty vector if there are no matches. An empty prefix matches every word.

Algorithm sketch:
1. Call `words.lower_bound(prefix)` to find the first word `>= prefix`.
2. Walk forward. At each step, check whether the current word still starts with `prefix`, stop as soon as it doesn't.
3. Push each matching `{frequency, word}` pair into a `priority_queue`.
4. Pop up to `n` elements into your result vector.

> [!NOTE]
> **What is an iterator?** Think of it as a typed pointer to one element inside a
> container. `words.lower_bound(prefix)` returns a
> `std::set<std::string>::iterator` pointing to the first word `>= prefix`. Advance
> it with `++it`, read the current word with `*it`, and check for the end of the
> set with `it != words.end()`. The `->` operator also works: `it->size()` is
> equivalent to `(*it).size()`.

> [!NOTE]
> `std::priority_queue` is a max-heap by default. Pushing `{frequency, word}` pairs means
> the word with the highest frequency always comes out first.

**Sample output:**

```
Dictionary loaded: 162 words
Enter prefix (or 'q' to quit): pre
  1. present      (831240099)
  2. prefix       (822561823)
  3. pretty       (815230077)
  4. print        (798442011)
  5. problem      (789123456)

Enter prefix (or 'q' to quit): q
Goodbye!
```

Once you have implemented `loadDictionary` and `autocomplete`, compile and launch the interactive REPL from the lab directory:

```bash
$ g++ -std=c++17 -Wall -Werror -o autocomplete autocomplete.cpp
$ ./autocomplete
```

The program loads `data/words.txt` and waits for a prefix. Try inputs like `pre`, `str`, `pro`, or `the`. Press Enter with an empty prefix to see the top-5 most frequent words overall. Type `q` to quit.

> [!NOTE]
> Run the program from the lab root directory (where the `data/` folder lives). If the path
> `data/words.txt` cannot be found, you will get a file-not-found error from `loadDictionary`.

## Part 2: LRU Cache

### What is an LRU cache?

If a user types `"p"`, `"pr"`, `"pre"`, `"pres"` in rapid succession, you're doing essentially the same prefix walk four times. Real search engines cache the results of recent queries so that repeated inputs cost $$O(1)$$, a hash map lookup, instead of re-scanning the dictionary.

An **LRU (Least Recently Used) cache** keeps the last `capacity` query results. When the cache is full, it evicts the entry used least recently. The trick is doing both lookup and eviction in $$O(1)$$.

A plain hash map gives fast lookup but no ordering. A linked list gives ordering and $$O(1)$$ front/back operations, but no fast lookup. Combining them gives both.

**Containers you'll use:**

| Container | Header | Role |
|-----------|--------|------|
| `std::list<std::pair<std::string, std::vector<std::string>>>` | `<list>` | Doubly-linked; MRU (Most Recently Used) entry at front, LRU (Least Recently Used) at back |
| `std::unordered_map<std::string, std::list<...>::iterator>` | `<unordered_map>` | $$O(1)$$ lookup; stores an *iterator* into the list, not a copy |

> [!WARNING]
> Your `unordered_map` stores **iterators** into the list, not copies of the data.
> `std::list` iterators stay valid as long as the node exists. Never copy the list itself,
> those iterators become dangling pointers.

> [!NOTE]
> **List iterators as map values.** A `std::list<T>::iterator` is a stable handle
> to one node in the list, it remains valid as long as that node exists (unlike
> vector iterators, which invalidate on resize). Storing one in the map lets you
> jump directly to any cache entry in $$O(1)$$. Dereference with `*it` to get the
> `{key, value}` pair, or use `it->first` / `it->second` to access key and value
> directly.

### Your work: LRU Cache

Implement the `LRUCache` class in `lru.cpp`.

**`LRUCache(int capacity)`**: Constructs a cache holding at most `capacity` entries. Capacity is always >= 1.

**`std::vector<std::string> get(const std::string& key)`**: On a cache hit, move the entry to the front of the list and return the cached value. On a miss, return an empty vector. Track hit and miss counts.

**`void put(const std::string& key, const std::vector<std::string>& value)`**: If the key exists, update its value and move it to the front. If it's new, insert at the front and update the map. If size now exceeds `capacity`, remove the back element from both the list and the map.

**`double hitRate() const`**: Returns the percentage of `get()` calls that were cache hits, as a value between 0.0 and 100.0. Returns 0.0 if `get()` has never been called.

> [!NOTE]
> `std::list::splice` can move a node to the front without any copying or memory allocation.
> It takes an iterator, not a value. Look it up, it is the right tool here.

Wire the cache into your REPL in `lru.cpp`: on each query, start the timer, call `cache.get(prefix)`, and run autocomplete only on a miss (then `cache.put`). Stop the timer, then print `[HIT] X µs` or `[MISS] X µs` on one line, followed by the numbered results. Print the final hit rate when the user quits.

**Sample output:**

```
Dictionary loaded: 162 words
Cache capacity: 5
Enter prefix (or 'q' to quit): pre
[MISS] 12 µs
  1. present      (831240099)
  2. prefix       (822561823)

Enter prefix (or 'q' to quit): pre
[HIT] 1 µs
  1. present      (831240099)
  2. prefix       (822561823)

Enter prefix (or 'q' to quit): q
Final hit rate: 50.00%
Goodbye!
```

Once you have implemented `LRUCache` and wired it into `main()`, compile `lru.cpp` directly. For the standalone binary it contains its own copies of the `loadDictionary` and `autocomplete` stubs — fill them in with the same implementations you wrote in `autocomplete.cpp`, then compile:

```bash
$ g++ -std=c++17 -Wall -Werror -o lru lru.cpp
$ ./lru
```

The program loads the dictionary, then wraps every autocomplete query in the LRU cache. Each query prints `[HIT]` or `[MISS]` and the elapsed time in microseconds. To observe a cache hit, type the same prefix twice in a row, the second query should print `[HIT]` and be noticeably faster. Type `q` to quit and see the final hit rate.

> [!NOTE]
> Do **not** compile `autocomplete.cpp` and `lru.cpp` together on the command line. `lru.cpp`
> declares its own stubs for `loadDictionary` and `autocomplete`, so passing both files to
> the compiler would produce duplicate symbol errors.

## Testing Your Code

`unittester.cpp` is an automated tester that compiles both of your source files and runs 15 checks, one per function behavior. Each test reports `[PASS]` or `[FAIL]` and its point value. Your score out of 40 is printed at the end.

**Do not wait until you finish both parts to run the tester.** Run it after implementing each function to catch mistakes early.

### Step 1: Compile the tester

```bash
$ g++ -std=c++17 -Wall -Werror -DTESTING -o tests unittester.cpp
```

The `-DTESTING` flag tells the compiler to skip the `main()` functions inside `autocomplete.cpp` and `lru.cpp` so the tester can supply its own. You must recompile every time you change your code.

### Step 2: Run the tests

```bash
$ ./tests
```

**Expected output (all passing):**

```
── loadDictionary tests ──────────────────
[PASS] loadDictionary: file loads without error (2/2 pts)
[PASS] loadDictionary: word count matches (2/2 pts)
[PASS] loadDictionary: specific word present (3/3 pts)
[PASS] loadDictionary: frequency correct (3/3 pts)

── autocomplete tests ────────────────────
[PASS] autocomplete: empty prefix returns top-N (3/3 pts)
[PASS] autocomplete: prefix matches correct words (3/3 pts)
[PASS] autocomplete: results ordered by frequency (4/4 pts)
[PASS] autocomplete: prefix with no matches returns empty (2/2 pts)
[PASS] autocomplete: fewer than N matches returns all (3/3 pts)

── LRUCache tests ────────────────────────
[PASS] LRUCache: get on empty cache returns empty (2/2 pts)
[PASS] LRUCache: put then get returns correct value (2/2 pts)
[PASS] LRUCache: capacity eviction removes LRU (3/3 pts)
[PASS] LRUCache: hit rate calculation (3/3 pts)
[PASS] LRUCache: move-to-front on hit updates LRU order (3/3 pts)
[PASS] LRUCache: edge case capacity=1 (2/2 pts)

─────────────────────────────────────────
15/15 tests passed.
Score: 40/40 pts
```

### Step 3: Interpreting failures

A `[FAIL]` line tells you which behavior is broken. Re-read the exercise description for that function, trace through the test case by hand, and fix before moving on.

> [!WARNING]
> Only write code inside the `// TODO` blocks. Do **not** rename functions, change parameter
> types, remove `#include` lines, or modify anything outside a TODO comment. The unit tester
> calls your functions by their exact names and signatures, any mismatch will cause a compile
> error or a wrong result.

> [!NOTE]
> If the tester does not compile, the most common cause is a modified function signature or
> a missing `#ifndef TESTING` guard around `main()` in your source files. Do **not** remove
> that guard, it is what allows the tester to include your file without a duplicate `main`.

## Submission

After completing both parts, submit the files below to Gradescope. All files must compile
cleanly with:

```bash
$ g++ -std=c++17 -Wall -Werror <file>.cpp -o <file>
```

Code should be well-formatted and easy to read. Test each program with several inputs before
submitting. Full credit requires **attendance**, **collaboration with your group**, and
**active participation**. No remote submissions will be accepted. If you have any questions,
ask the instructor or TA.

| File | Description |
|------|-------------|
| `autocomplete.cpp` | Prefix autocomplete with `std::set` and `std::priority_queue` |
| `lru.cpp` | LRU cache with HIT/MISS reporting and timing |
