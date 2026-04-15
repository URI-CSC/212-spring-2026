// unittester.cpp
// Lab 09 — Autocomplete
// CSC 212 Data Structures
//
// Compile:
//   g++ -std=c++17 -Wall -Werror -DTESTING -o tests unittester.cpp
// Run:
//   ./tests
//
// The -DTESTING flag suppresses the main() functions in the source files
// included below, so this file can define its own.

// ── Pull in both implementations under test ───────────────────────────────
#include "autocomplete.cpp"
#include "lru.cpp"

// ── Minimal test framework ────────────────────────────────────────────────
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

static int tests_run     = 0;
static int tests_passed  = 0;
static int points_earned = 0;
static int points_total  = 0;

#define CHECK(name, pts, condition)                                          \
    do {                                                                     \
        ++tests_run;                                                         \
        points_total += (pts);                                               \
        if (condition) {                                                     \
            std::cout << "[PASS] " << (name)                                 \
                      << " (" << (pts) << "/" << (pts) << " pts)\n";        \
            ++tests_passed;                                                  \
            points_earned += (pts);                                          \
        } else {                                                             \
            std::cout << "[FAIL] " << (name)                                 \
                      << " (0/" << (pts) << " pts)\n";                       \
        }                                                                    \
    } while (false)

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

// Build a small in-memory word list without touching the filesystem.
// We write a temp file because loadDictionary takes a filename.
static std::string writeTempDict(const std::string& content) {
    const std::string path = "/tmp/csc212_lab09_test_dict.txt";
    std::ofstream f(path);
    f << content;
    return path;
}

// Returns true if `vec` contains `word`.
static bool contains(const std::vector<std::string>& vec,
                     const std::string& word) {
    for (const auto& s : vec) if (s == word) return true;
    return false;
}

// Returns true if vec[i] has frequency >= that of vec[i+1] for all i.
// (Checks that results are non-increasing in frequency.)
static bool isSortedByFreqDesc(const std::vector<std::string>& vec,
                                const std::map<std::string, int>& freq) {
    for (size_t i = 1; i < vec.size(); ++i) {
        int f_prev = freq.count(vec[i-1]) ? freq.at(vec[i-1]) : 0;
        int f_cur  = freq.count(vec[i])   ? freq.at(vec[i])   : 0;
        if (f_prev < f_cur) return false;
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Test suite — loadDictionary  (10 pts)
// ─────────────────────────────────────────────────────────────────────────────
void test_loadDictionary() {
    // Write a controlled dictionary with known content.
    std::string path = writeTempDict(
        "the 1000\n"
        "there 500\n"
        "their 250\n"
        "program 800\n"
        "prefix 300\n"
        "print 400\n"
        "xyz 1\n"
    );

    std::set<std::string> words;
    std::map<std::string, int> freq;
    loadDictionary(path, words, freq);

    CHECK("loadDictionary: file loads without error",  2, !words.empty());
    CHECK("loadDictionary: word count matches",        2, words.size() == 7);
    CHECK("loadDictionary: specific word present",     3, words.count("program") == 1);
    CHECK("loadDictionary: frequency correct",         3, freq.count("the") && freq.at("the") == 1000);
}

// ─────────────────────────────────────────────────────────────────────────────
// Test suite — autocomplete  (15 pts)
// ─────────────────────────────────────────────────────────────────────────────
void test_autocomplete() {
    std::string path = writeTempDict(
        "the 1000\n"
        "there 500\n"
        "their 250\n"
        "them 200\n"
        "program 800\n"
        "prefix 300\n"
        "print 400\n"
        "process 600\n"
        "alpha 100\n"
        "beta 50\n"
        "gamma 75\n"
    );

    std::set<std::string> words;
    std::map<std::string, int> freq;
    loadDictionary(path, words, freq);

    // ── empty prefix returns top-N ────────────────────────────────────────
    {
        auto res = autocomplete("", words, freq, 3);
        CHECK("autocomplete: empty prefix returns top-N",        3,
              res.size() == 3 && res[0] == "the");
    }

    // ── prefix "the" matches "the", "their", "them", "there" ─────────────
    {
        auto res = autocomplete("the", words, freq, 10);
        CHECK("autocomplete: prefix matches correct words",      3,
              contains(res, "the") &&
              contains(res, "there") &&
              contains(res, "their") &&
              contains(res, "them") &&
              !contains(res, "program"));
    }

    // ── results ordered by frequency descending ───────────────────────────
    {
        auto res = autocomplete("the", words, freq, 10);
        CHECK("autocomplete: results ordered by frequency",      4,
              isSortedByFreqDesc(res, freq));
    }

    // ── prefix with no matches returns empty ─────────────────────────────
    {
        auto res = autocomplete("zzz", words, freq, 5);
        CHECK("autocomplete: prefix with no matches returns empty", 2,
              res.empty());
    }

    // ── fewer than N matches returns all ─────────────────────────────────
    {
        // "alpha" is the only word starting with "al"
        auto res = autocomplete("al", words, freq, 10);
        CHECK("autocomplete: fewer than N matches returns all",  3,
              res.size() == 1 && res[0] == "alpha");
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Test suite — LRUCache  (15 pts)
// ─────────────────────────────────────────────────────────────────────────────
void test_lrucache() {
    // ── get on empty cache returns empty ─────────────────────────────────
    {
        LRUCache cache(3);
        auto result = cache.get("anything");
        CHECK("LRUCache: get on empty cache returns empty",      2,
              result.empty());
    }

    // ── put then get returns correct value ───────────────────────────────
    {
        LRUCache cache(3);
        std::vector<std::string> val = {"hello", "world"};
        cache.put("greeting", val);
        auto result = cache.get("greeting");
        CHECK("LRUCache: put then get returns correct value",    2,
              result == val);
    }

    // ── capacity eviction removes LRU ────────────────────────────────────
    // Insert A, B, C (capacity 3). Then insert D. A (LRU) should be evicted.
    {
        LRUCache cache(3);
        cache.put("a", {"a1"});
        cache.put("b", {"b1"});
        cache.put("c", {"c1"});
        cache.put("d", {"d1"});
        auto got_a = cache.get("a");
        auto got_d = cache.get("d");
        CHECK("LRUCache: capacity eviction removes LRU",         3,
              got_a.empty() && !got_d.empty());
    }

    // ── hit rate calculation ──────────────────────────────────────────────
    {
        LRUCache cache(5);
        cache.put("x", {"x1"});
        cache.get("x");  // hit
        cache.get("y");  // miss
        cache.get("x");  // hit
        // 2 hits, 1 miss → 2/3 * 100 = 66.66...
        double rate = cache.hitRate();
        CHECK("LRUCache: hit rate calculation",                  3,
              rate > 66.0 && rate < 67.0);
    }

    // ── move-to-front on hit updates LRU order ───────────────────────────
    // Insert A, B, C (cap 3). Access A → A moves to front, B becomes LRU.
    // Insert D → B should be evicted (not A).
    {
        LRUCache cache(3);
        cache.put("a", {"a1"});
        cache.put("b", {"b1"});
        cache.put("c", {"c1"});
        // Current order (front→back): c, b, a  (a was inserted first → LRU)
        cache.get("a");  // a moves to front → order: a, c, b
        // Insert d → b (now LRU) should be evicted
        cache.put("d", {"d1"});
        auto got_a = cache.get("a");  // should still be in cache
        auto got_b = cache.get("b");  // should be evicted
        CHECK("LRUCache: move-to-front on hit updates LRU order", 3,
              !got_a.empty() && got_b.empty());
    }

    // ── edge case: capacity = 1 ───────────────────────────────────────────
    {
        LRUCache cache(1);
        cache.put("first",  {"f"});
        cache.put("second", {"s"});
        // "first" should have been evicted.
        auto got_first  = cache.get("first");
        auto got_second = cache.get("second");
        CHECK("LRUCache: edge case capacity=1",                  2,
              got_first.empty() && !got_second.empty());
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "── loadDictionary tests ──────────────────\n";
    test_loadDictionary();

    std::cout << "\n── autocomplete tests ────────────────────\n";
    test_autocomplete();

    std::cout << "\n── LRUCache tests ────────────────────────\n";
    test_lrucache();

    std::cout << "\n─────────────────────────────────────────\n";
    std::cout << tests_passed << "/" << tests_run << " tests passed.\n";
    std::cout << "Score: " << points_earned << "/" << points_total << " pts\n";

    return (tests_passed == tests_run) ? 0 : 1;
}
