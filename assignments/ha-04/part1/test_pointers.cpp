// ============================================================
// Assignment 04 - Part 1: Pointer Refresher  (30 pts)
// test_pointers.cpp  --  provided autograder, do not modify
// ============================================================
// Compile and run:
//   g++ -std=c++11 -Wall -Werror pointers.cpp test_pointers.cpp -o test
//   ./test
// ============================================================

#include "pointers.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <string>

// -- Scoring helpers ------------------------------------------
int g_earned = 0;
int g_total  = 0;

void check(bool cond, const char* msg = "condition failed") {
    if (!cond) throw std::runtime_error(msg);
}

void run_test(const std::string& name, int pts,
                     const std::function<void()>& fn) {
    g_total += pts;
    try {
        fn();
        g_earned += pts;
        std::cout << "  [" << pts << "/" << pts << "] "
                  << name << ": PASSED" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  [0/" << pts << "] "
                  << name << ": FAILED" << std::endl;
    }
}


// ============================================================
// SECTION 1 - Pointer Basics  (5 pts)
// ============================================================
void section1_pointer_basics() {
    int  x = 42;
    int* p = nullptr;

    // -- Task 1a  (2 pts) -------------------------------------
    run_test("Task 1a - address-of & dereference", 2, [&](){
        p = task1a_point_to(x);
        check(p != nullptr, "p should not be nullptr");
        check(p == &x,      "p must be the address of x, not a copy");
        check(*p == 42,     "*p should equal 42");
    });

    // -- Task 1b  (1 pt) --------------------------------------
    run_test("Task 1b - modify through pointer", 1, [&](){
        if (p == nullptr)
            throw std::runtime_error("p is nullptr (did Task 1a pass?)");
        task1b_modify(p);
        check(x == 100, "x should be 100 after writing through p");
    });

    // -- Task 1c  (2 pts) -------------------------------------
    run_test("Task 1c - pointer arithmetic", 2, [](){
        int arr[5] = {10, 20, 30, 40, 50};
        std::ostringstream oss;
        std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
        task1c_print_array(arr, 5);
        std::cout.rdbuf(old_buf);
        std::string out = oss.str();
        // parse output and verify correct values in correct order
        std::istringstream iss(out);
        int expected[] = {10, 20, 30, 40, 50};
        int idx = 0, val;
        while (iss >> val) {
            check(idx < 5,              "should not print more than 5 values");
            check(val == expected[idx], "values must be printed in order: 10 20 30 40 50");
            idx++;
        }
        check(idx == 5, "should print exactly 5 values");
    });
}


// ============================================================
// SECTION 2 - Array of Pointers to Arrays (Jagged 2D)  (10 pts)
// ============================================================
//   grid[0] --> [ 0 ][ 1 ][ 2 ]              (3 elements)
//   grid[1] --> [ 0 ][ 1 ][ 2 ][ 3 ][ 4 ]   (5 elements)
//   grid[2] --> [ 0 ][ 1 ]                   (2 elements)
// ============================================================
void section2_array_of_pointers() {
    const int ROWS        = 3;
    const int sizes[ROWS] = {3, 5, 2};

    int** grid = nullptr;

    // -- Task 2a  (3 pts) -------------------------------------
    run_test("Task 2a - allocate jagged grid", 3, [&](){
        grid = task2a_allocate(ROWS, sizes);
        check(grid != nullptr, "grid should not be nullptr after allocation");
        for (int i = 0; i < ROWS; i++)
            check(grid[i] != nullptr, "each grid[i] should not be nullptr");
        for (int i = 0; i < ROWS; i++)
            for (int j = i + 1; j < ROWS; j++)
                check(grid[i] != grid[j], "each row must be a distinct allocation");
    });

    // -- Task 2b  (5 pts) -------------------------------------
    run_test("Task 2b - fill", 5, [&](){
        if (grid == nullptr)
            throw std::runtime_error("grid is nullptr (did Task 2a pass?)");
        task2b_fill(grid, ROWS, sizes);
        check(grid[0][0] == 0 && grid[0][1] == 1 && grid[0][2] == 2,
              "grid[0] should be {0, 1, 2}");
        check(grid[1][0] == 0 && grid[1][1] == 1 && grid[1][2] == 2 &&
              grid[1][3] == 3 && grid[1][4] == 4,
              "grid[1] should be {0, 1, 2, 3, 4}");
        check(grid[2][0] == 0 && grid[2][1] == 1,
              "grid[2] should be {0, 1}");
    });

    // -- Task 2c  (2 pts) -------------------------------------
    run_test("Task 2c - free memory", 2, [&](){
        if (grid == nullptr)
            throw std::runtime_error("grid is nullptr (did Task 2a pass?)");
        task2c_free(grid, ROWS);
        grid = nullptr;
        // stress: if any row leaks or wrong-order free corrupts the heap,
        // new will throw or the process will crash within the loop
        const int sizes2[ROWS] = {3, 5, 2};
        for (int iter = 0; iter < 100000; iter++) {
            int** g = task2a_allocate(ROWS, sizes2);
            task2c_free(g, ROWS);
        }
    });
}


// ============================================================
// SECTION 3 - Rule of Three  (15 pts)
// ============================================================
void section3_rule_of_three() {
    IntArray a(5);

    // -- Task 3a  (2 pts) -------------------------------------
    run_test("Task 3a - constructor", 2, [&](){
        // dirty the heap so fresh allocation won't accidentally be zero
        { int* d[8]; for (int k=0;k<8;k++){d[k]=new int[5];for(int j=0;j<5;j++)d[k][j]=0xDEAD;} for(int k=0;k<8;k++) delete[] d[k]; }
        IntArray t(5);
        check(t.size() == 5, "size should be 5");
        for (size_t i = 0; i < t.size(); i++)
            check(t.at(i) == 0, "every element should be initialized to 0");
    });

    a.at(0) = 10;  a.at(1) = 20;  a.at(2) = 30;

    // -- Task 3b  (2 pts) -------------------------------------
    run_test("Task 3b - destructor", 2, [](){
        // if destructor leaks, new throws std::bad_alloc before 100000 iters
        for (int iter = 0; iter < 100000; iter++) { IntArray tmp(5); }
    });

    // -- Task 3c  (4 pts) -------------------------------------
    run_test("Task 3c - copy constructor", 4, [&](){
        // all elements copied correctly
        IntArray b(a);
        check(b.size() == 5,  "b.size() should be 5");
        check(b.at(0)  == 10, "b.at(0) should be 10");
        check(b.at(1)  == 20, "b.at(1) should be 20");
        check(b.at(2)  == 30, "b.at(2) should be 30");
        check(b.at(3)  == 0,  "b.at(3) should be 0");
        check(b.at(4)  == 0,  "b.at(4) should be 0");
        // deep copy: modifying the copy must not affect the original;
        // deleting the copy must not corrupt the original
        IntArray* pb = new IntArray(a);
        pb->at(0) = 999;
        check(a.at(0) == 10, "a should be unchanged after modifying the copy");
        delete pb;  // shallow copy would free a's data here
        check(a.at(0) == 10, "a should be intact after the copy is destroyed");
        // stress: if copy constructor leaks, new throws before 100000 iters
        for (int iter = 0; iter < 100000; iter++) { IntArray tmp(a); }
    });

    // -- Task 3d  (7 pts) -------------------------------------
    run_test("Task 3d - copy assignment", 7, [&](){
        // size increase: 3 -> 5
        IntArray c(3);
        c.at(0) = 7;
        c = a;
        check(c.size() == 5,  "c.size() should be 5 after c = a");
        check(c.at(0)  == 10, "c.at(0) should be 10");
        check(c.at(1)  == 20, "c.at(1) should be 20");
        check(c.at(2)  == 30, "c.at(2) should be 30");
        // size decrease: 5 -> 2
        IntArray d(5);
        IntArray e(2);
        e.at(0) = 7;  e.at(1) = 8;
        d = e;
        check(d.size() == 2, "size should shrink when assigning a smaller array");
        check(d.at(0)  == 7, "d.at(0) should be 7");
        check(d.at(1)  == 8, "d.at(1) should be 8");
        // deep copy: all elements, independence
        IntArray f(3);
        f = a;
        check(f.at(0) == 10, "assignment must copy all elements");
        check(f.at(1) == 20, "assignment must copy all elements");
        check(f.at(2) == 30, "assignment must copy all elements");
        check(f.at(3) == 0,  "assignment must copy all elements");
        check(f.at(4) == 0,  "assignment must copy all elements");
        f.at(1) = 888;
        check(a.at(1) == 20, "a should be unchanged after modifying the assigned copy");
        // self-assignment
        IntArray& ref = a;
        a = ref;
        check(a.at(0) == 10, "a.at(0) should survive self-assignment");
        check(a.size() == 5,  "a.size() should survive self-assignment");
        // chained assignment
        IntArray x(2), y(2), z(2);
        x.at(0) = 42;
        z = y = x;
        check(y.at(0) == 42, "y.at(0) should be 42 after z = y = x");
        check(z.at(0) == 42, "z.at(0) should be 42 after z = y = x");
        y.at(0) = 0;
        check(x.at(0) == 42, "x should be unchanged after modifying y");
        check(z.at(0) == 42, "z should be unchanged after modifying y");
        // stress: if operator= forgets delete[] data, old array leaks every iter
        IntArray s(3);
        for (int iter = 0; iter < 100000; iter++) { s = a; }
    });
}


// ============================================================
int main() {
    std::cout << "=== Part 1: Pointer Refresher (30 pts) ===" << std::endl;

    std::cout << "\n--- Section 1: Pointer Basics (5 pts) ---" << std::endl;
    section1_pointer_basics();

    std::cout << "\n--- Section 2: Array of Pointers (10 pts) ---" << std::endl;
    section2_array_of_pointers();

    std::cout << "\n--- Section 3: Rule of Three (15 pts) ---" << std::endl;
    section3_rule_of_three();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Score: " << g_earned << " / " << g_total << std::endl;
    std::cout << "========================================" << std::endl;
    return (g_earned == g_total) ? 0 : 1;
}
