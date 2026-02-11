# Lab 03 - Dynamic Arrays in C++

## Overview

In this lab, you will design and implement a **dynamic array** (also known as a resizing or growing array) from scratch. A dynamic array is a fundamental data structure that combines the random-access efficiency of static arrays with the ability to grow as needed. Unlike fixed-size arrays, dynamic arrays automatically allocate more memory when they reach capacity, allowing them to store an arbitrary number of elements.

You will build the `DynArray` class incrementally, starting with basic operations like `push_back` and `at`, then adding insertion and removal methods, and finally implementing proper copy semantics following C++'s **Rule of Three**. By the end of this lab, you will have a deep understanding of how `std::vector` works under the hood and why careful memory management is essential for dynamic data structures.

All tasks are expected to be completed during the lab session. However, if you run out of time, please make every effort to complete them outside of lab hours.

> [!CAUTION]
> For this lab, **AVOID** using large language models (LLMs) to **generate code or solutions** for the exercises. In particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE while working on this lab. You may ask specific questions to LLMs about concepts or language syntax, but we strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> For all tasks below, you must read the specifications and all the code provided carefully. This lab is designed to make you think critically about the design and implementation of a dynamic array, in addition to learning best practices in C++.

## Task 1: Basic Dynamic Array
In this task, you will implement the core functionality of a dynamic array: storing elements, accessing them by index, and automatically growing when capacity is exceeded.

### The Growth Strategy

When a dynamic array runs out of space, it must allocate a larger block of memory. A common strategy is to **grow by a factor** (e.g., 1.5x or 2x) rather than by a fixed amount. This ensures that the *amortized* cost of `push_back` remains $O(1)$. For this lab, we use a default **initial capacity of 10** and a **growth factor of 1.5**.

### Specification

Create a file named `dynarray.h` with the following class declaration:
```cpp
#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <cstddef>

class DynArray {
private:
    int* data;              // Pointer to dynamically allocated array
    size_t size;            // Number of elements currently stored
    size_t capacity;        // Total capacity of allocated array
    double growth_factor;   // Factor by which to grow when resizing

public:
    // Default constructor: initializes empty array
    // Sets capacity to 10, size to 0, growth_factor to 1.5
    // Allocates memory for 'capacity' integers
    DynArray();

    // Destructor: deallocates the dynamic array
    ~DynArray();

    // Returns a reference to the element at the given index
    // Throws std::out_of_range if index >= size
    int& at(size_t index);

    // Const version of at() for read-only access
    const int& at(size_t index) const;

    // Appends a value to the end of the array
    // Grows the array if size == capacity
    void push_back(int value);
};

#endif
```

### Implementation

Create a file named `dynarray.cpp` and implement the member functions:
```cpp
#include "dynarray.h"
#include <iostream>
#include <stdexcept>

DynArray::DynArray() {
    // TODO: Set capacity to 10
    // TODO: Set size to 0
    // TODO: Set growth_factor to 1.5
    // TODO: Allocate memory for 'capacity' integers using new[]
}

DynArray::~DynArray() {
    // TODO: Deallocate the dynamic array using delete[]
}

int& DynArray::at(size_t index) {
    // TODO: Check if index >= size, throw std::out_of_range if so
    // TODO: Return reference to data[index]
}

const int& DynArray::at(size_t index) const {
    // TODO: Check if index >= size, throw std::out_of_range if so
    // TODO: Return const reference to data[index]
}

void DynArray::push_back(int value) {
    // TODO: If size == capacity, grow the array:
    //       1. Calculate new_capacity = (size_t)(capacity * growth_factor)
    //       2. Ensure new_capacity > capacity (add 1 if necessary)
    //       3. Allocate new array of new_capacity
    //       4. Copy all elements from old array to new array
    //       5. Delete old array
    //       6. Update data pointer and capacity
    // TODO: Store value at data[size]
    // TODO: Increment size
}
```

### Testing

Create a file named `main.cpp` to verify your implementation:
```cpp
#include "dynarray.h"
#include <cassert>
#include <iostream>

int main() {
    std::cout << "=== Task 1: Basic Dynamic Array ===" << std::endl;
    DynArray arr;
    for (int i = 0; i < 5; i++) {
        arr.push_back(i * 10);
    }
    assert(arr.at(0) == 0);
    assert(arr.at(1) == 10);
    assert(arr.at(2) == 20);
    assert(arr.at(3) == 30);
    assert(arr.at(4) == 40);
    std::cout << "Basic push_back and at: PASSED" << std::endl;
    // Test modification through at()
    arr.at(2) = 999;
    assert(arr.at(2) == 999);
    std::cout << "Modification through at(): PASSED" << std::endl;
    // Test growth: push more than initial capacity (10)
    DynArray arr2;
    for (int i = 0; i < 25; i++) {
        arr2.push_back(i);
    }
    for (int i = 0; i < 25; i++) {
        assert(arr2.at(i) == i);
    }
    std::cout << "Growth beyond initial capacity: PASSED" << std::endl;
    // Test out_of_range exception
    bool caught = false;
    try {
        arr.at(100);
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "Out of range exception: PASSED" << std::endl;
    std::cout << "\n>>> Task 1 Complete! <<<\n" << std::endl;
    return 0;
}
```

Compile and run:
```bash
$ g++ -std=c++11 -Wall -Werror dynarray.cpp main.cpp -o dynarray
$ ./dynarray
```

> [!NOTE]
> Observe that we use `int` for the stored values but `size_t` for anything related to size or indices. The type `size_t` is an unsigned integer type guaranteed to be large enough to represent the size of any object in memory. Using `size_t` for sizes and indices is a best practice that prevents negative index bugs and aligns with how the C++ Standard Library implements containers like `std::vector`.

## Task 2: Size, Capacity, and Insert

Now that the basic array works, you will add methods to query its state and insert elements at arbitrary positions. You will also **refactor** the growth logic into a private helper method.

### Specification

Add the following to `dynarray.h`:
```cpp
// Add to the private section:
private:
    // Grows the array by the growth factor
    // Called internally when more capacity is needed
    void grow();

// Add to the public section:
public:
    // Returns true if the array contains no elements
    bool empty() const;

    // Returns the number of elements in the array
    size_t get_size() const;

    // Returns the current capacity of the array
    size_t get_capacity() const;

    // Inserts a value at the specified index
    // All elements from index onward are shifted right
    // Throws std::out_of_range if index > size
    // Note: index == size is valid (equivalent to push_back)
    void insert(int value, size_t index);
```

### Implementation

Add the following to `dynarray.cpp`:
```cpp
void DynArray::grow() {
    // TODO: Calculate new_capacity = (size_t)(capacity * growth_factor)
    // TODO: Ensure new_capacity > capacity (add 1 if necessary)
    // TODO: Allocate new array of new_capacity
    // TODO: Copy all elements from old array to new array
    // TODO: Delete old array
    // TODO: Update data pointer and capacity
}

bool DynArray::empty() const {
    // TODO: Return true if size == 0, false otherwise
}

size_t DynArray::get_size() const {
    // TODO: Return size
}

size_t DynArray::get_capacity() const {
    // TODO: Return capacity
}

void DynArray::insert(int value, size_t index) {
    // TODO: Check if index > size, throw std::out_of_range if so
    // TODO: If size == capacity, call grow()
    // TODO: Shift elements from index to size-1 one position to the right
    //       (start from the end to avoid overwriting!)
    // TODO: Store value at data[index]
    // TODO: Increment size
}
```

**Refactor** your `push_back` method to use the new `grow()` helper:
```cpp
void DynArray::push_back(int value) {
    if (size == capacity) {
        grow();
    }
    data[size] = value;
    size++;
}
```

### Testing

Add the following tests to `main.cpp` (after Task 1 tests, before `return 0;`):
```cpp
    std::cout << "=== Task 2: Size, Capacity, and Insert ===" << std::endl;
    DynArray arr3;
    // Test empty
    assert(arr3.empty() == true);
    assert(arr3.get_size() == 0);
    assert(arr3.get_capacity() == 10);
    // Add some elements
    arr3.push_back(100);
    arr3.push_back(200);
    arr3.push_back(300);
    assert(arr3.empty() == false);
    assert(arr3.get_size() == 3);
    std::cout << "Empty array state: PASSED" << std::endl;
    // Test insert at beginning
    arr3.insert(50, 0);  // Array: [50, 100, 200, 300]
    assert(arr3.get_size() == 4);
    assert(arr3.at(0) == 50);
    assert(arr3.at(1) == 100);
    assert(arr3.at(2) == 200);
    assert(arr3.at(3) == 300);
    std::cout << "Insert at beginning: PASSED" << std::endl;
    // Test insert in middle
    arr3.insert(150, 2);  // Array: [50, 100, 150, 200, 300]
    assert(arr3.get_size() == 5);
    assert(arr3.at(0) == 50);
    assert(arr3.at(1) == 100);
    assert(arr3.at(2) == 150);
    assert(arr3.at(3) == 200);
    assert(arr3.at(4) == 300);
    std::cout << "Insert in middle: PASSED" << std::endl;
    // Test insert at end (index == size)
    arr3.insert(400, 5);  // Array: [50, 100, 150, 200, 300, 400]
    assert(arr3.get_size() == 6);
    assert(arr3.at(5) == 400);
    std::cout << "Insert at end: PASSED" << std::endl;
    // Test insert with growth
    DynArray arr4;
    for (int i = 0; i < 10; i++) {
        arr4.push_back(i);
    }
    assert(arr4.get_size() == 10);
    assert(arr4.get_capacity() == 10);  // At capacity
    arr4.insert(999, 5);  // Should trigger growth
    assert(arr4.get_size() == 11);
    assert(arr4.get_capacity() > 10);
    assert(arr4.at(5) == 999);
    assert(arr4.at(6) == 5);  // Old element shifted
    std::cout << "Insert with growth: PASSED (new capacity = " << arr4.get_capacity() << ")" << std::endl;
    // Test insert out of range
    caught = false;
    try {
        arr4.insert(0, 100);  // index > size
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "Insert out of range exception: PASSED" << std::endl;
    std::cout << "\n>>> Task 2 Complete! <<<\n" << std::endl;
```

Compile and run to verify all tests pass.

> [!NOTE]
> The `insert` method must shift elements starting from the **last element** and moving toward the insertion point. If you shift from the front, you will overwrite elements before copying them! This is a common bug. Consider the array `[A, B, C, D]` when inserting at index 1: you must first move D to position 4, then C to position 3, then B to position 2, and finally place the new element at position 1.

## Task 3: Push Front

Now that you have a working `insert` method, implementing `push_front` becomes trivial.

### Specification

Add the following to `dynarray.h`:
```cpp
    // Inserts a value at the beginning of the array
    // All existing elements are shifted right
    void push_front(int value);
```

### Implementation

Add the following to `dynarray.cpp`:
```cpp
void DynArray::push_front(int value) {
    // TODO: Call insert with index 0
    // Hint: This is a one-liner!
}
```

### Testing

Add the following tests to `main.cpp`:
```cpp
    std::cout << "=== Task 3: Push Front ===" << std::endl;
    DynArray arr5;
    arr5.push_front(30);  // [30]
    arr5.push_front(20);  // [20, 30]
    arr5.push_front(10);  // [10, 20, 30]
    assert(arr5.get_size() == 3);
    assert(arr5.at(0) == 10);
    assert(arr5.at(1) == 20);
    assert(arr5.at(2) == 30);
    std::cout << "push_front basic: PASSED" << std::endl;
    // Mix push_front and push_back
    arr5.push_back(40);   // [10, 20, 30, 40]
    arr5.push_front(5);   // [5, 10, 20, 30, 40]
    assert(arr5.get_size() == 5);
    assert(arr5.at(0) == 5);
    assert(arr5.at(4) == 40);
    std::cout << "Mixed push_front and push_back: PASSED" << std::endl;
    // push_front with growth
    DynArray arr6;
    for (int i = 10; i >= 1; i--) {
        arr6.push_front(i);
    }
    // Array should be [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    assert(arr6.get_size() == 10);
    for (int i = 0; i < 10; i++) {
        assert(arr6.at(i) == i + 1);
    }
    arr6.push_front(0);  // Should trigger growth
    assert(arr6.get_size() == 11);
    assert(arr6.at(0) == 0);
    assert(arr6.at(10) == 10);
    std::cout << "push_front with growth: PASSED" << std::endl;
    std::cout << "\n>>> Task 3 Complete! <<<\n" << std::endl;
```

## Task 4: Erase Operations

Now you will implement methods to remove elements from the array. Unlike growing, shrinking does not require reallocation. We simply update the size and shift elements as needed.

### Specification

Add the following to `dynarray.h`:
```cpp
    // "Removes" all elements from the array
    // Sets size to 0 but does NOT deallocate memory
    // Capacity remains unchanged
    void clear();

    // "Removes" the last element from the array
    // Throws std::out_of_range if array is empty
    void pop_back();

    // "Removes" the first element from the array
    // All remaining elements are shifted left
    // Throws std::out_of_range if array is empty
    void pop_front();

    // "Removes" the element at the specified index
    // All elements after index are shifted left
    // Throws std::out_of_range if index >= size
    void erase(size_t index);
```

### Implementation

Add the following to `dynarray.cpp`:
```cpp
void DynArray::clear() {
    // TODO: Set size to 0
    // Note: We do NOT delete[] and reallocate. The capacity remains
    // available for future insertions. This is an intentional design
    // choice that avoids unnecessary allocations.
}

void DynArray::pop_back() {
    // TODO: Check if size == 0, throw std::out_of_range if so
    // TODO: Decrement size
    // Note: We don't need to "erase" the old value; it will be
    // overwritten when a new element is pushed.
}

void DynArray::erase(size_t index) {
    // TODO: Check if index >= size, throw std::out_of_range if so
    // TODO: Shift all elements from index+1 to size-1 one position left
    // TODO: Decrement size
}

void DynArray::pop_front() {
    // TODO: Call erase with index 0
    // Hint: This is a one-liner!
}
```

### Testing

Add the following tests to `main.cpp`:
```cpp
    std::cout << "=== Task 4: Erase Operations ===" << std::endl;
    DynArray arr7;
    for (int i = 1; i <= 5; i++) {
        arr7.push_back(i * 10);
    }
    // Array: [10, 20, 30, 40, 50]
    size_t old_capacity = arr7.get_capacity();
    // Test pop_back
    arr7.pop_back();  // [10, 20, 30, 40]
    assert(arr7.get_size() == 4);
    assert(arr7.at(3) == 40);
    std::cout << "pop_back: PASSED" << std::endl;
    // Test pop_front
    arr7.pop_front();  // [20, 30, 40]
    assert(arr7.get_size() == 3);
    assert(arr7.at(0) == 20);
    assert(arr7.at(2) == 40);
    std::cout << "pop_front: PASSED" << std::endl;
    // Test erase middle
    arr7.push_back(50);
    arr7.push_back(60);  // [20, 30, 40, 50, 60]
    arr7.erase(2);       // [20, 30, 50, 60]
    assert(arr7.get_size() == 4);
    assert(arr7.at(0) == 20);
    assert(arr7.at(1) == 30);
    assert(arr7.at(2) == 50);
    assert(arr7.at(3) == 60);
    std::cout << "erase middle: PASSED" << std::endl;
    // Test erase first (same as pop_front)
    arr7.erase(0);  // [30, 50, 60]
    assert(arr7.get_size() == 3);
    assert(arr7.at(0) == 30);
    std::cout << "erase first: PASSED" << std::endl;
    // Test erase last (same as pop_back)
    arr7.erase(arr7.get_size() - 1);  // [30, 50]
    assert(arr7.get_size() == 2);
    assert(arr7.at(1) == 50);
    std::cout << "erase last: PASSED" << std::endl;
    // Test clear
    arr7.clear();
    assert(arr7.get_size() == 0);
    assert(arr7.empty() == true);
    assert(arr7.get_capacity() == old_capacity);  // Capacity unchanged!
    std::cout << "clear (capacity preserved): PASSED" << std::endl;
    // Test that array is still usable after clear
    arr7.push_back(999);
    assert(arr7.get_size() == 1);
    assert(arr7.at(0) == 999);
    std::cout << "Reuse after clear: PASSED" << std::endl;
    // Test exceptions on empty array
    DynArray arr8;
    caught = false;
    try {
        arr8.pop_back();
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "pop_back on empty: exception PASSED" << std::endl;
    caught = false;
    try {
        arr8.pop_front();
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "pop_front on empty: exception PASSED" << std::endl;
    caught = false;
    try {
        arr8.erase(0);
    } catch (const std::out_of_range& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "erase on empty: exception PASSED" << std::endl;
    std::cout << "\n>>> Task 4 Complete! <<<\n" << std::endl;
```

## Task 5: Constructors and the Rule of Three

Your `DynArray` class currently has only a default constructor. In this task, you will add constructors with parameters and, critically, implement proper **copy semantics** by following the **Rule of Three**.

### Additional Constructors

It is often useful to create a dynamic array with a specific initial capacity or growth factor.

### The Copy Constructor

A **copy constructor** is a special constructor that creates a new object as a copy of an existing object. It is invoked in the following situations:

1. When an object is initialized from another object of the same type: `DynArray b = a;` or `DynArray b(a);`
2. When an object is passed **by value** to a function: `void foo(DynArray arr)`
3. When an object is returned **by value** from a function: `DynArray bar() { DynArray x; return x; }`

If you do not define a copy constructor, the compiler generates a default one that performs a **shallow copy** (it simply copies each member variable). For `DynArray`, this means both the original and the copy would point to the **same** underlying array! Modifying one would affect the other, and when both destructors run, they would attempt to `delete[]` the same memory, causing undefined behavior (typically a crash).

### The Rule of Three

The **Rule of Three** states that if a class requires a user-defined **destructor**, **copy constructor**, or **copy assignment operator**, it almost certainly requires all three. This rule applies to any class that manages a resource (like dynamically allocated memory).

For `DynArray`:
- **Destructor**: Needed to `delete[]` the dynamic array.
- **Copy Constructor**: Needed to perform a **deep copy** (allocate new memory and copy elements).
- **Copy Assignment Operator**: Needed to handle assignment between existing objects (e.g., `a = b;`), which requires cleaning up the old data before copying.

### Specification

Add the following to `dynarray.h`:
```cpp
    // Constructor with initial capacity
    // Sets capacity to initial_capacity, size to 0, growth_factor to 1.5
    // Throws std::invalid_argument if initial_capacity is 0
    explicit DynArray(size_t initial_capacity);

    // Constructor with initial capacity and growth factor
    // Throws std::invalid_argument if initial_capacity is 0 or growth_factor <= 1.0
    DynArray(size_t initial_capacity, double growth_factor);

    // Copy constructor: creates a deep copy of another DynArray
    DynArray(const DynArray& other);

    // Copy assignment operator: assigns contents of another DynArray
    // Must handle self-assignment correctly
    DynArray& operator=(const DynArray& other);
```

> [!NOTE]
> The `explicit` keyword on the single-parameter constructor prevents implicit conversions from `size_t` to `DynArray`. Without it, code like `DynArray arr = 10;` would compile (creating an array with capacity 10), which is confusing. The `explicit` keyword requires you to write `DynArray arr(10);` instead, making the intent clear.

### Implementation

Add the following to `dynarray.cpp`:
```cpp
DynArray::DynArray(size_t initial_capacity) {
    // TODO: Check if initial_capacity == 0, throw std::invalid_argument if so
    // TODO: Set capacity to initial_capacity
    // TODO: Set size to 0
    // TODO: Set growth_factor to 1.5
    // TODO: Allocate memory for 'capacity' integers
}

DynArray::DynArray(size_t initial_capacity, double gf) {
    // TODO: Check if initial_capacity == 0, throw std::invalid_argument if so
    // TODO: Check if gf <= 1.0, throw std::invalid_argument if so
    // TODO: Set capacity to initial_capacity
    // TODO: Set size to 0
    // TODO: Set growth_factor to gf
    // TODO: Allocate memory for 'capacity' integers
}

DynArray::DynArray(const DynArray& other) {
    // TODO: Copy size, capacity, and growth_factor from other
    // TODO: Allocate NEW memory for 'capacity' integers
    // TODO: Copy all elements from other.data to this->data
    // IMPORTANT: This must be a DEEP copy - do NOT just copy the pointer!
}

DynArray& DynArray::operator=(const DynArray& other) {
    // TODO: Check for self-assignment (if this == &other, return *this)
    // TODO: Delete the existing data array
    // TODO: Copy size, capacity, and growth_factor from other
    // TODO: Allocate NEW memory for 'capacity' integers
    // TODO: Copy all elements from other.data to this->data
    // TODO: Return *this
}
```

> [!WARNING]
> The copy assignment operator **MUST** check for self-assignment (`arr = arr;`). If you skip this check and delete the data first, you will destroy the source data before copying it, resulting in undefined behavior. Always include `if (this == &other) return *this;` at the beginning.

### Testing

Add the following tests to `main.cpp`:
```cpp
    std::cout << "=== Task 5: Constructors and Rule of Three ===" << std::endl;
    // Test constructor with initial capacity
    DynArray arr9(20);
    assert(arr9.get_capacity() == 20);
    assert(arr9.get_size() == 0);
    assert(arr9.empty() == true);
    std::cout << "Constructor with capacity: PASSED" << std::endl;
    // Test constructor with capacity and growth factor
    DynArray arr10(5, 2.0);
    assert(arr10.get_capacity() == 5);
    for (int i = 0; i < 6; i++) {
        arr10.push_back(i);
    }
    assert(arr10.get_capacity() == 10);  // 5 * 2.0 = 10
    std::cout << "Constructor with capacity and growth factor: PASSED" << std::endl;
    // Test invalid arguments
    caught = false;
    try {
        DynArray bad1(0);  // Invalid: zero capacity
    } catch (const std::invalid_argument& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "Zero capacity rejected: PASSED" << std::endl;
    caught = false;
    try {
        DynArray bad2(10, 1.0);  // Invalid: growth factor must be > 1
    } catch (const std::invalid_argument& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "Invalid growth factor rejected: PASSED" << std::endl;
    caught = false;
    try {
        DynArray bad3(10, 0.5);  // Invalid: growth factor <= 1
    } catch (const std::invalid_argument& e) {
        caught = true;
    }
    assert(caught);
    std::cout << "Growth factor <= 1 rejected: PASSED" << std::endl;
    // Test copy constructor
    DynArray original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);
    DynArray copy1(original);  // Copy constructor
    assert(copy1.get_size() == 3);
    assert(copy1.at(0) == 10);
    assert(copy1.at(1) == 20);
    assert(copy1.at(2) == 30);
    std::cout << "Copy constructor basic: PASSED" << std::endl;
    // Verify deep copy: modifying copy doesn't affect original
    copy1.at(0) = 999;
    copy1.push_back(40);
    assert(original.at(0) == 10);  // Original unchanged
    assert(original.get_size() == 3);  // Original size unchanged
    std::cout << "Copy constructor deep copy: PASSED" << std::endl;
    // Test copy assignment operator
    DynArray arr11;
    arr11.push_back(100);
    arr11.push_back(200);
    DynArray arr12;
    arr12.push_back(1);  // arr12 has existing data
    arr12 = arr11;  // Copy assignment
    assert(arr12.get_size() == 2);
    assert(arr12.at(0) == 100);
    assert(arr12.at(1) == 200);
    std::cout << "Copy assignment basic: PASSED" << std::endl;
    // Verify deep copy: modifying arr12 doesn't affect arr11
    arr12.at(0) = 555;
    assert(arr11.at(0) == 100);  // arr11 unchanged
    std::cout << "Copy assignment deep copy: PASSED" << std::endl;
    // Test self-assignment
    DynArray& ref = arr11;
    arr11 = ref; // Should not crash or corrupt data
    assert(arr11.get_size() == 2);
    assert(arr11.at(0) == 100);
    assert(arr11.at(1) == 200);
    std::cout << "Self-assignment handled: PASSED" << std::endl;
    // Test chained assignment
    DynArray a, b, c;
    a.push_back(7);
    c = b = a;
    assert(b.at(0) == 7);
    assert(c.at(0) == 7);
    b.at(0) = 8;
    assert(a.at(0) == 7);  // a unchanged
    assert(c.at(0) == 7);  // c unchanged
    std::cout << "Chained assignment: PASSED" << std::endl;
    std::cout << "\n>>> Task 5 Complete! <<<" << std::endl;
    std::cout << "\n========================================" << std::endl;
    std::cout << "      ALL TESTS PASSED!" << std::endl;
    std::cout << "========================================" << std::endl;
```

## Summary

By completing this lab, you have implemented a fully functional dynamic array with the following features:

| Operation | Time Complexity | Notes |
|-----------|-----------------|-------|
| `at(index)` | $O(1)$ | Random access |
| `push_back(value)` | $O(1)$ amortized | $O(n)$ when growth occurs |
| `push_front(value)` | $O(n)$ | Must shift all elements |
| `insert(value, index)` | $O(n)$ | Must shift elements after index |
| `pop_back()` | $O(1)$ | No shifting needed |
| `pop_front()` | $O(n)$ | Must shift all elements |
| `erase(index)` | $O(n)$ | Must shift elements after index |
| `clear()` | $O(1)$ | Just resets size |

You have also learned about:
- **The Rule of Three**: destructor, copy constructor, and copy assignment operator
- **Deep vs. shallow copying** and why it matters for resource-managing classes

## Submission

After you have completed all the tasks, submit the files below to Gradescope. Your code should be well-formatted and easy to read. Make sure to test your code thoroughly before submitting it. Full credit for this lab requires **attendance**, **collaboration with your group**, and **active participation**. No remote submissions will be accepted. If you have any questions, please ask the instructor or the TA for help.

- `dynarray.h`
- `dynarray.cpp`
- `main.cpp`

