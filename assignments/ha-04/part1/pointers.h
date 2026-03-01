#ifndef POINTERS_H
#define POINTERS_H

// ============================================================
// Assignment 04 - Part 1: Pointer Refresher
// pointers.h  --  provided, do not modify
// ============================================================

#include <cstddef>
#include <stdexcept>
#include <iostream>


// ============================================================
// Section 1 - Pointer Basics
// ============================================================

// Returns a pointer to x.
int* task1a_point_to(int& x);

// Writes the value 100 to the memory location p points to.
void task1b_modify(int* p);

// Prints every element of arr (length n) separated by spaces,
// using pointer arithmetic only — no [] subscript operator.
void task1c_print_array(int* arr, int n);


// ============================================================
// Section 2 - Jagged 2D Array
// ============================================================

// Allocates and returns a jagged 2D array with `rows` rows,
// where row i has sizes[i] elements.
int** task2a_allocate(int rows, const int* sizes);

// Sets every element so that grid[i][j] == j.
void task2b_fill(int** grid, int rows, const int* sizes);

// Frees all heap memory: each row first, then the outer array.
void task2c_free(int** grid, int rows);


// ============================================================
// Section 3 - IntArray (Rule of Three)
// ============================================================
// IntArray wraps a heap-allocated array of integers.
// Because it manages a raw resource, it requires the Rule of Three:
// a destructor, a copy constructor, and a copy assignment operator.

class IntArray {
private:
    int*   data;    // heap-allocated array
    size_t length;  // number of elements

public:
    IntArray(size_t n);
    ~IntArray();
    
    IntArray(const IntArray& other); // copy constructor
    IntArray& operator=(const IntArray& other); // copy assignment operator

    // Provided - returns a reference to element i (bounds-checked).
    int& at(size_t i) {
        if (i >= length) throw std::out_of_range("IntArray::at - index out of range");
        return data[i];
    }

    // Provided - returns the number of elements.
    size_t size() const { return length; }
};

#endif // POINTERS_H
