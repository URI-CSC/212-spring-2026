#include "linkedlist.h"

// ---------------------------------------------------------------------------
// PROVIDED -- do not modify these methods.
// ---------------------------------------------------------------------------

int& SLList::front() {
    if (!head) throw std::underflow_error("List is empty");
    return head->data;
}

int& SLList::back() {
    if (!tail) throw std::underflow_error("List is empty");
    return tail->data;
}

void SLList::push_front(const int& value) {
    Node* p = new Node(value);
    if (!head) {
        head = p;
        tail = head;
    } else {
        p->next = head;
        head = p;
    }
    size_++;
}

void SLList::push_back(const int& value) {
    if (size_ == 0) {
        head = new Node(value);
        tail = head;
    } else {
        Node* temp = new Node(value);
        tail->next = temp;
        tail = temp;
    }
    size_++;
}

void SLList::pop_back() {
    if (!head) throw std::underflow_error("List is empty");
    if (head == tail) {        // single element: both pointers must become nullptr
        delete tail;
        head = nullptr;
        tail = nullptr;
    } else {                   // two or more elements: walk to the second-to-last node
        Node* q = head;
        while (q->next != tail) {
            q = q->next;
        }
        delete tail;
        tail = q;
        tail->next = nullptr;
    }
    size_--;
}

void SLList::print() {
    Node* p = head;
    while (p != nullptr) {
        std::cout << p->data << " ";
        p = p->next;
    }
    std::cout << "| [" << size_ << "]\n";
}

// ---------------------------------------------------------------------------
// Task 1 -- implement the two methods below.
// ---------------------------------------------------------------------------

void SLList::clear() {
    // TODO: Walk the list from head to tail, deleting each node one at a time.
    //       When finished, reset head and tail to nullptr and size_ to 0.
    //
    // IMPORTANT: save a pointer to the *next* node BEFORE deleting the current
    //            one -- after delete, the memory is gone and p->next is invalid.
    //
    //   Suggested skeleton:
    //
    //       Node* p = head;
    //       while (p != nullptr) {
    //           Node* next = p->next;   // save next BEFORE deleting p
    //           delete p;
    //           p = next;
    //       }
    //       head   = nullptr;
    //       tail   = nullptr;
    //       size_  = 0;
}

void SLList::pop_front() {
    // TODO: Remove the first node from the list and free its memory.
    //
    //  1. Throw std::underflow_error("List is empty") if the list is empty.
    //  2. Handle the single-element case (head == tail):
    //       delete the node, then set BOTH head and tail to nullptr.
    //  3. Handle the general case (two or more elements):
    //       advance head to head->next, then delete the old front node.
    //  4. Decrement size_ in every non-throwing path.
    //
    // Hint: look at how pop_back() handles the single-element case -- the
    //       logic here is symmetric but simpler because no traversal is needed.
}
