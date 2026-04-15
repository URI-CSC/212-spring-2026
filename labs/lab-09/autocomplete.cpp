// autocomplete.cpp
// Lab 09 — Search Like a Machine, Part 1
// CSC 212 Data Structures
//
// Your job: implement loadDictionary() and autocomplete().
// Do NOT change function signatures — the unit tester depends on them.
//
// Containers you'll need:
//   #include <set>            → std::set<std::string>
//   #include <map>            → std::map<std::string, int>
//   #include <queue>          → std::priority_queue<std::pair<int,std::string>>
//
// Compile (no tests):
//   g++ -std=c++17 -Wall -Werror -o autocomplete autocomplete.cpp
//
// Compile (with unit tester):
//   g++ -std=c++17 -Wall -Werror -DTESTING -o tests unittester.cpp
//
// AUTOCOMPLETE_INCLUDED tells lru.cpp (when both are included by the tester)
// that loadDictionary and autocomplete are already defined here.
#define AUTOCOMPLETE_INCLUDED

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// loadDictionary
//
// Opens `filename` and populates two data structures:
//   - `words`: a sorted set of every word in the file
//   - `freq`:  a map from word → frequency count
//
// File format — one entry per line:
//   word frequency
//   e.g. "the 23135851162"
//
// If the file cannot be opened, print an error to std::cerr and return early.
// Blank lines and malformed lines (no space, missing frequency) should be
// skipped silently.
// ─────────────────────────────────────────────────────────────────────────────
void loadDictionary(const std::string& filename,
                    std::set<std::string>& words,
                    std::map<std::string, int>& freq) {
    // TODO: open the file with std::ifstream
    // TODO: read each line — extract the word and frequency
    // TODO: insert word into `words`, store frequency in `freq`
    // TODO: handle file-not-found by printing to std::cerr and returning
}

// ─────────────────────────────────────────────────────────────────────────────
// autocomplete
//
// Returns up to `n` words from `words` that begin with `prefix`,
// ordered by frequency descending (most frequent first).
//
// Parameters:
//   prefix  — the string the user has typed (may be empty)
//   words   — sorted set from loadDictionary
//   freq    — frequency map from loadDictionary
//   n       — max results to return (always >= 1)
//
// Returns:
//   A vector of up to n matching words, highest frequency first.
//   Empty prefix → n most-frequent words overall.
//   Fewer than n matches → return all matches.
//   No matches → return empty vector.
//
// Key containers to use inside this function:
//   std::set::lower_bound(prefix)     → O(log n) jump to first candidate
//   std::priority_queue<std::pair<int,std::string>>  → top-N extraction
// ─────────────────────────────────────────────────────────────────────────────
std::vector<std::string> autocomplete(const std::string& prefix,
                                       const std::set<std::string>& words,
                                       const std::map<std::string, int>& freq,
                                       int n) {
    std::vector<std::string> results;

    // TODO: use words.lower_bound(prefix) to find the start of the matching range
    //
    // TODO: walk forward through the iterator while the current word
    //       still starts with `prefix` — think carefully about your
    //       loop condition. When do you stop?
    //
    // TODO: for each matching word, push {frequency, word} onto a
    //       priority_queue (pairs compare on first element → max-heap
    //       automatically orders by frequency)
    //
    // TODO: pop up to n elements from the queue into `results`

    return results;
}

// ─────────────────────────────────────────────────────────────────────────────
// main — interactive REPL
// Guarded by #ifndef TESTING so the unit tester can include this file
// without getting a duplicate main().
// ─────────────────────────────────────────────────────────────────────────────
#ifndef TESTING
int main() {
    std::set<std::string> words;
    std::map<std::string, int> freq;

    // TODO: call loadDictionary with the path to data/words.txt
    //       print how many words were loaded

    // TODO: set TOP_N = 5 here once you're ready to call autocomplete()

    std::string prefix;

    std::cout << "Enter prefix (or 'q' to quit): ";
    while (std::getline(std::cin, prefix)) {
        if (prefix == "q") break;

        // TODO: call autocomplete(prefix, words, freq, TOP_N)
        // TODO: print results with their frequencies
        //       e.g. "  1. present      (831240099)"

        std::cout << "\nEnter prefix (or 'q' to quit): ";
    }

    std::cout << "Goodbye!\n";
    return 0;
}
#endif
