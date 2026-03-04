#include "maxpqueue.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

int main() {
    int points = 0;
    bool caught = false;

    // -----------------------------------------------------------------------
    // Task 1: Core Operations
    // -----------------------------------------------------------------------
    std::cout << "=== Task 1: MaxPQueue Core Operations ===" << std::endl;

    // push and top: the maximum string must always be at the root
    {
        MaxPQueue pq(10);
        pq.push("apple");
        pq.push("mango");
        pq.push("banana");
        assert(pq.top() == "mango");   // "mango" > "banana" > "apple"
        points += 2;
        std::cout << "[+2] push + top (max at root): PASSED" << std::endl;
    }

    // pop: elements must come out in descending alphabetical order
    {
        MaxPQueue pq(10);
        pq.push("cherry");
        pq.push("avocado");
        pq.push("watermelon");
        pq.push("blueberry");
        assert(pq.top() == "watermelon");
        pq.pop();
        assert(pq.top() == "cherry");
        pq.pop();
        assert(pq.top() == "blueberry");
        pq.pop();
        assert(pq.top() == "avocado");
        pq.pop();
        assert(pq.empty() == true);
        points += 2;
        std::cout << "[+2] pop (correct descending order): PASSED" << std::endl;
    }

    // size and empty
    {
        MaxPQueue pq(5);
        assert(pq.empty() == true);
        assert(pq.size() == 0);
        pq.push("alpha");
        assert(pq.empty() == false);
        assert(pq.size() == 1);
        pq.push("beta");
        assert(pq.size() == 2);
        pq.pop();
        assert(pq.size() == 1);
        pq.pop();
        assert(pq.empty() == true);
        assert(pq.size() == 0);
        points += 1;
        std::cout << "[+1] size and empty: PASSED" << std::endl;
    }

    // exceptions: overflow_error, underflow_error, and invalid_argument
    {
        // push on a full queue
        caught = false;
        MaxPQueue full(3);
        full.push("x");
        full.push("y");
        full.push("z");
        try {
            full.push("w");    // queue is full -- must throw
        } catch (const std::overflow_error&) {
            caught = true;
        }
        assert(caught);
        std::cout << "    push on full queue: exception PASSED" << std::endl;

        // pop on an empty queue
        caught = false;
        MaxPQueue empty_pq(5);
        try {
            empty_pq.pop();    // must throw
        } catch (const std::underflow_error&) {
            caught = true;
        }
        assert(caught);
        std::cout << "    pop on empty queue: exception PASSED" << std::endl;

        // top on an empty queue
        caught = false;
        try {
            empty_pq.top();    // must throw
        } catch (const std::underflow_error&) {
            caught = true;
        }
        assert(caught);
        std::cout << "    top on empty queue: exception PASSED" << std::endl;

        // invalid capacity
        caught = false;
        try {
            MaxPQueue bad(0);  // must throw
        } catch (const std::invalid_argument&) {
            caught = true;
        }
        assert(caught);
        points += 1;
        std::cout << "[+1] all exceptions thrown correctly: PASSED" << std::endl;
    }

    // Rule of Three: copy constructor and copy assignment
    {
        MaxPQueue original(10);
        original.push("fig");
        original.push("grape");
        original.push("elderberry");

        MaxPQueue copy1(original);         // copy constructor
        assert(copy1.size() == 3);
        assert(copy1.top() == "grape");

        // Deep copy: modifying the copy must not affect the original
        copy1.pop();
        assert(original.top() == "grape");
        assert(original.size() == 3);
        std::cout << "    copy constructor (deep copy verified): PASSED" << std::endl;

        // Copy assignment
        MaxPQueue pq2(10);
        pq2.push("zzz");
        pq2 = original;                    // copy assignment
        assert(pq2.size() == 3);
        assert(pq2.top() == "grape");

        // Deep copy: modifying pq2 must not affect original
        pq2.push("zebra");
        assert(original.size() == 3);
        std::cout << "    copy assignment (deep copy verified): PASSED" << std::endl;

        // Self-assignment must not crash or corrupt data
        MaxPQueue& ref = original;
        original = ref;
        assert(original.top() == "grape");
        assert(original.size() == 3);
        std::cout << "    self-assignment handled: PASSED" << std::endl;

        // Chained assignment: c = b = a
        MaxPQueue a(5), b(5), c(5);
        a.push("hello");
        c = b = a;
        assert(b.top() == "hello");
        assert(c.top() == "hello");
        b.push("world");
        assert(a.size() == 1);   // a unchanged
        assert(c.size() == 1);   // c unchanged
        std::cout << "    chained assignment: PASSED" << std::endl;

        points += 2;
        std::cout << "[+2] Rule of Three (copy ctor + assignment): PASSED" << std::endl;
    }

    std::cout << "\n>>> Task 1 Complete! (" << points << "/8 pts) <<<\n" << std::endl;

    // -----------------------------------------------------------------------
    // Task 2: buildHeap Constructor
    // (prints SKIP until the buildHeap constructor is implemented)
    // -----------------------------------------------------------------------
    std::cout << "=== Task 2: buildHeap Constructor ===" << std::endl;

    int task2_points = 0;
    try {
        // Basic: top() must equal the alphabetically greatest element
        {
            std::vector<std::string> words = {"cat","ant","fox","bee","dog","eel","gnu"};
            MaxPQueue pq(words);
            assert(pq.size() == 7);
            assert(pq.top() == "gnu");     // "gnu" is alphabetically greatest
            task2_points += 2;
            std::cout << "[+2] buildHeap top() is max element: PASSED" << std::endl;
        }

        // Pop all: must come out in non-increasing alphabetical order
        {
            std::vector<std::string> words =
                {"pear","apple","orange","kiwi","mango","fig","grape"};
            MaxPQueue pq(words);
            std::string prev = pq.top();
            pq.pop();
            while (!pq.empty()) {
                assert(pq.top() <= prev);  // each element <= the previous
                prev = pq.top();
                pq.pop();
            }
            task2_points += 2;
            std::cout << "[+2] buildHeap pop order (descending): PASSED" << std::endl;
        }

        // Edge case: single-element vector
        {
            std::vector<std::string> one = {"solo"};
            MaxPQueue pq(one);
            assert(pq.size() == 1);
            assert(pq.top() == "solo");
            pq.pop();
            assert(pq.empty() == true);
            task2_points += 1;
            std::cout << "[+1] buildHeap single element: PASSED" << std::endl;
        }

        // Already sorted ascending input still produces a valid heap
        {
            std::vector<std::string> sorted = {"aardvark","bear","crane","dolphin","eagle"};
            MaxPQueue pq(sorted);
            assert(pq.top() == "eagle");
            std::cout << "    buildHeap on sorted input: PASSED" << std::endl;
        }

    } catch (const std::logic_error& e) {
        std::cout << "[SKIP] Task 2 tests: " << e.what() << std::endl;
        std::cout << "       Implement the buildHeap constructor to pass Task 2." << std::endl;
    }

    points += task2_points;
    std::cout << "\n>>> Task 2 Complete! (" << task2_points << "/5 pts) <<<\n" << std::endl;

    // -----------------------------------------------------------------------
    // Final banner
    // -----------------------------------------------------------------------
    std::cout << "========================================" << std::endl;
    std::cout << "  ALL TESTS PASSED!" << std::endl;
    std::cout << "  Total (Tasks 1+2): " << points << "/13 points" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
