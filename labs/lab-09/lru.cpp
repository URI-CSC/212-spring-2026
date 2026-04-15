// lru.cpp
// Lab 09 — Search Like a Machine, Part 2
// CSC 212 Data Structures
//
// Your job: implement the LRUCache class, then wire it around autocomplete()
// in main() with HIT/MISS reporting and <chrono> timing.
//
// Containers you'll need inside LRUCache:
//   std::list<std::pair<std::string, std::vector<std::string>>>
//       → doubly-linked list; front = MRU, back = LRU
//   std::unordered_map<std::string, std::list<...>::iterator>
//       → O(1) lookup; stores an iterator into the list, not a copy
//
// Compile (no tests):
//   g++ -std=c++17 -Wall -Werror -o lru lru.cpp
//
// Compile (with unit tester):
//   g++ -std=c++17 -Wall -Werror -DTESTING -o tests unittester.cpp

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// Forward-declare autocomplete helpers so main() can call them.
// (In a real project these would live in a separate header.)
// ─────────────────────────────────────────────────────────────────────────────
void loadDictionary(const std::string& filename,
                    std::set<std::string>& words,
                    std::map<std::string, int>& freq);

std::vector<std::string> autocomplete(const std::string& prefix,
                                       const std::set<std::string>& words,
                                       const std::map<std::string, int>& freq,
                                       int n);

// ─────────────────────────────────────────────────────────────────────────────
// LRUCache
//
// A fixed-capacity cache mapping string keys to vector<string> values.
// Evicts the least-recently-used entry when capacity is exceeded.
// ─────────────────────────────────────────────────────────────────────────────
class LRUCache {
public:
    // Construct a cache with the given capacity (always >= 1).
    explicit LRUCache(int capacity);

    // get(key):
    //   HIT  → move entry to front, increment hit counter, return its value.
    //   MISS → increment miss counter, return empty vector.
    std::vector<std::string> get(const std::string& key);

    // put(key, value):
    //   If key exists: update value, move to front.
    //   If key is new: insert at front, evict back if size > capacity.
    void put(const std::string& key, const std::vector<std::string>& value);

    // hitRate():
    //   Returns hits / (hits + misses) * 100.0.
    //   Returns 0.0 if get() has never been called.
    double hitRate() const;

private:
    // TODO: int _capacity  — max number of entries
    // TODO: int _hits      — number of cache hits
    // TODO: int _misses    — number of cache misses
    //
    // TODO: std::list<std::pair<std::string, std::vector<std::string>>> _cache
    //         Front of the list = most recently used
    //         Back  of the list = least recently used
    //
    // TODO: std::unordered_map<std::string,
    //           std::list<std::pair<std::string,
    //                               std::vector<std::string>>>::iterator> _map
    //         Maps each key to its node's iterator in _cache.
    //         This is what makes get() and put() O(1).
    //
    // ⚠️  CAUTION: Iterators into a std::list remain valid as long as the
    //     node exists. Never copy the list — always use references.
    //     std::list::splice() is your friend for moving nodes around.
};

// ─────────────────────────────────────────────────────────────────────────────
// LRUCache::LRUCache
// ─────────────────────────────────────────────────────────────────────────────
LRUCache::LRUCache(int capacity) {
    // TODO: store capacity, zero-initialize hits and misses
}

// ─────────────────────────────────────────────────────────────────────────────
// LRUCache::get
// ─────────────────────────────────────────────────────────────────────────────
std::vector<std::string> LRUCache::get(const std::string& key) {
    // TODO: look up key in _map
    // TODO: HIT  — splice node to front, increment _hits, return value
    // TODO: MISS — increment _misses, return empty vector
    return {};
}

// ─────────────────────────────────────────────────────────────────────────────
// LRUCache::put
// ─────────────────────────────────────────────────────────────────────────────
void LRUCache::put(const std::string& key, const std::vector<std::string>& value) {
    // TODO: if key already in _map → update value, move to front, return early
    //
    // TODO: insert {key, value} at front of _cache
    // TODO: update _map[key] = _cache.begin()
    // TODO: if _cache.size() > _capacity:
    //           remove _map entry for the back key
    //           pop_back from _cache
}

// ─────────────────────────────────────────────────────────────────────────────
// LRUCache::hitRate
// ─────────────────────────────────────────────────────────────────────────────
double LRUCache::hitRate() const {
    // TODO: guard against division by zero
    // TODO: return (_hits / total_calls) * 100.0
    return 0.0;
}

// ─────────────────────────────────────────────────────────────────────────────
// loadDictionary and autocomplete
//
// When the unit tester includes both autocomplete.cpp and lru.cpp, these
// functions are already defined by autocomplete.cpp. The guard below prevents
// a redefinition error in that case.
// For the standalone `./lru` build (no prior include), AUTOCOMPLETE_INCLUDED
// is not defined and the function stubs compile normally.
// ─────────────────────────────────────────────────────────────────────────────
#ifndef AUTOCOMPLETE_INCLUDED
#define AUTOCOMPLETE_INCLUDED

void loadDictionary(const std::string& filename,
                    std::set<std::string>& words,
                    std::map<std::string, int>& freq) {
    // TODO: same implementation as in autocomplete.cpp
}

std::vector<std::string> autocomplete(const std::string& prefix,
                                       const std::set<std::string>& words,
                                       const std::map<std::string, int>& freq,
                                       int n) {
    // TODO: same implementation as in autocomplete.cpp
    return {};
}

#endif // AUTOCOMPLETE_INCLUDED

// ─────────────────────────────────────────────────────────────────────────────
// main — REPL with LRU caching + timing
// ─────────────────────────────────────────────────────────────────────────────
#ifndef TESTING
int main() {
    std::set<std::string> words;
    std::map<std::string, int> freq;

    // TODO: loadDictionary("data/words.txt", words, freq);
    //       print how many words loaded

    // TODO: const int TOP_N    = 5;
    // TODO: const int CAPACITY = 5;

    // TODO: construct LRUCache cache(CAPACITY);
    //       print "Cache capacity: N"

    std::string prefix;
    std::cout << "Enter prefix (or 'q' to quit): ";
    while (std::getline(std::cin, prefix)) {
        if (prefix == "q") break;

        // TODO: auto t_start = std::chrono::high_resolution_clock::now()
        //
        // TODO: result = cache.get(prefix)
        // TODO: if result is empty (MISS):
        //           result = autocomplete(prefix, words, freq, TOP_N)
        //           cache.put(prefix, result)
        //
        // TODO: auto t_end = std::chrono::high_resolution_clock::now()
        //       auto us = duration_cast<microseconds>(t_end - t_start).count()
        //
        // TODO: print "[HIT] X µs" or "[MISS] X µs" on one line
        //
        // TODO: print numbered results with frequencies
        //       e.g. "  1. present      (831240099)"

        std::cout << "\nEnter prefix (or 'q' to quit): ";
    }

    // TODO: print final hit rate: "Final hit rate: XX.XX%"
    std::cout << "Goodbye!\n";
    return 0;
}
#endif
