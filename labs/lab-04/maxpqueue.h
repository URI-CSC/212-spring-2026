#ifndef MAXPQUEUE_H
#define MAXPQUEUE_H

#include <string>
#include <vector>
#include <stdexcept>

// MaxPQueue: a max-priority queue of strings backed by a binary max-heap.
//
// The heap is stored in a 0-indexed array of fixed capacity:
//   root              -> data[0]
//   left child of i   -> data[2*i + 1]
//   right child of i  -> data[2*i + 2]
//   parent of i       -> data[(i - 1) / 2]
//
// The heap invariant holds at all times: every node is greater than or equal
// to both of its children, so data[0] always holds the maximum element.
class MaxPQueue {
private:
    std::string* data;   // 0-indexed heap array: active elements at data[0..sz-1]
    int capacity;        // maximum number of elements the queue can hold
    int sz;              // current number of elements in the queue

    // Moves the element at pos upward until the max-heap property is restored.
    // Called internally after inserting a new element at position data[sz-1].
    void upHeap(int pos);

    // Moves the element at pos downward until the max-heap property is restored.
    // Called internally after moving the last element to data[0] during pop,
    // and during the buildHeap constructor.
    void downHeap(int pos);

public:
    // Constructs an empty priority queue with the given capacity.
    // Throws std::invalid_argument if capacity <= 0.
    MaxPQueue(int capacity);

    // buildHeap constructor: builds a valid max-heap from all elements in O(n)
    // time.  The queue's capacity equals elements.size().
    // Throws std::invalid_argument if elements is empty.
    MaxPQueue(const std::vector<std::string>& elements);

    // Destructor: releases the dynamically allocated array.
    ~MaxPQueue();

    // Copy constructor: creates a deep copy of other.
    MaxPQueue(const MaxPQueue& other);

    // Copy assignment operator: replaces this queue's contents with a deep copy
    // of other.  Handles self-assignment correctly.
    MaxPQueue& operator=(const MaxPQueue& other);

    // Inserts s into the priority queue while preserving the heap invariant.
    // Throws std::overflow_error if the queue is already at capacity.
    void push(const std::string& s);

    // Removes the maximum element and restores the heap invariant.
    // Throws std::underflow_error if the queue is empty.
    void pop();

    // Returns a const reference to the maximum element (always at data[0]).
    // Throws std::underflow_error if the queue is empty.
    const std::string& top() const;

    // Returns the number of elements currently in the queue.
    int size() const;

    // Returns true if the queue contains no elements.
    bool empty() const;
};

#endif
