#ifndef DEQUE_H
#define DEQUE_H

#include "blockmap.h"

// ============================================================
// Deque
// ============================================================
// A double-ended queue of integers implemented as a segmented array.
//
// The Deque owns a BlockMap which in turn owns all Blocks.
// The Deque does not manage Block memory directly — it delegates
// all allocation and deallocation to the BlockMap.
// ============================================================

class Deque {
private:
    BlockMap bmap;        // owns all Blocks
    size_t   total_size;  // total number of elements across all Blocks

public:
    // Creates an empty Deque (no Blocks allocated yet).
    Deque();

    // The BlockMap destructor handles all cleanup; no extra work needed here.
    // When a Deque goes out of scope the compiler automatically calls ~BlockMap()
    // on the bmap member, which frees all Blocks and the slots array.

    // '= delete' makes copying a compile error (same rationale as BlockMap above).
    // Because Deque contains a BlockMap by value, copying a Deque would require
    // deep-copying every Block - a non-trivial operation not required here.
    // A real-world implementation would provide these via the Rule of Three.
    Deque(const Deque&)            = delete;
    Deque& operator=(const Deque&) = delete;

    // Returns the total number of elements.
    size_t size() const;

    // Returns true if the deque holds no elements.
    bool empty() const;

    // Appends val to the back.
    void push_back(int val);

    // Prepends val to the front.
    void push_front(int val);

    // Removes the back element.
    // Throws std::out_of_range if empty.
    void pop_back();

    // Removes the front element.
    // Throws std::out_of_range if empty.
    void pop_front();

    // Returns a const reference to the back element without removing it.
    // Throws std::out_of_range if empty.
    const int& back() const;

    // Returns a const reference to the front element without removing it.
    // Throws std::out_of_range if empty.
    const int& front() const;
};

#endif
