#include "block.h"

// ============================================================
// Block - Implementation
// ============================================================
// Invariant: 0 <= head <= tail <= BLOCK_SIZE
//   head - index of the first live element (inclusive)
//   tail - index one past the last live element (exclusive)
// ============================================================

// Default constructor: creates an empty block.
Block::Block() {
    head = 0;
    tail = 0;
}

// Parameterized constructor.
Block::Block(size_t h, size_t t) {
    head = h;
    tail = t;
}

// Returns the number of live elements.
size_t Block::size() const {
    // TODO: implement
    return 0;
}

// Returns true if there are no live elements.
bool Block::empty() const {
    // TODO: implement
    return true;
}

// Returns true if there is space at the right end of the window.
bool Block::can_push_back() const {
    // TODO: implement
    return false;
}

// Returns true if there is space at the left end of the window.
bool Block::can_push_front() const {
    // TODO: implement
    return false;
}

// Appends val to the back.
// Requires: can_push_back() == true
void Block::push_back(int val) {
    // TODO: store val and expand the live window to the right
}

// Prepends val to the front.
// Requires: can_push_front() == true
void Block::push_front(int val) {
    // TODO: expand the live window to the left and store val
}

// Removes the rightmost element.
// Requires: !empty()
void Block::pop_back() {
    // TODO: shrink the live window from the right
}

// Removes the leftmost element.
// Requires: !empty()
void Block::pop_front() {
    // TODO: shrink the live window from the left
}

// Returns a reference to the i-th live element (0 = front).
// Throws std::out_of_range if i >= size().
int& Block::at(size_t i) {
    // TODO: validate i, then return a reference to the correct element
    return data[0]; // placeholder
}

const int& Block::at(size_t i) const {
    // TODO: validate i, then return a const reference to the correct element
    return data[0]; // placeholder
}

// Returns a const reference to the first live element.
// Requires: !empty()
const int& Block::front() const {
    // TODO: return a reference to the first live element
    return data[0]; // placeholder
}

// Returns a const reference to the last live element.
// Requires: !empty()
const int& Block::back() const {
    // TODO: return a reference to the last live element
    return data[0]; // placeholder
}
