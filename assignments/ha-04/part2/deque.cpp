#include "deque.h"

// ============================================================
// Deque - Implementation
// ============================================================
// The Deque coordinates Block and BlockMap.
// All memory is managed by BlockMap; Deque tracks the total element count.
// ============================================================

// Creates an empty Deque.
Deque::Deque() : bmap(), total_size(0) {
    // Nothing extra needed: BlockMap default-constructs to empty.
}

// Returns the total number of elements.
size_t Deque::size() const {
    // TODO: implement
    (void)total_size; // suppress unused-field warning
    return 0; // placeholder
}

// Returns true if there are no elements.
bool Deque::empty() const {
    // TODO: implement
    return true;
}

// Appends val to the back.
void Deque::push_back(int val) {
    // TODO: allocate a new back Block if needed, then delegate to it
}

// Prepends val to the front.
void Deque::push_front(int val) {
    // TODO: allocate a new front Block if needed, then delegate to it
}

// Removes the back element.
// Throws std::out_of_range if empty.
void Deque::pop_back() {
    // TODO: throw if empty; remove the back element;
    //       discard the back Block if it becomes empty
}

// Removes the front element.
// Throws std::out_of_range if empty.
void Deque::pop_front() {
    // TODO: throw if empty; remove the front element;
    //       discard the front Block if it becomes empty
}

// Returns a const reference to the back element without removing it.
// Throws std::out_of_range if empty.
const int& Deque::back() const {
    // TODO: throw if empty; return the back element
    static int dummy = 0; return dummy; // placeholder
}

// Returns a const reference to the front element without removing it.
// Throws std::out_of_range if empty.
const int& Deque::front() const {
    // TODO: throw if empty; return the front element
    static int dummy = 0; return dummy; // placeholder
}

