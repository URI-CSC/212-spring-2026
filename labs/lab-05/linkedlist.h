#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <stdexcept>

// SLList: a singly-linked list of ints.
//
// The list maintains a head pointer (front), a tail pointer (back), and a
// size counter.  Most operations run in O(1); clear() and print() traverse
// the entire list and run in O(n).
//
//   head                              tail
//     │                                │
//     ▼                                ▼
//  ┌──────┬───┐   ┌──────┬───┐   ┌──────┬──────┐
//  │  10  │ ●─┼──▶│  20  │ ●─┼──▶│  30  │ null │
//  └──────┴───┘   └──────┴───┘   └──────┴──────┘
//
class SLList {
private:
    struct Node {
        int   data;
        Node* next;

        Node(const int& value) : data(value), next(nullptr) {}
    };

    Node*  head;   // pointer to the first node (nullptr if the list is empty)
    Node*  tail;   // pointer to the last  node (nullptr if the list is empty)
    size_t size_;  // number of elements currently in the list

public:
    // Constructs an empty list.
    SLList() : head(nullptr), tail(nullptr), size_(0) {}

    // Destructor: calls clear() to release all dynamically allocated nodes.
    ~SLList() { clear(); }

    // Returns the number of elements in the list.
    size_t size() { return size_; }

    // Returns true if the list contains no elements.
    bool empty() { return size_ == 0; }

    // Removes all elements from the list and frees their memory.
    // After this call: head == nullptr, tail == nullptr, size_ == 0.
    void clear();

    // Returns a reference to the first element.
    // Throws std::underflow_error if the list is empty.
    int& front();

    // Returns a reference to the last element.
    // Throws std::underflow_error if the list is empty.
    int& back();

    // Inserts value at the front of the list in O(1).
    void push_front(const int& value);

    // Removes the front element in O(1).
    // Throws std::underflow_error if the list is empty.
    void pop_front();

    // Inserts value at the back of the list in O(1).
    void push_back(const int& value);

    // Removes the back element in O(n) — must walk to find the new tail.
    // Throws std::underflow_error if the list is empty.
    void pop_back();

    // Prints every element followed by the list size.
    // Example:  10 20 30 | [3]
    void print();
};

#endif
