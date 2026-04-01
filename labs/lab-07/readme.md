# Lab 07 - Sets and Maps

## Overview

In this lab you will explore two of the most powerful **containers** in the C++ Standard Template
Library: **`std::set`** and **`std::map`**. Both give you fast, organized access to data without
writing a single loop to search. By the end of this lab you will know *what* each container is,
*why* it exists, and *when* to reach for it.

The lab has four parts:

- **Part 1**: Learn what a `set` is and use it to analyze social network connections.
- **Part 2**: Learn what a `map` is and use it to decode Roman numerals.
- **Part 3**: Use two maps together to build a Morse code translator.
- **Part 4**: Combine a `map` with sorting to find the most frequent words in any text.

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. In
> particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE.
> You may ask specific questions to LLMs about concepts or language syntax, but we
> strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> **Read each concept section fully before writing any code**. Sets and maps solve problems that
> loops also solve, but more cleanly and faster. Your goal is not just to make things work but
> to understand *why* these containers exist and *when* to use them. Discuss every exercise with
> your group before writing a single line.

## Part 1: `std::set`

### What Is a Set?

A **set** is a collection that holds each value **at most once**. Inserting a duplicate does
nothing, the set silently keeps one copy. C++'s `std::set` also keeps its elements in
**sorted order** at all times, with no effort from you. Sets are ideal for storing a collection of unique items when you need to check membership frequently, that is, you only care whether something is present or absent. Examples: a dictionary of valid words or a collection of unique IDs.

Internally, `std::set` uses a **balanced binary search tree**, so every insert, lookup, and
erase runs in **O(log n)** time, far faster than scanning an unsorted array, and you never
have to manage the tree yourself.

The core operations:

| Operation | Code | Notes |
|-----------|------|-------|
| Insert | `s.insert(x)` | Ignored if `x` is already present |
| Check membership | `s.count(x)` | Returns `1` if present, `0` otherwise |
| Remove | `s.erase(x)` | Does nothing if `x` is absent |
| Size | `s.size()` | Number of unique elements |
| Iterate | `for (const auto& x : s)` | Always visits elements in sorted order |

### A First Look

Type, compile, and run the program below. Observe the output carefully, pay attention to the
order of the elements and what happens to the duplicate.

```cpp
#include <iostream>
#include <set>
#include <string>

int main() {
    std::set<std::string> fruits;

    fruits.insert("banana");
    fruits.insert("apple");
    fruits.insert("cherry");
    fruits.insert("apple");   // duplicate: silently ignored
    fruits.insert("date");

    std::cout << "Set has " << fruits.size() << " fruits:\n";
    for (const std::string& f : fruits) {
        std::cout << "  " << f << "\n";
    }

    if (fruits.count("apple") > 0) {
        std::cout << "apple is in the set\n";
    }

    fruits.erase("banana");
    std::cout << "After erasing banana, size = " << fruits.size() << "\n";
    return 0;
}
```

```bash
$ g++ -std=c++11 -Wall -Werror first_set.cpp -o first_set
$ ./first_set
```

> [!NOTE]
> Even though `"apple"` was inserted twice, the set holds it once. The output is also
> alphabetically sorted. You never called any sort function. That ordering is free.

### Set Operations: Intersection, Difference, Union

Given two sets A and B, three classic operations let you compare and combine them:

| Operation | Meaning | How to implement in C++ |
|-----------|---------|------------------------|
| **Intersection** A ∩ B | Elements in **both** A and B | Loop over A; keep elements where `B.count(x) > 0` |
| **Difference** A − B | Elements in A but **not** in B | Loop over A; keep elements where `B.count(x) == 0` |
| **Union** A ∪ B | Elements in **either** A or B | Copy A into a new set; insert every element of B |

```cpp
// Intersection example
std::set<std::string> mutual;
for (const std::string& x : A) {
    if (B.count(x) > 0) {
        mutual.insert(x);
    }
}
```

The union is even simpler, insert all of B into a copy of A and let the set handle
duplicates automatically.

### Exercise: Mutual Friends

**Scenario:** Alice and Bob each have a list of friends. Your program reads both lists and
reports:

1. Their **mutual friends**
2. Friends **only Alice** has
3. Friends **only Bob** has
4. **Everyone** combined

The program reads from standard input. Each list ends with the sentinel word `END`.

**Sample run:**

```
$ ./friends
Alice's friends (END to finish): Bob Carol Dave Eve END
Bob's   friends (END to finish): Carol Dave Frank Grace END

Mutual friends   : Carol Dave
Only Alice knows : Bob Eve
Only Bob knows   : Frank Grace
Everyone         : Bob Carol Dave Eve Frank Grace
```

### Skeleton

```cpp
#include <iostream>
#include <set>
#include <string>

// Reads names from stdin until the sentinel "END".
std::set<std::string> readFriends(const std::string& prompt) {
    std::set<std::string> friends;
    std::cout << prompt;
    std::string name;
    while (std::cin >> name && name != "END") {
        friends.insert(name);
    }
    return friends;
}

// Prints every element of a set on one line, separated by spaces.
void printSet(const std::set<std::string>& s) {
    for (const std::string& name : s) {
        std::cout << name << " ";
    }
    std::cout << "\n";
}

// Returns all elements that appear in both A and B.
std::set<std::string> intersect(const std::set<std::string>& A,
                                const std::set<std::string>& B) {
    std::set<std::string> result;
    // TODO: loop over A; insert element into result if it also appears in B
    return result;
}

// Returns elements that are in A but NOT in B.
std::set<std::string> difference(const std::set<std::string>& A,
                                 const std::set<std::string>& B) {
    std::set<std::string> result;
    // TODO: loop over A; insert element into result if it does not appear in B
    return result;
}

// Returns all elements that appear in A or B (or both).
std::set<std::string> unionOf(const std::set<std::string>& A,
                              const std::set<std::string>& B) {
    std::set<std::string> result;
    // TODO: insert all elements of A, then all elements of B.
    //       The set handles duplicates automatically.
    return result;
}

#ifndef TESTING
int main() {
    std::set<std::string> alice = readFriends("Alice's friends (END to finish): ");
    std::set<std::string> bob   = readFriends("Bob's   friends (END to finish): ");

    std::cout << "Mutual friends   : "; printSet(intersect(alice, bob));
    std::cout << "Only Alice knows : "; printSet(difference(alice, bob));
    std::cout << "Only Bob knows   : "; printSet(difference(bob,   alice));
    std::cout << "Everyone         : "; printSet(unionOf(alice, bob));
    return 0;
}
#endif
```

```bash
$ g++ -std=c++11 -Wall -Werror friends.cpp -o friends
$ ./friends
```

> [!NOTE]
> Because `std::set` keeps its elements sorted, your output will always be alphabetical even
> if the user types names in a random order. Sorted output completely free.

> [!WARNING]
> Do not use `<algorithm>`'s `std::set_intersection` or `std::set_union` for this exercise.
> Write the loops yourself. That is where the understanding comes from.

## Part 2: `std::map`

### What Is a Map?

A **map** stores **<key, value>** pairs. Given a key, you get its value in O(log n) time.
Every key is unique; two entries cannot share the same key, but different keys can share the
same value. Maps are ideal for storing a collection of items where each item has an associated value you want to look up. Examples: <word, frequency>, <student name, grade>, <letter, morse code>. Think of it as a real dictionary: you look up a word (key) to find its definition (value). Or
a phone book: a name maps to a number.

The notation used by the STL is similar to the use of square brackets for arrays. Instead of `arr[index]`, you write `m[key]` to look up the value associated with `key` in map `m`. For example, `dict["apple"]` gives you the value associated with the key `"apple"` in the map `dict`. If `"apple"` is not present, `dict["apple"]` will create a new entry with a default value (e.g., `0` for integers, empty string for strings) and return that.

The core operations:

| Operation | Code | Notes |
|-----------|------|-------|
| Insert / update | `m[key] = value` | Creates entry if missing; overwrites if present |
| Read | `m[key]` | Returns value; **creates a zero-default entry if key is absent** |
| Safe read | `m.at(key)` | Returns value; throws `std::out_of_range` if absent |
| Check key | `m.count(key)` | `1` if present, `0` otherwise |
| Remove | `m.erase(key)` | Does nothing if key is absent |
| Iterate | `for (auto& p : m)` | `p.first` = key, `p.second` = value; sorted by key |

### A First Look

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // Map from student name to their score.
    std::map<std::string, int> score;

    score["Alice"] = 95;
    score["Bob"]   = 87;
    score["Carol"] = 92;

    std::cout << "Bob's score: " << score["Bob"] << "\n";

    // Iterate: keys come out in sorted order
    for (const auto& entry : score) {
        std::cout << entry.first << " -> " << entry.second << "\n";
    }

    if (score.count("Dave") == 0) {
        std::cout << "Dave has no score yet\n";
    }
    return 0;
}
```

> [!WARNING]
> Using `m[key]` when the key does not exist **silently creates it** with a zero or empty
> default value. Always check with `m.count(key)` first, or use `m.at(key)` if you want
> an exception instead of silent creation.

### Exercise: Roman Numeral Decoder

Roman numerals assign integer values to letters:

| Symbol | I | V | X | L | C | D | M |
|--------|---|---|---|---|---|---|---|
| Value | 1 | 5 | 10 | 50 | 100 | 500 | 1000 |

The decoding rule: scan the string left to right. If the current symbol's value is **less
than** the next symbol's value, **subtract** it; otherwise **add** it.

Examples:
- `XIV`: X(10) + [I < V, so -1] + V(5) = **14**
- `XLII`: [X < L, so -10] + L(50) + I(1) + I(1) = **42**
- `MCMXCIV`: 1000 - 100 + 1000 - 10 + 100 - 1 + 5 = **1994**

The map makes this elegant: instead of a long `if/else` chain, you build a
`map<char, int>` and look up each character's value in one line.

**Sample run:**

```
$ ./roman XIV
14
$ ./roman MCMXCIV
1994
$ ./roman XLII
42
```

### Skeleton

```cpp
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

// Decodes a Roman numeral string and returns its integer value.
int decodeRoman(const std::string& roman) {
    std::map<char, int> values;
    // TODO: fill map `values` with <symbol, value> pairs. 
    //       For example, 'I' and 1, 'V' and 5, etc.

    int result = 0;
    // TODO: iterate through every character in `roman`.
    //   For each character at position i:
    //     - Look up values[roman[i]] and values[roman[i+1]] (check i+1 is valid first).
    //     - If the current value is less than the next value, subtract it.
    //     - Otherwise, add it.
    return result;
}

#ifndef TESTING
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <roman_numeral>\n";
        return 1;
    }
    std::cout << decodeRoman(argv[1]) << "\n";
    return 0;
}
#endif
```

```bash
$ g++ -std=c++11 -Wall -Werror roman.cpp -o roman
$ ./roman XIV
$ ./roman MCMXCIV
$ ./roman XLII
```

> [!NOTE]
> Try a few more numerals: `LVIII` (58), `CDXLIV` (444), `MMXXVI` (2026). Trace each one
> by hand before you trust your program.

## Part 3: Morse Code

### The Reverse Map Pattern

Sometimes you need to look things up in **both directions**: given a letter, what is its code?
Given a code, what letter does it represent? A single map only goes one way. The solution is
to build **two maps**: one for each direction, and generate the second one automatically
from the first.

```cpp
std::map<char, std::string> encode;  // letter => morse code
std::map<std::string, char> decode;  // morse code => letter
```

After filling `encode`, generating `decode` is a single loop:

```cpp
for (const auto& entry : encode) {
    decode[entry.second] = entry.first; // flip: value becomes key, key becomes value
}
```

This **reverse map** pattern appears constantly in real software: URL routing tables, compiler
symbol tables, config key lookups, translation layers. Recognizing it is worth more than any
single algorithm.

### Morse Code Basics

Each letter maps to a sequence of dots (`.`) and dashes (`-`):

```
A .-    B -...  C -.-.  D -..   E .     F ..-.
G --.   H ....  I ..    J .---  K -.-   L .-..
M --    N -.    O ---   P .--.  Q --.-  R .-.
S ...   T -     U ..-   V ...-  W .--   X -..-
Y -.--  Z --..
```

**Encoding format used in this lab:**
- Letters within a word are separated by a **single space**.
- Words are separated by ` / ` (space, slash, space).

Examples:

```
SOS =>           ... --- ...
HELLO WORLD =>   .... . .-.. .-.. --- / .-- --- .-. .-.. -..
```

### Exercise: Morse Code Translator

Your program accepts one command-line argument (`encode` or `decode`) and reads one line
from standard input.

- **Encode mode:** reads uppercase letters and spaces, prints the Morse representation.
- **Decode mode:** reads a Morse string (letter codes separated by spaces, words by ` / `),
  prints the original text.

**Sample run:**

In the examples below, the `|` character represents a pipeline between two commands. When `|` is used, the output of the command on the left is fed as input to the command on the right. For example, `echo "SOS" | ./morse encode` sends the string `"SOS"` to the `morse` program in encode mode.

```
$ echo "SOS" | ./morse encode
... --- ...

$ echo "... --- ..." | ./morse decode
SOS

$ echo "HELLO WORLD" | ./morse encode
.... . .-.. .-.. --- / .-- --- .-. .-.. -..

$ echo "HELLO WORLD" | ./morse encode | ./morse decode
HELLO WORLD
```

> [!NOTE]
> That last command is the real test: encode a message, then immediately pipe the result into
> decode. If you get back exactly what you started with, both directions are correct.

### Skeleton

```cpp
#include <iostream>
#include <map>
#include <vector>
#include <string>

// Splits 's' on every occurrence of 'delim' and returns the tokens.
// split("A B C", " ")     ->  {"A", "B", "C"}
// split("HI / THERE", " / ")  ->  {"HI", "THERE"}
std::vector<std::string> split(const std::string& s, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t pos = 0, found;
    while ((found = s.find(delim, pos)) != std::string::npos) {
        tokens.push_back(s.substr(pos, found - pos));
        pos = found + delim.size();
    }
    tokens.push_back(s.substr(pos));
    return tokens;
}

// Builds and returns the decode map by reversing the encode map.
// Every (letter -> code) entry becomes a (code -> letter) entry.
std::map<std::string, char> buildDecodeMap(const std::map<char, std::string>& encode) {
    std::map<std::string, char> decode;
    // TODO: loop over encode; for each entry, insert a flipped entry into decode.
    return decode;
}

// Encodes a plaintext string into Morse.
// Letters are separated by " ", words by " / ".
std::string encodeMessage(const std::string& text,
                          const std::map<char, std::string>& encode) {
    std::string result;
    // TODO: iterate character by character through text.
    //   - If the character is ' ' (word boundary), append " / " to result.
    //   - If encode.count(c) > 0, append the code; separate letter codes with " ".
    //   Hint: track whether you have already output a letter in the current word
    //         so you know when to prepend a space before the next code.
    return result;
}

// Decodes a Morse string back into plaintext.
std::string decodeMessage(const std::string& morse,
                          const std::map<std::string, char>& decode) {
    std::string result;
    // TODO: split morse by " / " to get words.
    //   - For each word, split by " " to get individual codes.
    //   - Look up each code in decode and append the letter to result.
    //   - Append a space ' ' between words.
    return result;
}

#ifndef TESTING
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " encode|decode\n";
        return 1;
    }

    std::map<char, std::string> encode;
    encode['A'] = ".-";   encode['B'] = "-..."; encode['C'] = "-.-.";
    encode['D'] = "-..";  encode['E'] = ".";    encode['F'] = "..-.";
    encode['G'] = "--.";  encode['H'] = "...."; encode['I'] = "..";
    encode['J'] = ".---"; encode['K'] = "-.-";  encode['L'] = ".-..";
    encode['M'] = "--";   encode['N'] = "-.";   encode['O'] = "---";
    encode['P'] = ".--."; encode['Q'] = "--.-"; encode['R'] = ".-.";
    encode['S'] = "...";  encode['T'] = "-";    encode['U'] = "..-";
    encode['V'] = "...-"; encode['W'] = ".--";  encode['X'] = "-..-";
    encode['Y'] = "-.--"; encode['Z'] = "--..";

    std::map<std::string, char> decode = buildDecodeMap(encode);

    std::string line;
    std::getline(std::cin, line);

    std::string mode = argv[1];
    if (mode == "encode") {
        std::cout << encodeMessage(line, encode) << "\n";
    } else if (mode == "decode") {
        std::cout << decodeMessage(line, decode) << "\n";
    } else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }
    return 0;
}
#endif
```

```bash
$ g++ -std=c++11 -Wall -Werror morse.cpp -o morse
$ echo "SOS" | ./morse encode
$ echo "... --- ..." | ./morse decode
$ echo "HELLO WORLD" | ./morse encode | ./morse decode
```

> [!WARNING]
> The `split` helper treats the delimiter as a **string**, not a character. Splitting on `" / "`
> (three characters) is deliberate as it correctly separates words without confusing the
> slash-space sequence with the spaces between letter codes.

## Part 4: Top-K Most Frequent Words

### The Pattern: Count, Transfer, Sort, Display

Finding the most frequent items in a dataset is one of the most common tasks in software. It appears in search engines, recommendation systems, log analysis, spam filters, and text
editors. The recipe always follows the same four steps:

1. **Count**: use a `map<string, int>` to tally how many times each word appears.
2. **Transfer**: copy the map entries into a `vector<pair<string, int>>` so they can be sorted.
3. **Sort**: sort the vector by count, descending.
4. **Display**: print the first K entries.

The map shines at step 1. Counting a word is a single, clean line:

```cpp
freq[word]++;
```

When `word` appears for the first time, `operator[]` creates the entry with value `0`, and
`++` immediately makes it `1`. Every subsequent occurrence simply increments the existing
entry. No `if` check, no initialization loop, just one line.

### Moving from a Map to a Sorted List

`std::map` is always sorted by **key**. Here you need to sort by **value** (the count). Maps **cannot be reordered by value**, so you transfer the data to a vector of pairs first:

```cpp
std::vector<std::pair<std::string, int>> ranked(freq.begin(), freq.end());
```

A `std::pair<A, B>` is a simple struct with two public fields: `.first` (type A) and
`.second` (type B). Because `map<K, V>` is a collection of `pair<K, V>`, you can initialize the vector directly from the map's begin/end iterators in one line.

Then sort with a custom comparator:

```cpp
std::stable_sort(ranked.begin(), ranked.end(),
    [](const std::pair<std::string, int>& a,
       const std::pair<std::string, int>& b) {
        return a.second > b.second;   // higher count first
    });
```

> [!NOTE]
> `std::stable_sort` preserves the original relative order of equal elements. Because the
> map fed the vector in alphabetical key order, ties between words with equal counts will
> automatically be broken alphabetically.

### Normalizing Words

Raw text is messy. `"The"`, `"the"`, and `"the,"` should all count as the same word.
Before inserting into the frequency map, normalize each token: lowercase it and strip
non-alphabetic characters.

```cpp
std::string normalize(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) {
            result += std::tolower(c);
        }
    }
    return result;
}
```

### Exercise: Top-K Most Frequent Words

Your program reads `K` from the command line and text from standard input. It prints the `K` most frequent words with their counts, aligned in a clean table.

**Sample run:**

```
$ echo "to be or not to be that is the question to be" | ./topk 4
be                  3
to                  3
is                  1
not                 1
```

Try it on something larger: paste a speech, a song, or a book excerpt into a `.txt` file then run:

```bash
$ ./topk 15 < gettysburg.txt
```

The results are often surprising. Function words like `"the"`, `"of"`, and `"and"` dominate most English text, but once you look past them, the top words paint a clear picture of what the text is actually *about*.

### Skeleton

```cpp
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>

// Returns a lowercase, letters-only version of word.
// "Hello,"  ->  "hello"
// "don't"   ->  "dont"
// "123"     ->  ""  (empty: caller should skip these)
std::string normalize(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) {
            result += std::tolower(c);
        }
    }
    return result;
}

// Counts how many times each normalized word appears in text.
// Tokens are whitespace-separated. Empty normalized tokens are ignored.
std::map<std::string, int> buildFreqMap(const std::string& text) {
    std::map<std::string, int> freq;
    std::istringstream ss(text);
    std::string token;
    while (ss >> token) {
        std::string word = normalize(token);
        if (!word.empty()) {
            // TODO: update the frequency of 'word' in the map. 
            //       Hint: one line, no if/else.
        }
    }
    return freq;
}

// Returns a vector of (word, count) pairs sorted by count descending.
// If freq has fewer than k entries, returns all of them.
std::vector<std::pair<std::string, int>> topK(const std::map<std::string, int>& freq,
                                              int k) {
    // Step 1: copy map entries into a vector of pairs.
    std::vector<std::pair<std::string, int>> ranked(freq.begin(), freq.end());

    // Step 2: sort by count descending; stable_sort keeps alphabetical order for ties.
    // TODO: call std::stable_sort with a lambda that compares .second descending.

    // Step 3: trim to at most k entries.
    // TODO: if ranked.size() > k, resize ranked to k.

    return ranked;
}

#ifndef TESTING
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <k>\n";
        return 1;
    }
    int k = std::atoi(argv[1]);

    // Read all of stdin into a single string, then process.
    std::string text, line;
    while (std::getline(std::cin, line)) {
        text += line + " ";
    }

    std::vector<std::pair<std::string, int>> ranked = topK(buildFreqMap(text), k);

    for (const auto& p : ranked) {
        std::cout << std::left << std::setw(20) << p.first << p.second << "\n";
    }
    return 0;
}
#endif
```

```bash
$ g++ -std=c++11 -Wall -Werror topk.cpp -o topk
$ echo "to be or not to be that is the question to be" | ./topk 4
$ ./topk 10 < yourfile.txt
```

> [!NOTE]
> `std::map` uses a balanced binary search tree internally, so every insert and lookup runs 
> in **O(log n)** time. A related container, `std::unordered_map`, uses a hash table for
> **O(1)** average lookup at the cost of losing the sorted-key property. For most counting
> tasks either works; for problems that require the keys in order, `std::map` is the right
> choice.

> [!WARNING]
> `std::sort` does not guarantee stable ordering of equal elements. Use `std::stable_sort`
> so that words with identical counts remain in the alphabetical order they had when they
> came out of the map.

## Testing Your Code

A unit tester is provided in `unittester.cpp`. It compiles all four of your source files
together and runs a suite of checks against each core function.

```bash
$ g++ -std=c++11 -Wall -Werror unittester.cpp -o tester
$ ./tester
```

> [!WARNING]
> Only write code inside the `// TODO` blocks. Do **not** rename functions, change
> parameter types, remove `#include` lines, or modify anything outside a TODO comment.
> The unit tester calls your functions by their exact names and signatures. Any
> mismatch will cause a compile error or wrong result.

Passing output looks like:

```
--- Part 1: Friends (Sets) ---
  PASS  intersect: common friends
  PASS  intersect: empty set yields empty
  ...

Results: 65/65 passed
```

> [!NOTE]
> The tester uses `#define TESTING` to include your source files without triggering their
> `main()` functions. Your files must keep the `#ifndef TESTING` guard around `main` exactly
> as shown in the skeletons. DO NOT remove it.

## Submission

After completing all parts, submit the files below to Gradescope. All files must compile
cleanly with:

```bash
$ g++ -std=c++11 -Wall -Werror <file>.cpp -o <file>
```

Code should be well-formatted and easy to read. Test each program with several inputs before
submitting. Full credit requires **attendance**, **collaboration with your group**, and
**active participation**. No remote submissions will be accepted. If you have any questions,
ask the instructor or TA.

| File | Description |
|------|-------------|
| `friends.cpp` | Mutual friends via set operations |
| `roman.cpp` | Roman numeral decoder |
| `morse.cpp` | Morse code encoder and decoder |
| `topk.cpp` | Top-K most frequent words |

