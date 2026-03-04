#include "maxpqueue.h"
#include <algorithm>   // std::swap
#include <stdexcept>

// ---------------------------------------------------------------------------
// PROVIDED -- do not modify upHeap or downHeap.
// ---------------------------------------------------------------------------

void MaxPQueue::upHeap(int pos) {
    // Repeatedly compare the element at pos with its parent at (pos-1)/2.
    // If it is greater, swap them and move up; stop when the heap property holds.
    while (pos > 0 && data[pos] > data[(pos - 1) / 2]) {
        std::swap(data[pos], data[(pos - 1) / 2]);
        pos = (pos - 1) / 2;
    }
}

void MaxPQueue::downHeap(int pos) {
    // Repeatedly find the larger child of pos (left: 2*pos+1, right: 2*pos+2).
    // If that child is greater, swap and move down; stop when the property holds.
    while (2 * pos + 1 < sz) {
        int child = 2 * pos + 1;                          // left child
        if (child + 1 < sz && data[child + 1] > data[child]) {
            child++;                                      // right child is larger
        }
        if (data[pos] >= data[child]) break;              // heap property satisfied
        std::swap(data[pos], data[child]);
        pos = child;
    }
}

// ---------------------------------------------------------------------------
// PROVIDED -- constructor, destructor, top, size, and empty.
// ---------------------------------------------------------------------------

MaxPQueue::MaxPQueue(int cap) {
    if (cap <= 0)
        throw std::invalid_argument("capacity must be positive");
    capacity = cap;
    sz       = 0;
    data     = new std::string[capacity];
}

MaxPQueue::~MaxPQueue() {
    delete[] data;
}

const std::string& MaxPQueue::top() const {
    if (sz == 0)
        throw std::underflow_error("priority queue is empty");
    return data[0];
}

int MaxPQueue::size() const {
    return sz;
}

bool MaxPQueue::empty() const {
    return sz == 0;
}

// ---------------------------------------------------------------------------
// Task 1 -- implement the four methods below.
// ---------------------------------------------------------------------------

MaxPQueue::MaxPQueue(const MaxPQueue& other) {
    // TODO: create a new queue that is an independent copy of other.
}

MaxPQueue& MaxPQueue::operator=(const MaxPQueue& other) {
    // TODO: replace this queue's contents with an independent copy of other.
    //       guard against self-assignment.
    return *this;  // placeholder -- replace with your implementation
}

void MaxPQueue::push(const std::string& s) {
    // TODO: insert s while preserving the max-heap property.
    //       throw if the queue is already at capacity.
}

void MaxPQueue::pop() {
    // TODO: remove the maximum element and restore the heap property.
    //       throw if the queue is empty.
}

// ---------------------------------------------------------------------------
// Task 2 -- replace the stub below with your buildHeap implementation.
// ---------------------------------------------------------------------------

MaxPQueue::MaxPQueue(const std::vector<std::string>& elements) {
    // TODO: build a valid max-heap from elements in O(n) time.
    //       throw if elements is empty.
    //
    // The three lines below are a temporary stub -- they keep the file compiling
    // so that Task 1 tests run first.  Replace this entire body in Task 2.
    (void)elements; capacity = 0; sz = 0; data = nullptr;
    throw std::logic_error("buildHeap constructor not yet implemented (Task 2)");
}
