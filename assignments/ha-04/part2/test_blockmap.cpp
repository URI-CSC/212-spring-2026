// ============================================================
// test_blockmap.cpp  -  Unit tests for BlockMap  (22 pts)
// ============================================================
// Compile and run:
//   g++ -std=c++11 -Wall -Werror block.cpp blockmap.cpp test_blockmap.cpp -o test_blockmap
//   ./test_blockmap
// ============================================================

#include "blockmap.h"
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
    std::cout << "=== BlockMap Tests (22 pts) ===" << std::endl;

    // -- Test 1: default constructor --------------------------
    run_test("Default constructor", 1, [](){
        BlockMap bm;
        check(bm.size()  == 0,    "size() should be 0");
        check(bm.empty() == true, "empty() should be true");
        check(bm.full()  == false,"full() should be false");
    });

    // -- Test 2: push_back_block (single block) ---------------
    run_test("push_back_block - single block", 2, [](){
        BlockMap bm;
        Block* b1 = new Block();
        b1->push_back(1);
        b1->push_back(2);
        bm.push_back_block(b1);

        check(bm.size()                 == 1,  "size() should be 1");
        check(bm.empty()                == false, "empty() should be false");
        check(bm.front_block()          == b1, "front_block() should be b1");
        check(bm.back_block()           == b1, "back_block() should be b1");
        check(bm.get_block(0)           == b1, "get_block(0) should be b1");
        check(bm.front_block()->front() == 1,  "front element should be 1");
    });

    // -- Test 3: push_back_block (two blocks) -----------------
    run_test("push_back_block - two blocks", 2, [](){
        BlockMap bm;
        Block* b1 = new Block();  b1->push_back(10);
        Block* b2 = new Block();  b2->push_back(20);
        bm.push_back_block(b1);
        bm.push_back_block(b2);

        check(bm.size()        == 2,  "size() should be 2");
        check(bm.front_block() == b1, "front_block() should be b1");
        check(bm.back_block()  == b2, "back_block() should be b2");
        check(bm.get_block(0)  == b1, "get_block(0) should be b1");
        check(bm.get_block(1)  == b2, "get_block(1) should be b2");
    });

    // -- Test 4: push_front_block -----------------------------
    run_test("push_front_block", 3, [](){
        BlockMap bm;
        Block* b1 = new Block();                      b1->push_back(1);
        Block* b2 = new Block();                      b2->push_back(2);
        Block* b0 = new Block(BLOCK_SIZE, BLOCK_SIZE); b0->push_front(0);

        bm.push_back_block(b1);
        bm.push_back_block(b2);
        bm.push_front_block(b0);   // logical order: b0, b1, b2

        check(bm.size()        == 3,  "size() should be 3");
        check(bm.front_block() == b0, "front_block() should be b0");
        check(bm.back_block()  == b2, "back_block() should be b2");
        check(bm.get_block(0)  == b0, "get_block(0) should be b0");
        check(bm.get_block(1)  == b1, "get_block(1) should be b1");
        check(bm.get_block(2)  == b2, "get_block(2) should be b2");
    });

    // -- Test 5: pop_front_block (including drain to empty) ---
    run_test("pop_front_block", 3, [](){
        BlockMap bm;
        Block* b1 = new Block(); b1->push_back(1);
        Block* b2 = new Block(); b2->push_back(2);
        bm.push_back_block(b1);
        bm.push_back_block(b2);

        bm.pop_front_block();   // deletes b1

        check(bm.size()                 == 1,  "size() should be 1");
        check(bm.front_block()          == b2, "front_block() should now be b2");
        check(bm.front_block()->front() == 2,  "front value should be 2");

        bm.pop_front_block();   // deletes b2 - should reach empty

        check(bm.empty() == true, "should be empty after popping all blocks");
        check(bm.size()  == 0,    "size() should be 0");
    });

    // -- Test 6: pop_back_block -------------------------------
    run_test("pop_back_block", 3, [](){
        BlockMap bm;
        Block* b1 = new Block(); b1->push_back(1);
        Block* b2 = new Block(); b2->push_back(2);
        bm.push_back_block(b1);
        bm.push_back_block(b2);

        bm.pop_back_block();    // deletes b2

        check(bm.size()                == 1, "size() should be 1");
        check(bm.back_block()          == b1, "back_block() should now be b1");
        check(bm.back_block()->front() == 1,  "back value should be 1");
    });

    // -- Test 7: resize (order preserved) ---------------------
    // Start with capacity 2 to force multiple resizes quickly.
    run_test("resize - logical order preserved", 4, [](){
        BlockMap bm(2);
        const int N = 6;
        for (int i = 0; i < N; i++) {
            Block* b = new Block();
            b->push_back(i * 10);
            bm.push_back_block(b);  // triggers resize at i=2 and i=4
        }
        check(bm.size() == (size_t)N, "size() should be 6 after 6 push_back_block calls");
        for (int i = 0; i < N; i++) {
            check(bm.get_block(i)->front() == i * 10,
                  "logical order should be preserved after resize");
        }
    });

    // -- Test 8: test blockmap at max capacity ------------
    // Push 4 blocks to the back
    run_test("max capacity", 1, [](){
        const int N = 4;
        BlockMap bm(4);
        auto blocks = new Block*[N];
        for (int i = 0; i < N; i++) {
            blocks[i] = new Block(0, 0);
            blocks[i]->push_back(i);
            bm.push_back_block(blocks[i]);
        }

        for (int i = 0; i < N; i++)
            check(bm.get_block(i) == blocks[i], "get_block(i) should be correct");

        check(bm.front_block() == blocks[0], "front_block() should be correct");
        check(bm.back_block() == blocks[N - 1], "back_block() should be correct");
    });

    // -- Test 9: circular wrapping with push_front and push_back ------------
    // Push 6 blocks to the front/back; last pushed = logical index 0.
    run_test("circular push_front and push_back", 1, [](){
        BlockMap bm(4), bmb(4);
        const int N = 6;
        for (int i = 0; i < N; i++) {
            Block* b = new Block(BLOCK_SIZE, BLOCK_SIZE);
            b->push_front(i * 5);
            bm.push_front_block(b);

            Block* bb = new Block(BLOCK_SIZE, BLOCK_SIZE);
            bb->push_front(i * 5);
            bmb.push_back_block(bb);
        }
        check(bm.size() == (size_t)N && bmb.size() == (size_t)N,
           "size() should be 6");

        // push front checks
        check(bm.front_block()->front() == (N - 1) * 5,
           "front block should hold the last-pushed value (25)");
        check(bm.back_block()->front()  == 0,
           "back block should hold the first-pushed value (0)");

        // push back checks
        check(bmb.back_block()->front() == (N - 1) * 5,
           "back block should hold the last-pushed value (25)");
        check(bmb.front_block()->front()  == 0,
           "front block should hold the first-pushed value (0)");
    });

    // -- Test 10: destructor / memory stress -------------------
    // 15000 rounds x 500 blocks each.
    // A missing ~BlockMap leaks ~240 MB total, triggering std::bad_alloc.
    run_test("Destructor / memory stress", 2, [](){
        // Sanity check: push_back_block must work for this test to be meaningful.
        {
            BlockMap sanity;
            Block* b = new Block(); b->push_back(1);
            sanity.push_back_block(b);
            if (sanity.size() != 1)
                throw std::runtime_error(
                    "push_back_block not working - cannot verify destructor");
            // ~sanity: must delete b and slots array
        }
        for (int round = 0; round < 15000; round++) {
            BlockMap bm;
            for (int i = 0; i < 500; i++) {
                Block* b = new Block();
                b->push_back(i % 100);
                bm.push_back_block(b);
            }
            // ~bm: must delete all 500 Block objects and the slots array
        }
    });

    // -- Final score ------------------------------------------
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Score: " << g_earned << " / " << g_total << std::endl;
    std::cout << "========================================" << std::endl;
    return (g_earned == g_total) ? 0 : 1;
}
