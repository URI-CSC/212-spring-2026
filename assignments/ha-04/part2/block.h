#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>
#include <stdexcept>

// BLOCK_SIZE controls how many integers each Block can hold.
// A small value makes it easy to observe block-boundary behavior during testing.
static const size_t BLOCK_SIZE = 4;

// ============================================================
// Block
// ============================================================
// A Block is a fixed-capacity array of integers that tracks a
// contiguous "live" window within it using a head and tail index.
//
// Invariant: 0 <= head <= tail <= BLOCK_SIZE
// ============================================================

class Block {
private:
    int    data[BLOCK_SIZE]; // the array of integers, note it is not a heap allocation
    size_t head;   // index of first live element  (inclusive)
    size_t tail;   // index past last live element (exclusive)

public:
    // Default constructor: empty block with head=0, tail=0.
    // Use this when the block will receive push_back calls.
    Block();

    // Constructor with explicit head and tail.
    // Use Block(BLOCK_SIZE, BLOCK_SIZE) to create a block ready for push_front.
    Block(size_t h, size_t t);

    // No destructor needed: data[] is stack-allocated inside the object.

    // Returns the number of elements in the live window.
    size_t size() const;

    // Returns true if the live window holds no elements.
    bool empty() const;

    // Returns true if there is room to push at the back.
    bool can_push_back() const;

    // Returns true if there is room to push at the front.
    bool can_push_front() const;

    // Appends val to the right end of the live window.
    // Requires: can_push_back() == true
    void push_back(int val);

    // Prepends val to the left end of the live window.
    // Requires: can_push_front() == true
    void push_front(int val);

    // Removes the rightmost element.
    // Requires: !empty()
    void pop_back();

    // Removes the leftmost element.
    // Requires: !empty()
    void pop_front();

    // Returns a reference to the i-th logical element (0 = front of live window).
    // Throws std::out_of_range if i >= size().
    int&       at(size_t i);
    const int& at(size_t i) const;

    // Returns a const reference to the front / back element without removing it.
    // Requires: !empty()
    const int& front() const;
    const int& back()  const;
};

#endif
