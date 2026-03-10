// ============================================================
// test_memory_safety.cpp  -  Memory-safety focused tests for Part 2
// ============================================================
//
// Runs the Part 2 implementation under AddressSanitizer, LeakSanitizer,
// and UndefinedBehaviorSanitizer to catch memory bugs that functional
// tests miss: double-free, use-after-free, heap buffer overflow,
// size_t underflow, and memory leaks in the destructor and resize().
//
// PURPOSE
//   This file targets memory bugs that are easy to miss but are
//   caught reliably by AddressSanitizer (ASan) and
//   UndefinedBehaviorSanitizer (UBSan):
//
//     MEM-1  Destructor does not call delete on live blocks.
//            → ASan leak report; or OOM on stress test.
//     MEM-2  pop_*_block deletes the block but does NOT update
//            front_idx / count correctly, so the destructor
//            visits the same freed pointer again.
//            → ASan heap-use-after-free / double-free.
//     MEM-3  Destructor forgets delete[] slots.
//            → ASan leak report.
//     MEM-4  resize() forgets delete[] old_slots.
//            → ASan leak; OOM after many resizes.
//     MEM-5  push_back writes past data[BLOCK_SIZE-1].
//            → ASan heap-buffer-overflow.
//     MEM-6  push_front decrements head when head==0 → size_t
//            wraps to SIZE_MAX, then data[SIZE_MAX] is written.
//            → UBSan unsigned-integer-overflow + ASan OOB write.
//     MEM-7  at() computes head+i without checking head+i < BLOCK_SIZE
//            → ASan OOB read past the Block's allocation.
//     MEM-8  pop_back / pop_front empties a block but does not
//            remove it from the BlockMap, leaving a stale empty
//            block as front/back → subsequent ops use wrong block.
//     MEM-9  Deque destructor only works via ~BlockMap; if
//            ~BlockMap is broken the Deque leaks everything.
//     MEM-10 pop_back / pop_front on an empty Deque calls
//            bmap.back_block() before the empty() guard fires,
//            dereferencing a null/garbage pointer.
//            → ASan / segfault.
//
// HOW TO COMPILE
//   With sanitizers (strongly recommended):
//     g++ -std=c++11 -g -fsanitize=address,leak,undefined -fno-omit-frame-pointer
//         block.cpp blockmap.cpp deque.cpp test_memory_safety.cpp 
//         -o test_memory_safety
//     ./test_memory_safety
//
//   Without sanitizers (stress tests still catch gross leaks via OOM):
//     g++ -std=c++11 -Wall -Werror 
//         block.cpp blockmap.cpp deque.cpp test_memory_safety.cpp 
//         -o test_memory_safety
//     ./test_memory_safety
// ============================================================

#include "block.h"
#include "blockmap.h"
#include "deque.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

// ---- tiny harness (same style as the provided test files) ----
static int g_earned = 0;
static int g_total  = 0;

static void check(bool cond, const char* msg = "condition failed") {
    if (!cond) throw std::runtime_error(msg);
}

static void run_test(const std::string& name, int pts,
                     const std::function<void()>& fn) {
    g_total += pts;
    try {
        fn();
        g_earned += pts;
        std::cout << "  [" << pts << "/" << pts << "] "
                  << name << ": PASSED\n";
    } catch (const std::exception& e) {
        std::cout << "  [0/" << pts << "] "
                  << name << ": FAILED (" << e.what() << ")\n";
    }
}

// ============================================================
int main() {
    std::cout << "=== Memory Safety Tests – Part 2 ===\n";
    std::cout << "(Run with -fsanitize=address,undefined for full coverage)\n\n";

    // ----------------------------------------------------------
    // SECTION 1 – BlockMap destructor  (catches MEM-1, MEM-2, MEM-3)
    // ----------------------------------------------------------
    std::cout << "-- Section 1: BlockMap destructor --\n";

    // Destroy a non-empty BlockMap (never popped).
    // MEM-1: missing delete on blocks → ASan leak / OOM.
    // MEM-3: missing delete[] slots   → ASan leak.
    run_test("Destructor – non-empty, no pops", 2, [](){
        {
            BlockMap bm;
            for (int i = 0; i < 8; i++) {
                Block* b = new Block();
                b->push_back(i);
                bm.push_back_block(b);
            }
            check(bm.size() == 8, "size should be 8 before destroy");
            // ~bm here: must delete all 8 blocks and the slots array.
        }
        check(true, "destructor ran without crashing");
    });

    // Destroy an empty BlockMap.
    // MEM-3: delete[] slots must still run even when count == 0.
    run_test("Destructor – empty map", 1, [](){
        { BlockMap bm; check(bm.empty(), "should be empty"); }
        check(true, "empty-map destructor ran without crashing");
    });

    // pop_front then let the destructor handle the rest.
    // MEM-2: if pop_front deletes the block but does NOT advance
    //        front_idx, the destructor will visit the freed pointer
    //        again → ASan double-free.
    run_test("Destructor after pop_front – double-free probe", 3, [](){
        {
            BlockMap bm(4);
            Block* b0 = new Block(); b0->push_back(0); bm.push_back_block(b0);
            Block* b1 = new Block(); b1->push_back(1); bm.push_back_block(b1);
            Block* b2 = new Block(); b2->push_back(2); bm.push_back_block(b2);
            // front_idx=0, count=3
            bm.pop_front_block();
            // Correct: front_idx → 1, count → 2, b0 deleted once.
            // Bug:     front_idx stays 0, destructor re-deletes b0 → ASan.
            check(bm.size() == 2, "size should be 2 after pop");
            check(bm.front_block()->front() == 1, "front value should be 1");
            // ~bm: must delete b1 and b2 exactly once.
        }
        check(true, "no double-free detected");
    });

    // pop_back then destroy – same probe for the back pointer.
    run_test("Destructor after pop_back – double-free probe", 3, [](){
        {
            BlockMap bm(4);
            Block* b0 = new Block(); b0->push_back(10); bm.push_back_block(b0);
            Block* b1 = new Block(); b1->push_back(20); bm.push_back_block(b1);
            Block* b2 = new Block(); b2->push_back(30); bm.push_back_block(b2);
            bm.pop_back_block();
            // Bug: count not decremented; destructor sees count=3 but only
            //      2 valid blocks; third slot may be freed or garbage → ASan.
            check(bm.size() == 2, "size should be 2 after pop_back");
            check(bm.back_block()->front() == 20, "back value should be 20");
        }
        check(true, "no double-free detected");
    });

    // Drain all blocks then destroy.
    // Destructor must NOT re-delete already-freed blocks (count == 0)
    // but MUST still free the slots array (MEM-3).
    run_test("Destructor after full drain – no double-free, no leak", 3, [](){
        {
            BlockMap bm(4);
            for (int i = 0; i < 4; i++) {
                Block* b = new Block(); b->push_back(i); bm.push_back_block(b);
            }
            for (int i = 0; i < 4; i++) bm.pop_front_block();
            check(bm.empty(), "should be empty after full drain");
            // ~bm: count==0, no blocks to delete, but slots array must be freed.
        }
        check(true, "drain-then-destroy ran cleanly");
    });

    // ----------------------------------------------------------
    // SECTION 2 – resize() does not leak the old slots array  (MEM-4)
    // ----------------------------------------------------------
    std::cout << "\n-- Section 2: resize() does not leak old slots array --\n";

    // Each resize that forgets delete[] old_slots wastes:
    //   old_capacity * sizeof(Block*) bytes.
    // 1 000 rounds × 512 blocks, with initial_capacity=2:
    //   rounds of resize: log2(512/2) = 8 resizes × 8B × 2^1..2^8 ≈ 4 KB/round
    //   × 1 000 rounds ≈ 4 MB cumulative → OOM around round 50 without sanitizer.
    // With ASan+LSan: definite leak reported on every round.
    run_test("resize leak stress (1 000 rounds × 512 blocks)", 3, [](){
        for (int round = 0; round < 1000; round++) {
            BlockMap bm(2);  // tiny capacity forces many resizes
            for (int i = 0; i < 512; i++) {
                Block* b = new Block();
                b->push_back(i % 100);
                bm.push_back_block(b);
            }
            check(bm.size() == 512, "size should be 512");
            // ~bm: all 512 blocks freed, and every old slots array from each
            // doubling must also be freed (the final slots array is freed too).
        }
        check(true, "no OOM from resize leaks");
    });

    // ----------------------------------------------------------
    // SECTION 3 – Block buffer overflow  (MEM-5, MEM-6, MEM-7)
    // ----------------------------------------------------------
    std::cout << "\n-- Section 3: Block buffer overflow / OOB access --\n";

    // MEM-5: push_back into a full block writes past data[BLOCK_SIZE-1].
    // This test only pushes into a non-full block; ASan confirms no OOB.
    run_test("push_back fills to BLOCK_SIZE without OOB write", 2, [](){
        Block b;
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
            check(b.can_push_back(), "should have room");
            b.push_back((int)i * 10);
        }
        check(!b.can_push_back(), "block should be full");
        check(b.size() == BLOCK_SIZE, "size should equal BLOCK_SIZE");
        // ASan: if push_back wrote data[BLOCK_SIZE] or beyond, it fires here.
        for (size_t i = 0; i < BLOCK_SIZE; i++)
            check(b.at(i) == (int)i * 10, "at(i) value mismatch");
    });

    // MEM-6: push_front with head==0 should be prevented by can_push_front().
    // If push_front decrements head before checking, head wraps to SIZE_MAX.
    // UBSan catches the unsigned-integer underflow; ASan catches the write.
    run_test("push_front fills from BLOCK_SIZE without size_t underflow", 2, [](){
        Block b(BLOCK_SIZE, BLOCK_SIZE);
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
            check(b.can_push_front(), "should have room at front");
            b.push_front((int)i);
        }
        check(!b.can_push_front(), "block should be full at front");
        check(b.size() == BLOCK_SIZE, "size should equal BLOCK_SIZE");
        // Logical order: push_front(0) first → sits at back of the live window.
        // Values in window from front: BLOCK_SIZE-1, BLOCK_SIZE-2, ..., 0
        for (size_t i = 0; i < BLOCK_SIZE; i++)
            check(b.at(i) == (int)(BLOCK_SIZE - 1 - i),
                  "front-to-back order incorrect");
        // UBSan: if any push_front decremented head past 0, this crashes.
    });

    // MEM-7: at(i) must throw for i >= size() and must NOT read OOB memory.
    // An incorrect implementation might compute data[head + i] without
    // checking head + i < BLOCK_SIZE first.
    run_test("at() in-bounds is safe; at() OOB throws, not silently reads OOB", 2, [](){
        Block b;
        b.push_back(100);
        b.push_back(200);
        // size()==2 → valid indices: 0 and 1 only.
        check(b.at(0) == 100, "at(0) should be 100");
        check(b.at(1) == 200, "at(1) should be 200");

        // at(2) is out of range: must throw, not access data[head+2].
        bool threw = false;
        try { (void)b.at(2); } catch (const std::out_of_range&) { threw = true; }
        check(threw, "at(2) must throw out_of_range, not access OOB memory");

        // at(SIZE_MAX) with a size_t argument — also must throw.
        threw = false;
        try { (void)b.at((size_t)-1); } catch (const std::out_of_range&) { threw = true; }
        check(threw, "at(SIZE_MAX) must throw out_of_range");
        // ASan: if either at() call read past the block's allocation, it fires.
    });

    // ----------------------------------------------------------
    // SECTION 4 – Deque memory safety  (MEM-8, MEM-9, MEM-10)
    // ----------------------------------------------------------
    std::cout << "\n-- Section 4: Deque memory safety --\n";

    // MEM-8: when pop_back / pop_front empties a Block, that Block must be
    // removed from the BlockMap (which deletes it).  If it is not removed:
    // (a) it leaks, and (b) subsequent push operations use a stale empty
    // block instead of allocating a new one, producing wrong results.
    run_test("pop empties a Block – empty Block is removed from BlockMap", 3, [](){
        Deque dq;
        // Fill exactly one block.
        for (int i = 0; i < (int)BLOCK_SIZE; i++) dq.push_back(i);
        check(dq.size() == BLOCK_SIZE, "size should equal BLOCK_SIZE");

        // Pop all elements – the block should be deleted by BlockMap.
        for (int i = 0; i < (int)BLOCK_SIZE; i++) dq.pop_back();
        check(dq.empty(), "should be empty after popping all");

        // Push again: must allocate a fresh block, not reuse the deleted one.
        for (int i = 10; i < 10 + (int)BLOCK_SIZE; i++) dq.push_back(i);
        check(dq.size()  == BLOCK_SIZE, "size should be BLOCK_SIZE again");
        check(dq.front() == 10,                       "front should be 10");
        check(dq.back()  == 10 + (int)BLOCK_SIZE - 1, "back should be last value");
        // ASan: use-after-free if push reused a deleted block's address.
    });

    // MEM-9: Deque destructor relies entirely on ~BlockMap.
    // 2 000 rounds × 5 000 elements.  If ~BlockMap leaks,
    // we run out of memory around round 50.
    run_test("Deque destructor stress (2 000 rounds × 5 000 elements)", 3, [](){
        for (int round = 0; round < 2000; round++) {
            Deque dq;
            for (int i = 0; i < 5000; i++) {
                if (i % 3 == 0) dq.push_front(i);
                else             dq.push_back(i);
            }
            check(dq.size() == 5000, "size should be 5000");
            // ~dq → ~BlockMap: all blocks and slots array freed.
        }
        check(true, "no OOM");
    });

    // MEM-10: pop_back/pop_front must check empty() BEFORE touching the
    // BlockMap, otherwise bmap.back_block() / bmap.front_block() returns
    // nullptr (or garbage) and dereferencing it is UB / ASan crash.
    run_test("pop on empty Deque throws, does not dereference null", 2, [](){
        Deque dq;
        bool caught_back = false, caught_front = false;
        try { dq.pop_back();  } catch (const std::out_of_range&) { caught_back  = true; }
        try { dq.pop_front(); } catch (const std::out_of_range&) { caught_front = true; }
        check(caught_back,  "pop_back on empty must throw out_of_range");
        check(caught_front, "pop_front on empty must throw out_of_range");
        // ASan: if pop called bmap.back_block() before checking empty(), and
        // back_block() returned nullptr, then back_block()->back() crashes.
    });

    // ----------------------------------------------------------
    // SECTION 5 – Circular wrap + resize interaction
    // ----------------------------------------------------------
    // After front_idx has wrapped (non-zero offset) and then a resize
    // is triggered, resize() must copy blocks in logical order, not raw
    // slot order.  A buggy copy produces wrong values AND may leave old
    // block pointers dangling in the new array (UAF / double-free).
    std::cout << "\n-- Section 5: Circular wrap + resize --\n";

    run_test("Circular wrap + resize: no crash, no double-free, no leak", 3, [](){
        {
            BlockMap bm(4);  // capacity=4

            // Fill slots 0-3; front_idx=0, count=4 (full)
            for (int i = 0; i < 4; i++) {
                Block* b = new Block(); b->push_back(i * 10); bm.push_back_block(b);
            }
            check(bm.full(), "should be full");

            // Drain all; front_idx wraps back to 0, count=0
            for (int i = 0; i < 4; i++) bm.pop_front_block();
            check(bm.empty(), "should be empty after drain");

            // Re-fill from the current (potentially non-zero) front_idx.
            for (int i = 0; i < 4; i++) {
                Block* b = new Block(); b->push_back(100 + i); bm.push_back_block(b);
            }
            // Prepend 4 more from the front – will trigger resize.
            for (int i = 0; i < 4; i++) {
                Block* b = new Block(BLOCK_SIZE, BLOCK_SIZE);
                b->push_front(200 + i);
                bm.push_front_block(b);
            }
            check(bm.size() == 8, "size should be 8");
            check(bm.front_block()->front() == 203, "logical front should be 203");
            check(bm.back_block()->front()  == 103, "logical back should be 103");
            // ~bm: all 8 blocks deleted exactly once; old slots array freed.
        }
        check(true, "no memory errors detected");
    });

    // ----------------------------------------------------------
    // SECTION 6 – Interleaved push / pop cycling stress
    // ----------------------------------------------------------
    // Exercising the full Block lifecycle: allocate, use, delete via pop,
    // repeat.  Any accumulated leak or double-free surfaces as OOM or ASan.
    std::cout << "\n-- Section 6: Interleaved push/pop cycling stress --\n";

    run_test("Deque push/pop cycling (500 cycles)", 3, [](){
        Deque dq;
        // Each cycle: push BLOCK_SIZE+1 elements (forces a new block),
        // then pop BLOCK_SIZE (deletes the first block, leaving one element).
        for (int cycle = 0; cycle < 500; cycle++) {
            for (int i = 0; i < (int)BLOCK_SIZE + 1; i++)
                dq.push_back(cycle * 100 + i);
            for (int i = 0; i < (int)BLOCK_SIZE; i++)
                dq.pop_front();
        }
        // 500 cycles × 1 remaining element each = 500 elements
        check(dq.size() == 500, "500 elements should remain");
        while (!dq.empty()) dq.pop_front();
        check(dq.empty(), "should be empty after final drain");
        // ASan: any block that was freed by pop but still referenced
        // inside the Deque/BlockMap would be a UAF caught here.
    });

    // ----------------------------------------------------------
    std::cout << "\n========================================\n";
    std::cout << "  Score: " << g_earned << " / " << g_total << "\n";
    std::cout << "========================================\n";
    return (g_earned == g_total) ? 0 : 1;
}
