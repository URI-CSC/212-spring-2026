// ============================================================
// test_block.cpp  -  Unit tests for Block
// ============================================================
// Compile and run:
//   g++ -std=c++11 -Wall -Werror block.cpp test_block.cpp -o test_block
//   ./test_block
// ============================================================

#include "block.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string>

// -- Point tracker ------------------------------------------
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
int main() {
    std::cout << "=== Block Tests (20 pts) ===" << std::endl;

    // -- [2 pts] Default constructor --------------------------
    run_test("Default constructor", 2, [](){
        Block b;
        check(b.size()           == 0,    "size() should be 0");
        check(b.empty()          == true, "empty() should be true");
        check(b.can_push_back()  == true, "can_push_back() should be true (tail=0 < BLOCK_SIZE)");
        check(b.can_push_front() == false,"can_push_front() should be false (head=0)");
    });

    // -- [3 pts] push_back and at -----------------------------
    run_test("push_back and at()", 3, [](){
        Block b;
        b.push_back(10);
        b.push_back(20);
        b.push_back(30);
        check(b.size()  == 3,  "size() should be 3 after 3 push_backs");
        check(b.empty() == false, "empty() should be false");
        check(b.front() == 10, "front() should be 10");
        check(b.back()  == 30, "back() should be 30");
        check(b.at(0)   == 10, "at(0) should be 10");
        check(b.at(1)   == 20, "at(1) should be 20");
        check(b.at(2)   == 30, "at(2) should be 30");
    });

    // -- [2 pts] Block(BLOCK_SIZE, BLOCK_SIZE) constructor ----
    run_test("Block(BLOCK_SIZE, BLOCK_SIZE) constructor", 2, [](){
        Block fb(BLOCK_SIZE, BLOCK_SIZE);
        check(fb.size()           == 0,    "size() should be 0");
        check(fb.empty()          == true, "empty() should be true");
        check(fb.can_push_back()  == false,"can_push_back() should be false (tail=BLOCK_SIZE)");
        check(fb.can_push_front() == true, "can_push_front() should be true (head=BLOCK_SIZE > 0)");
        fb.push_front(40);
        fb.push_front(30);
        check(fb.size()  == 2,  "size() should be 2 after 2 push_fronts");
        check(fb.front() == 30, "front() should be 30");
        check(fb.back()  == 40, "back() should be 40");
        check(fb.at(0)   == 30, "at(0) should be 30");
        check(fb.at(1)   == 40, "at(1) should be 40");
    });

    // -- [3 pts] pop_back -------------------------------------
    run_test("pop_back", 3, [](){
        Block b;
        b.push_back(1); b.push_back(2); b.push_back(3);
        check(b.back()  == 3,    "back() should be 3 before pop");    b.pop_back();
        check(b.size()  == 2,    "size() should be 2");
        check(b.back()  == 2,    "back() should be 2 before pop");    b.pop_back();
        check(b.size()  == 1,    "size() should be 1");
        check(b.back()  == 1,    "back() should be 1 before pop");    b.pop_back();
        check(b.empty() == true, "block should be empty after 3 pops");
    });

    // -- [3 pts] pop_front ------------------------------------
    run_test("pop_front", 3, [](){
        Block b;
        b.push_back(10); b.push_back(20); b.push_back(30);
        check(b.front() == 10, "front() should be 10 before pop");   b.pop_front();
        check(b.size()  == 2,  "size() should be 2");
        check(b.front() == 20, "front() should be 20 before pop");   b.pop_front();
        check(b.front() == 30, "front() should be 30 before pop");   b.pop_front();
        check(b.empty() == true, "block should be empty");
    });

    // -- [2 pts] Full block state -----------------------------
    run_test("Full block state", 2, [](){
        Block b;
        for (size_t i = 0; i < BLOCK_SIZE; i++) b.push_back((int)i);
        check(b.size()           == BLOCK_SIZE, "size() should equal BLOCK_SIZE");
        check(b.can_push_back()  == false,      "can_push_back() should be false when full");
        check(b.can_push_front() == false,      "can_push_front() should be false (head=0)");
    });

    // -- [3 pts] at() out of range ----------------------------
    run_test("at() throws std::out_of_range", 3, [](){
        Block b;
        b.push_back(5);
        bool caught = false;
        try {
            b.at(1);    // only index 0 is valid
        } catch (const std::out_of_range&) {
            caught = true;
        }
        check(caught, "at(1) should throw std::out_of_range when size()==1");

        caught = false;
        try {
            b.at(100);
        } catch (const std::out_of_range&) {
            caught = true;
        }
        check(caught, "at(100) should throw std::out_of_range");
    });

    // -- [2 pts] Mixed push_front / push_back -----------------
    run_test("Mixed push_front / push_back", 2, [](){
        // Block(1, BLOCK_SIZE-1): head=1, tail=BLOCK_SIZE-1
        // Has room on both sides
        Block b(1, BLOCK_SIZE - 1);
        check(b.can_push_front() == true, "can_push_front() should be true (head=1)");
        check(b.can_push_back()  == true, "can_push_back() should be true (tail<BLOCK_SIZE)");
        size_t sz = b.size();
        b.push_front(99);
        check(b.front() == 99,    "front() should be 99 after push_front");
        check(b.size()  == sz+1,  "size() should increase by 1");
        b.push_back(77);
        check(b.back()  == 77,    "back() should be 77 after push_back");
        check(b.size()  == sz+2,  "size() should increase by 1 again");
    });

    // -- Score -------------------------------------------------
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Score: " << g_earned << " / " << g_total << std::endl;
    std::cout << "========================================" << std::endl;
    return (g_earned == g_total) ? 0 : 1;
}
