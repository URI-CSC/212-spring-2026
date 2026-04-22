// tester.cpp
// Lab 10 - HashSet Unit Tester
// CSC 212 Data Structures
//
// Compile:
//   g++ -std=c++17 -Wall -Werror -o tests tester.cpp
// Run:
//   ./tests
//
#include "hashset.hpp"

#include <iostream>
#include <string>

// Minimal test framework
static int tests_run     = 0;
static int tests_passed  = 0;
static int points_earned = 0;
static int points_total  = 0;

#define CHECK(name, pts, condition)                                           \
    do {                                                                      \
        ++tests_run;                                                          \
        points_total += (pts);                                                \
        if (condition) {                                                      \
            std::cout << "[PASS] " << (name)                                  \
                  << " (" << (pts) << "/" << (pts) << " pts)\n";              \
            ++tests_passed;                                                   \
            points_earned += (pts);                                           \
        } else {                                                              \
            std::cout << "[FAIL] " << (name)                                  \
                      << " (0/" << (pts) << " pts)\n";                        \
        }                                                                     \
    } while (false)

// ===========================================================================
// Section 1: insert and contains  (10 pts)
// Tests that inserted elements can be found, that duplicates are silently
// ignored, and that elements that were never inserted are not found.
void test_insert_and_contains() {
    HashSet<std::string> s(8);

    // Insert one element. contains() should return true for it immediately.
    s.insert("apple");
    CHECK("insert: single element is found", 2,
          false  // TODO: verify that "apple" is in the set
    );

    // Inserting the same value a second time must not increase the size.
    s.insert("apple");
    CHECK("insert: duplicate does not increase size", 2,
          false  // TODO: size() should still be 1
    );

    // A value that was never inserted must not be found.
    CHECK("contains: absent element returns false", 3,
          false  // TODO: verify that "banana" is NOT in the set
    );

    // Insert several distinct words; every one must be findable.
    s.insert("banana");
    s.insert("cherry");
    s.insert("date");
    CHECK("contains: multiple elements, all found", 3,
          false  // TODO: all three words are present in s
    );
}

// ===========================================================================
// Section 2: remove  (8 pts)
//
// Tests that removing a present element succeeds and actually removes it,
// and that removing an absent element returns false without side effects.
void test_remove() {
    HashSet<std::string> s(8);
    s.insert("alpha");
    s.insert("beta");

    // Removing a present element should return true.
    CHECK("remove: existing element returns true", 2,
          false  // TODO: s.remove("alpha") returns true
    );

    // After removal the element must no longer be found.
    CHECK("remove: element no longer found after remove", 3,
          false  // TODO: contains("alpha") is now false
    );

    // Removing an element that was never inserted must return false.
    CHECK("remove: absent element returns false", 3,
          false  // TODO: s.remove("gamma") returns false ("gamma" was never inserted)
    );
}

// ===========================================================================
// Section 3: size and empty  (6 pts)
//
// Tests that size() is zero on construction, tracks inserts and removes
// correctly (duplicates do not count), and that empty() reflects size() == 0.
void test_size_and_empty() {
    HashSet<int> s(4);

    // A freshly constructed set has no elements.
    CHECK("size: starts at zero", 2,
          false  // TODO: s.size() == 0
    );

    // Insert 10 and 20 (two distinct elements), then insert 10 again (duplicate),
    // then remove 20. Net result: one element remains.
    s.insert(10);
    s.insert(20);
    s.insert(10);   // duplicate - must not be counted
    s.remove(20);
    CHECK("size: tracks insertions and removals", 2,
          false  // TODO: s.size() == 1
    );

    // A set that has never had anything inserted reports empty() == true.
    HashSet<int> empty_set(4);
    CHECK("empty: returns true on fresh set", 2,
          false  // TODO: empty_set.empty() is true
    );
}

// ===========================================================================
// Section 4: load_factor  (6 pts)
//
// Tests that load_factor() returns size / bucket_count as a float,
// and that an empty set returns exactly 0.0f.
void test_load_factor() {
    // 4 elements in 8 buckets -> load factor should be exactly 0.5f.
    HashSet<int> s(8);
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    CHECK("load_factor: correct after insertions", 3,
          false  // TODO: s.load_factor() == 0.5f
    );

    // An empty set has load factor 0.0f.
    HashSet<int> empty_set(8);
    CHECK("load_factor: zero on empty set", 3,
          false  // TODO: empty_set.load_factor() == 0.0f
    );
}

// ===========================================================================
// Section 5: rehash  (6 pts)
//
// Tests that after rehash() all previously inserted elements are still present,
// and that bucket_count() reflects the new size.
void test_rehash() {
    HashSet<std::string> s(4);
    s.insert("one");
    s.insert("two");
    s.insert("three");

    // Grow the table from 4 buckets to 16. All elements must survive.
    s.rehash(16);
    CHECK("rehash: all elements survive", 3,
          false  // TODO: "one", "two", and "three" are all still in s
    );

    // bucket_count() must match the argument passed to rehash().
    CHECK("rehash: bucket count changes", 3,
          false  // TODO: s.bucket_count() == 16
    );
}

// ===========================================================================
// Section 6: edge cases  (4 pts)
//
// Stress-tests collision handling and the insert -> remove -> reinsert cycle.
void test_edge_cases() {
    // With only 4 buckets and 20 inserts, average chain length is 5.
    // Every element 0..19 must still be retrievable.
    HashSet<int> s(4);
    for (int i = 0; i < 20; ++i) s.insert(i);

    bool all_found = true;
    for (int i = 0; i < 20; ++i) {
        if (!s.contains(i)) { all_found = false; break; }
    }
    CHECK("edge: many elements, all retrievable", 2,
          false  // TODO: all_found is true
    );

    // Insert a value, remove it, then reinsert it.
    // The element must be present and size must be exactly 1.
    HashSet<std::string> s2(8);
    s2.insert("temp");
    s2.remove("temp");
    s2.insert("temp");
    CHECK("edge: insert-remove-reinsert", 2,
          false  // TODO: s2.contains("temp") && s2.size() == 1
    );
}

// ===========================================================================
// main
int main() {
    std::cout << "insert / contains tests\n";
    test_insert_and_contains();

    std::cout << "\nremove tests\n";
    test_remove();

    std::cout << "\nsize / empty tests\n";
    test_size_and_empty();

    std::cout << "\nload_factor tests\n";
    test_load_factor();

    std::cout << "\nrehash tests\n";
    test_rehash();

    std::cout << "\nedge case tests\n";
    test_edge_cases();

    std::cout << "\n";
    std::cout << tests_passed << "/" << tests_run << " tests passed.\n";
    std::cout << "Score: " << points_earned << "/" << points_total << " pts\n";

    return (tests_passed == tests_run) ? 0 : 1;
}
