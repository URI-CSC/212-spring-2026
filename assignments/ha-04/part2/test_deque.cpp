// ============================================================
// test_deque.cpp  -  Unit tests for Deque  (39 pts)
// ============================================================
// Compile and run:
//   g++ -std=c++11 -Wall -Werror block.cpp blockmap.cpp deque.cpp test_deque.cpp -o test_deque
//   ./test_deque
// ============================================================

#include "deque.h"
#include <iostream>
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
int main() {
    std::cout << "=== Deque Tests (39 pts) ===" << std::endl;

    // -- Test 1: empty deque ----------------------------------
    run_test("Empty deque", 1, [](){
        Deque dq;
        check(dq.size()  == 0,    "size() should be 0");
        check(dq.empty() == true, "empty() should be true");
    });

    // -- Test 2: push_back basic ------------------------------
    run_test("push_back - basic", 2, [](){
        Deque dq;
        dq.push_back(10);
        dq.push_back(20);
        dq.push_back(30);
        check(dq.size()  == 3,  "size() should be 3");
        check(dq.front() == 10, "front() should be 10");
        check(dq.back()  == 30, "back() should be 30");
        check(dq.front() == 10, "front() should be 10"); dq.pop_front();
        check(dq.front() == 20, "front() should be 20"); dq.pop_front();
        check(dq.front() == 30, "front() should be 30"); dq.pop_front();
        check(dq.empty(), "should be empty after drain");
    });

    // -- Test 3: push_front basic -----------------------------
    run_test("push_front - basic", 2, [](){
        Deque dq;
        dq.push_back(30);
        dq.push_front(20);
        dq.push_front(10);
        check(dq.size()  == 3,  "size() should be 3");
        check(dq.front() == 10, "front() should be 10");
        check(dq.back()  == 30, "back() should be 30");
        check(dq.front() == 10, "front() should be 10"); dq.pop_front();
        check(dq.front() == 20, "front() should be 20"); dq.pop_front();
        check(dq.front() == 30, "front() should be 30"); dq.pop_front();
        check(dq.empty(), "should be empty after drain");
    });

    // -- Test 4: pop_back to empty ----------------------------
    run_test("pop_back - to empty", 2, [](){
        Deque dq;
        dq.push_back(1); dq.push_back(2); dq.push_back(3);
        check(dq.back()  == 3,    "back() should be 3");    dq.pop_back();
        check(dq.size()  == 2,    "size() should be 2");
        check(dq.back()  == 2,    "back() should be 2");    dq.pop_back();
        check(dq.size()  == 1,    "size() should be 1");
        check(dq.back()  == 1,    "back() should be 1");    dq.pop_back();
        check(dq.empty() == true, "should be empty");
    });

    // -- Test 5: pop_front to empty ---------------------------
    run_test("pop_front - to empty", 2, [](){
        Deque dq;
        dq.push_back(1); dq.push_back(2); dq.push_back(3);
        check(dq.front() == 1,    "front() should be 1");   dq.pop_front();
        check(dq.size()  == 2,    "size() should be 2");
        check(dq.front() == 2,    "front() should be 2");   dq.pop_front();
        check(dq.size()  == 1,    "size() should be 1");
        check(dq.front() == 3,    "front() should be 3");   dq.pop_front();
        check(dq.empty() == true, "should be empty");
    });

    // -- Test 6: push_back across block boundaries ------------
    // With BLOCK_SIZE=4, every 4 push_backs forces a new Block.
    run_test("push_back - across blocks", 3, [](){
        Deque dq;
        for (int i = 0; i < 20; i++) dq.push_back(i);
        check(dq.size()  == 20, "size() should be 20");
        check(dq.front() == 0,  "front() should be 0");
        check(dq.back()  == 19, "back() should be 19");
        for (int i = 0; i < 20; i++) {
            check(dq.front() == i, "front() should return i in order");
            dq.pop_front();
        }
        check(dq.empty(), "should be empty after draining");
    });

    // -- Test 7: push_front across block boundaries -----------
    run_test("push_front - across blocks", 3, [](){
        Deque dq;
        for (int i = 0; i < 20; i++) dq.push_front(i);
        // logical order: [19, 18, 17, ..., 1, 0]
        check(dq.size()  == 20, "size() should be 20");
        check(dq.front() == 19, "front() should be 19");
        check(dq.back()  == 0,  "back() should be 0");
        for (int i = 0; i < 20; i++) {
            check(dq.front() == 19 - i, "front() should return 19-i in order");
            dq.pop_front();
        }
        check(dq.empty(), "should be empty after draining");
    });

    // -- Test 8: interleaved push_front and push_back ---------
    run_test("Interleaved push_front / push_back", 3, [](){
        Deque dq;
        dq.push_back(3);   // [3]
        dq.push_back(4);   // [3, 4]
        dq.push_front(2);  // [2, 3, 4]
        dq.push_front(1);  // [1, 2, 3, 4]
        dq.push_back(5);   // [1, 2, 3, 4, 5]
        check(dq.size() == 5, "size() should be 5");
        for (int i = 0; i < 5; i++) {
            check(dq.front() == i + 1, "front() should return i+1 in order");
            dq.pop_front();
        }
        check(dq.empty(), "should be empty after draining");
    });

    // -- Test 9: pop to empty, then push again ----------------
    run_test("Pop to empty then push", 2, [](){
        Deque dq;
        dq.push_back(1);
        dq.push_back(2);
        dq.pop_front();
        dq.pop_front();
        check(dq.empty() == true, "should be empty");

        dq.push_back(99);
        check(dq.size()  == 1,  "size() should be 1");
        check(dq.front() == 99, "front() should be 99");
        check(dq.back()  == 99, "back() should be 99");
    });

    // -- Tests 10-14: exceptions on empty deque (1 pt each) ---
    run_test("pop_back on empty - exception", 1, [](){
        Deque dq;
        bool caught = false;
        try { dq.pop_back(); } catch (const std::out_of_range&) { caught = true; }
        check(caught, "pop_back() should throw std::out_of_range on empty deque");
    });

    run_test("pop_front on empty - exception", 1, [](){
        Deque dq;
        bool caught = false;
        try { dq.pop_front(); } catch (const std::out_of_range&) { caught = true; }
        check(caught, "pop_front() should throw std::out_of_range on empty deque");
    });

    run_test("front() on empty - exception", 1, [](){
        Deque dq;
        bool caught = false;
        try { dq.front(); } catch (const std::out_of_range&) { caught = true; }
        check(caught, "front() should throw std::out_of_range on empty deque");
    });

    run_test("back() on empty - exception", 1, [](){
        Deque dq;
        bool caught = false;
        try { dq.back(); } catch (const std::out_of_range&) { caught = true; }
        check(caught, "back() should throw std::out_of_range on empty deque");
    });

    // -- Test 14: large mixed push/pop ------------------------
    run_test("Large mixed push / pop", 3, [](){
        Deque dq;
        for (int i = 0; i < 50; i++) {
            if (i % 2 == 0) dq.push_back(i);
            else             dq.push_front(i);
        }
        check(dq.size() == 50, "size() should be 50");

        while (!dq.empty()) {
            dq.pop_front();
            if (!dq.empty()) dq.pop_back();
        }
        check(dq.empty() == true, "should be empty after draining");
        check(dq.size()  == 0,    "size() should be 0");
    });

    // -- Test 15: large push_back - correctness at scale ------
    run_test("Large push_back - 10000 elements", 3, [](){
        Deque dq;
        for (int i = 0; i < 10000; i++) dq.push_back(i);
        check(dq.size()  == (size_t)10000, "size should be 10000");
        check(dq.front() == 0,             "front should be 0");
        check(dq.back()  == 9999,          "back should be 9999");
        // Drain alternating from both ends; verify every value.
        for (int i = 0; i < 5000; i++) {
            check(dq.front() == i,        "front() should return i");       dq.pop_front();
            check(dq.back()  == 9999 - i, "back() should return 9999-i");   dq.pop_back();
        }
        check(dq.empty(), "should be empty after full drain");
    });

    // -- Test 16: destructor / memory stress ------------------
    // 4000 rounds x 10000 elements (~112 KB per round).
    // A missing ~BlockMap leaks ~450 MB total, triggering std::bad_alloc.
    run_test("Destructor / memory stress", 5, [](){
        // Sanity check: push_back must work for this test to be meaningful.
        {
            Deque sanity;
            sanity.push_back(1);
            if (sanity.size() != 1)
                throw std::runtime_error(
                    "push_back not working - cannot verify destructor");
            // ~sanity: ~BlockMap must free the block and slots array
        }
        for (int round = 0; round < 4000; round++) {
            Deque dq;
            for (int i = 0; i < 10000; i++) dq.push_back(i);
            // ~dq: ~BlockMap must delete all Block objects and the slots array
        }
    });

    // -- Test 17: circular BlockMap wrap-around and resize ----
    // Exercises the hardest BlockMap invariant: resize() must copy blocks
    // in logical order even when front_idx has wrapped past slot 0.
    //
    // With BLOCK_SIZE=4, initial capacity=8:
    //   Phase 1: push 16 to back  -> 4 blocks at physical slots [0,1,2,3], front_idx=0
    //   Phase 2: drain front       -> front_idx advances to physical slot 4, count=0
    //   Phase 3: push 32 to back  -> fills slots 4-7, then back_idx wraps:
    //            (4+4)%8=0, (4+5)%8=1, (4+6)%8=2, (4+7)%8=3
    //            Result: front_idx=4, count=8=capacity (full AND wrapped)
    //   Phase 4: one more push    -> triggers resize(); a buggy resize() that
    //            copies slot[0]..slot[7] in physical order produces
    //            values 116..131 at the front instead of 100..115
    //   Phase 5: drain and verify every element in logical order
    run_test("Circular BlockMap wrap and resize", 4, [](){
        Deque dq;

        // Phase 1: 4 blocks fill physical slots 0-3; front_idx stays at 0
        for (int i = 0; i < 16; i++) dq.push_back(i);

        // Phase 2: drain all; front_idx now sits at physical slot 4
        for (int i = 0; i < 16; i++) dq.pop_front();
        check(dq.empty(),     "should be empty after first drain");
        check(dq.size() == 0, "size() should be 0 after first drain");

        // Phase 3: 8 more blocks; slots 4-7 fill first, then back_idx wraps to 0-3
        // front_idx=4, count=8=capacity -> BlockMap is full and wrapped
        for (int i = 0; i < 32; i++) dq.push_back(100 + i);
        check(dq.size()  == 32,  "size() should be 32 after circular fill");
        check(dq.front() == 100, "front() should be 100");
        check(dq.back()  == 131, "back() should be 131");

        // Phase 4: triggers resize(); correct impl copies logical order [4,5,6,7,0,1,2,3]
        dq.push_back(999);
        check(dq.size()  == 33,  "size() should be 33 after resize");
        check(dq.front() == 100, "front() should still be 100 after resize");
        check(dq.back()  == 999, "back() should be 999 after resize");

        // Phase 5: drain and verify every element — order proves resize was correct
        for (int i = 0; i < 32; i++) {
            check(dq.front() == 100 + i, "element order preserved through circular wrap and resize");
            dq.pop_front();
        }
        check(dq.front() == 999, "last element should be 999");
        dq.pop_front();
        check(dq.empty(), "should be empty after complete drain");
    });

    // -- Final score ------------------------------------------
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Score: " << g_earned << " / " << g_total << std::endl;
    std::cout << "========================================" << std::endl;
    return (g_earned == g_total) ? 0 : 1;
}
