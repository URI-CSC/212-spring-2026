// ============================================================
// Assignment 04 - Part 1: Pointer Refresher  (30 pts)
// pointers.cpp  --  YOUR WORK GOES HERE
// ============================================================
// Compile and run with the autograder:
//   g++ -std=c++11 -Wall -Werror pointers.cpp test_pointers.cpp -o test
//   ./test
//
// Do NOT modify pointers.h or test_pointers.cpp.
// ============================================================

#include "pointers.h"
#include <iostream>


// ============================================================
// Section 1 - Pointer Basics  (5 pts)
// ============================================================

// Use the address-of operator to get a pointer to x and return it.
int* task1a_point_to(int& x) {
    // TODO 1a 
    return nullptr; // placeholder to allow compilation, must be modified
}

// Dereference p and assign 100 to the value at that address.
void task1b_modify(int* p) {
    // TODO 1b
}

// Starting from a pointer to arr[0], print each of the n elements
// separated by spaces. Do not use the [] subscript operator.
void task1c_print_array(int* arr, int n) {
    // TODO 1c
}


// ============================================================
// Section 2 - Jagged 2D Array  (10 pts)
// ============================================================

// Allocate int*[rows] for the outer array, then for each row i
// allocate int[sizes[i]]. Return the outer array.
int** task2a_allocate(int rows, const int* sizes) {
    // TODO 2a
    return nullptr; // placeholder to allow compilation, must be modified
}

// For every row i and column j (0 <= j < sizes[i]), set grid[i][j] = j.
void task2b_fill(int** grid, int rows, const int* sizes) {
    // TODO 2b
}

// delete[] each row, then delete[] the outer array.
void task2c_free(int** grid, int rows) {
    // TODO 2c
}


// ============================================================
// Section 3 - Rule of Three  (15 pts)
// ============================================================

// Allocate a heap array of n ints and zero-initialize every element.
IntArray::IntArray(size_t n) : data(new int[n]), length(n) {
    // TODO 3a: zero-initialize all n elements
}

// Release the heap array.
IntArray::~IntArray() {
    // TODO 3b
}

// Allocate a new array of the same length and copy every element from other.
IntArray::IntArray(const IntArray& other) : data(new int[other.length]), length(other.length) {
    // TODO 3c: copy all elements from other.data into this->data
}

// Guard against self-assignment. Free the old array, allocate a new one
// of other's length, copy every element, return *this.
IntArray& IntArray::operator=(const IntArray& other) {
    if (this == &other) return *this;
    // TODO 3d: free old array, then deep-copy length and all elements from other
    return *this;
}
