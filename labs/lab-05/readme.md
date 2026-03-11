# Lab 05 - Singly-Linked List & Templates

## Overview

In this lab you will implement key operations on a **singly-linked list** and then explore C++ **templates** to make that list generic. The lab has two parts:

- **Part 1** - Complete two missing methods in a split header/source implementation, then validate with a provided test suite.
- **Part 2** - Learn what template functions and template classes are, practice with short examples, then convert your linked list into a generic `SLList<T>` that works with any type.

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. In particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE while working on this lab. You may ask specific questions to LLMs about concepts or language syntax, but we strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> Read all provided code and specifications carefully before writing a single line. This lab is designed to sharpen your understanding of pointers, dynamic memory, and generic programming in C++.

---

## Part 1 - Task 1: SLList

### The Data Structure

A **singly-linked list** is a chain of `Node` objects. Each node holds a value and a single pointer to the next node in the chain. Two bookkeeping pointers track the endpoints: `head` always points to the **front** of the list; `tail` always points to the **back**. When the list is empty, both are `nullptr`.

### Files

| File | Role | Status |
|------|------|--------|
| `linkedlist.h` | Complete class declaration | Do not modify |
| `linkedlist.cpp` | Mostly implemented; two methods are stubs | **Your work** |
| `testlinkedlist.cpp` | Automated test suite | Do not modify |

### Interface

| Method | Description | Status |
|--------|-------------|--------|
| `SLList()` | Constructs an empty list | Provided |
| `~SLList()` | Calls `clear()` to free all nodes | Provided |
| `size()` | Returns the number of elements | Provided |
| `empty()` | Returns `true` if the list has no elements | Provided |
| `front()` | Returns a reference to the first element | Provided |
| `back()` | Returns a reference to the last element | Provided |
| `push_front(value)` | Inserts `value` at the front in O(1) | Provided |
| `push_back(value)` | Inserts `value` at the back in O(1) | Provided |
| `pop_back()` | Removes the back element in O(n) | Provided |
| `print()` | Prints all elements and the size | Provided |
| `pop_front()` | Removes the front element in O(1) | **TODO** |
| `clear()` | Removes and frees every node | **TODO** |

### What to Implement

Open `linkedlist.cpp`. Two method stubs near the bottom are marked `TODO`. Read the comments inside each stub carefully before writing any code.

- **`clear()`**: Walk the list from `head` to `tail`, `delete`-ing each node. After all nodes are freed, reset `head`, `tail`, and `size_` to their empty-list values. The destructor calls `clear()`, so a correct implementation also prevents memory leaks.

- **`pop_front()`**: Remove the first node. Throw `std::underflow_error` if the list is empty. Handle the single-element case (`head == tail`) separately, look at how `pop_back()` does it for reference. No traversal is needed here; `head` already points directly at the node to remove.

### Testing

Once you have implemented both methods, compile and run the test suite:

```bash
$ g++ -std=c++11 -Wall -Werror linkedlist.cpp testlinkedlist.cpp -o testlinkedlist
$ ./testlinkedlist
```

A fully correct implementation prints `10/10 points`. The test runner will crash with an assertion error at the first failing check, so if it aborts partway through you can identify which method still has a bug.

> [!WARNING]
> If `clear()` does not set `head` and `tail` back to `nullptr` after freeing nodes, the next `push_back` or `push_front` call on a freshly-cleared list will corrupt the list. The test suite checks this: it calls `clear()` and then pushes new elements to make sure the list still works correctly.

---

## Part 2 - Task 2: Templates

So far, `SLList` only stores `int` values. What if you need a list of `double`s? Or `std::string`s? You could copy-paste the whole class and substitute the type but that duplicates code and is a maintenance nightmare. C++ solves this with **templates**, which let you write a single class (or function) that works for *any* type.

### Template Functions

A **template function** is a function where one or more types are left as a named placeholder, the *type parameter*, filled in automatically by the compiler at each call site.

```cpp
#include <iostream>

template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    std::cout << maximum(3, 7)        << "\n";  // T = int    -> prints 7
    std::cout << maximum(3.14, 2.72)  << "\n";  // T = double -> prints 3.14
    std::cout << maximum('a', 'z')    << "\n";  // T = char   -> prints z
}
```

The compiler generates three separate functions, one for `int`, one for `double`, one for `char`, from a single piece of source code. A few things to notice:

- `template <typename T>` must appear immediately before every function that uses the placeholder `T`.
- The compiler **deduces** `T` from the argument types automatically, so you do not need to write `maximum<int>(3, 7)` (though you may do so explicitly if you wish).
- `T` must support every operation used inside the function. `maximum` uses `>`, so any type passed as `T` must have `operator>` defined.

### Templated Classes

A **templated class** works the same way, but the placeholder `T` is shared across all member functions and the private data. Here is a minimal `Box<T>` that stores one value of any type:

```cpp
#include <iostream>
#include <string>

template <typename T>
class Box {
private:
    T contents;
public:
    Box(T value) : contents(value) {}
    T    get()         const { return contents; }
    void set(T value)        { contents = value; }
    void print()       const { std::cout << "Box: " << contents << "\n"; }
};

int main() {
    Box<int>         intBox(42);
    Box<double>      dblBox(3.14);
    Box<std::string> strBox("hello");

    intBox.print();   // Box: 42
    dblBox.print();   // Box: 3.14
    strBox.print();   // Box: hello

    intBox.set(100);
    std::cout << intBox.get() << "\n";  // 100
}
```

When *using* a templated class, you must supply the type argument explicitly in angle brackets: `Box<int>`, `Box<std::string>`, etc.

> [!IMPORTANT]
> **Templates must be fully defined in the header file.** Unlike ordinary classes, the compiler needs to see the *complete implementation* (not just the declaration) when it instantiates a template for a specific type. This means both the class definition and all method bodies belong in the `.h` file. Do **not** split the implementation into a separate `.cpp` file for templated classes.

###  Try It Yourself

Copy the `Box<T>` example above into a new file `box.cpp`, compile it, and run it:

```bash
$ g++ -std=c++11 box.cpp -o box
$ ./box
```

Then discuss the following with your group before moving on:

1. Can you create a `Box<Box<int>>`? Try it. Can you call `get()` on it and get back a `Box<int>`?
2. Write a free template function `void swap_boxes(Box<T>& a, Box<T>& b)` that swaps the contents of two boxes. What does the signature look like? (Hint: it needs its own `template <typename T>` line.)

### Converting SLList to a Templated Class

Now you will transform your `SLList` into `SLList<T>`, a list that can hold values of **any** type.
Create a **new file** called `sllist.hpp`. Because the entire implementation must live in the header, this single file replaces both `linkedlist.h` and `linkedlist.cpp`. You will copy-paste the code from both files into `sllist.hpp` and then make the necessary changes to turn it into a template class.
The transformation follows four mechanical rules applied everywhere in the class:

| What to change | Before | After |
|---|---|---|
| Line before the class definition | *(nothing)* | `template <typename T>` |
| `Node`'s data field and constructor parameter | `int data` / `const int& value` | `T data` / `const T& value` |
| Return types of `front()` and `back()` | `int&` | `T&` |
| Parameters of `push_front()` and `push_back()` | `const int& value` | `const T& value` |

Everything else stays the same: the pointer types (`Node*`), the constructor initializer lists, the size counter -- none of these change.

If you define methods **outside** the class body (recommended, since it mirrors what you did in Part 1), every method definition must be prefixed with its own `template <typename T>` line, and `SLList::` becomes `SLList<T>::`:

```cpp
// Before (in linkedlist.cpp):
void SLList::push_back(const int& value) { ... }

// After (in sllist.h, outside the class body):
template <typename T>
void SLList<T>::push_back(const T& value) { ... }
```

### Testing Your Templated List

Write a program called `testtemplate.cpp` that declares three instances of your `SLList<T>` with three different value types:

1. **`SLList<int>`**: push at least four integers, call `print()`, pop the front, call `print()` again.
2. **`SLList<float>`**: push at least three floats, call `print()`.
3. **`SLList<std::string>`**: push at least three strings, call `print()`, pop the back, call `print()` again.

Make sure you include `sllist.hpp` in your `testtemplate.cpp` file. Then compile and run with:

```bash
$ g++ -std=c++11 testtemplate.cpp -o testtemplate
$ ./testtemplate
```

> [!NOTE]
> Notice that you only compile `testtemplate.cpp`. There is **no** `sllist.cpp` to include on the command line. This is the practical consequence of the "templates must live in the header" rule: all the code the compiler needs is pulled in through `#include "sllist.hpp"`.

There is no autograder for this task.

---

## Submission

After completing both tasks, submit the files below to Gradescope. Your code should be well-formatted and easy to read. Test thoroughly before submitting. Full credit for this lab requires **attendance**, **collaboration with your group**, and **active participation**. No remote submissions will be accepted. If you have any questions, ask the instructor or TA.

- `linkedlist.cpp`
- `sllist.hpp`
- `testtemplate.cpp`
