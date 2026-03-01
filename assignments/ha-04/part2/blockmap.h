#ifndef BLOCKMAP_H
#define BLOCKMAP_H

#include "block.h"

// ============================================================
// BlockMap
// ============================================================
// BlockMap manages a dynamic, ordered sequence of Block* pointers
// using a circular array as its backing store.
//
// Ownership: BlockMap owns every Block it contains.
//   push_front_block / push_back_block: BlockMap takes ownership.
//   pop_front_block  / pop_back_block:  BlockMap deletes the Block.
//   ~BlockMap: deletes all remaining Blocks, then deletes slots[].
// ============================================================

class BlockMap {
private:
    Block** slots;     // circular array of Block* pointers
    size_t  capacity;  // total number of slots allocated
    size_t  count;     // number of active Block pointers
    size_t  front_idx; // physical index of logical slot 0

    // Returns the physical array index of the back block.
    // Requires: !empty()
    size_t back_idx() const;

    // Doubles the capacity, preserving logical block order.
    // Called automatically by push_*_block when full.
    void resize();

public:
    // Constructs an empty BlockMap with the given initial capacity.
    // All slots are initialized to nullptr.
    BlockMap(size_t initial_capacity = 8);

    // Destructor: deletes all remaining Block objects, then deletes slots[].
    ~BlockMap();

    // '= delete' tells the compiler to reject any attempt to copy a BlockMap.
    // Trying to write:  BlockMap b = a;  or  b = a;  will produce a compile error.
    //
    // Why: BlockMap owns heap memory (slots[] and every Block inside it). A
    // compiler-generated copy would do a shallow copy - both objects would share
    // the same pointers, and when both destructors run, every Block would be
    // deleted twice, causing a crash.
    //
    // The correct fix is the Rule of Three: write a deep-copy constructor and a
    // deep-copy assignment operator that duplicate every Block. We are deliberately
    // skipping that here because it adds significant complexity to an already
    // non-trivial class. In a real production codebase this would be required.
    BlockMap(const BlockMap&)            = delete;
    BlockMap& operator=(const BlockMap&) = delete;

    // Returns the number of active blocks.
    size_t size() const;

    // Returns true if no blocks are active.
    bool empty() const;

    // Returns true if there is no room to add a block without resizing.
    bool full() const;

    // Returns a pointer to the front Block.
    // Requires: !empty()
    Block* front_block() const;

    // Returns a pointer to the back Block.
    // Requires: !empty()
    Block* back_block() const;

    // Returns a pointer to the Block at logical position i (0 = front).
    // Requires: i < size()
    Block* get_block(size_t i) const;

    // Inserts b at the logical front of the map (BlockMap takes ownership).
    // Calls resize() if full.
    void push_front_block(Block* b);

    // Appends b at the logical back of the map (BlockMap takes ownership).
    // Calls resize() if full.
    void push_back_block(Block* b);

    // Removes and DELETES the front Block.
    // Requires: !empty()
    void pop_front_block();

    // Removes and DELETES the back Block.
    // Requires: !empty()
    void pop_back_block();
};

#endif
