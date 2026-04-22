// hashset.hpp
// Lab 10 - HashSet with Separate Chaining
// CSC 212 Data Structures
//
// ===========================================================================
// Do NOT change any declaration, signature, data member name, or #include.
// Write your implementation only inside the TODO method bodies below.
//
// Because HashSet is a template, the full implementation must live in this
// header. The compiler needs to see the complete definition when it instantiates
// HashSet<int>, HashSet<std::string>, etc.
//
// Compile the unit tester
//   g++ -std=c++17 -Wall -Werror -o tests tester.cpp
// ===========================================================================

#pragma once

#include <functional>   // std::hash<T>
#include <list>         // std::list  - one chain per bucket
#include <vector>       // std::vector - the bucket array

template <typename T>
class HashSet {
public:
    // Constructs a hash set with `num_buckets` empty chains.
    // Precondition:  num_buckets >= 1
    // Postcondition: bucket_count() == num_buckets, size() == 0
    HashSet(size_t num_buckets = 16);

    // Adds `value` to the set if it is not already present.
    // Precondition:  (none)
    // Postcondition: contains(value) == true
    //                size() increases by exactly 1 if value was not present;
    //                size() is unchanged if value was already in the set
    void insert(const T& value);

    // Checks if the set contains the value
    // Precondition:  (none)
    // Returns:       true  if value is in the set
    //                false otherwise
    // Does not modify the set.
    bool contains(const T& value) const;

    // Removes `value` from the set if it is present.
    // Precondition:  (none)
    // Returns:       true  if value was found and removed
    //                false if value was not in the set (no change)
    // Postcondition: contains(value) == false
    //                size() decreases by 1 only if value was found
    bool remove(const T& value);

    // Returns: the number of distinct elements currently in the set
    // Does not modify the set.
    size_t size() const;

    // Returns: true if the set contains no elements, false otherwise
    // Does not modify the set.
    bool empty() const;

    // Returns: the current load factor
    //          Measures average chain length across all buckets.
    //          Returns 0.0f when the set is empty.
    // Does not modify the set.
    float load_factor() const;

    // Returns: the current number of buckets in the table
    // Does not modify the set.
    size_t bucket_count() const;

    // Rebuilds the table with exactly `new_bucket_count` buckets.
    // Every element currently in the set is re-inserted under the new mapping.
    // Precondition:  new_bucket_count >= 1
    // Postcondition: bucket_count() == new_bucket_count
    //                all elements still present; size() unchanged
    void rehash(size_t new_bucket_count);

private:
    std::vector<std::list<T>> buckets;  // bucket array; buckets[i] is the chain for bucket i
    size_t                    n_elem;     // number of elements currently in the set

    // Maps any value to a valid index in [0, buckets.size()) [private helper]
    size_t _bucket_index(const T& value) const;
};

// ===========================================================================
// Implementation
// Template bodies must live in the header file.
// ===========================================================================

template <typename T>
HashSet<T>::HashSet(size_t num_buckets) {
    // TODO
}

template <typename T>
size_t HashSet<T>::_bucket_index(const T& value) const {
    // TODO
    return 0;
}

template <typename T>
void HashSet<T>::insert(const T& value) {
    // TODO
}

template <typename T>
bool HashSet<T>::contains(const T& value) const {
    // TODO
    return false;
}

template <typename T>
bool HashSet<T>::remove(const T& value) {
    // TODO
    return false;
}

template <typename T>
size_t HashSet<T>::size() const {
    // TODO
    return 0;
}

template <typename T>
bool HashSet<T>::empty() const {
    // TODO
    return true;
}

template <typename T>
float HashSet<T>::load_factor() const {
    // TODO
    return 0.0f;
}

template <typename T>
size_t HashSet<T>::bucket_count() const {
    // TODO
    return 0;
}

template <typename T>
void HashSet<T>::rehash(size_t new_bucket_count) {
    // TODO
}
