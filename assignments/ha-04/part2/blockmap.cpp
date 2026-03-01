#include "blockmap.h"

// ============================================================
// BlockMap - Implementation
// ============================================================
// BlockMap stores Block pointers in a circular array.
// front_idx is the physical position of logical slot 0.
// Logical-to-physical index mapping is handled by the circular array structure.
// ============================================================

// Constructor: allocate the slots array; initialize to empty.
BlockMap::BlockMap(size_t initial_capacity)
    : capacity(initial_capacity), count(0), front_idx(0) {
    // TODO: allocate slots and initialize all entries to nullptr
    slots = nullptr; // placeholder
}

// Destructor: release all owned Block objects, then the slots array.
BlockMap::~BlockMap() {
    // TODO: delete every live Block, then free slots
}

// Returns the physical array index of the back block.
// Requires: size() > 0
size_t BlockMap::back_idx() const {
    // TODO: return the physical index of the back block
    (void)slots; (void)front_idx; (void)count; (void)capacity; // suppress unused-field warnings, remove this line when you implement the function
    return 0; // placeholder
}

// Returns the number of active Block pointers.
size_t BlockMap::size() const {
    // TODO: implement
    return 0;
}

// Returns true if there are no active blocks.
bool BlockMap::empty() const {
    // TODO: implement
    return true;
}

// Returns true if every slot is occupied.
bool BlockMap::full() const {
    // TODO: implement
    return false;
}

// Returns the Block pointer at the logical front.
// Requires: !empty()
Block* BlockMap::front_block() const {
    // TODO: implement
    return nullptr;
}

// Returns the Block pointer at the logical back.
// Requires: !empty()
Block* BlockMap::back_block() const {
    // TODO: implement
    return nullptr;
}

// Returns the Block pointer at logical index i (0 = front).
// Requires: i < count
Block* BlockMap::get_block(size_t i) const {
    // TODO: implement
    return nullptr;
}

// Doubles capacity, preserving all blocks in logical order.
void BlockMap::resize() {
    // TODO: implement
}

// Appends b at the logical back. Takes ownership of b.
void BlockMap::push_back_block(Block* b) {
    // TODO: grow if needed, then insert b at the back
}

// Inserts b at the logical front. Takes ownership of b.
void BlockMap::push_front_block(Block* b) {
    // TODO: grow if needed, then insert b at the front
}

// Removes and deletes the front Block.
// Requires: !empty()
void BlockMap::pop_front_block() {
    // TODO: delete the front Block and update the internal bookkeeping
}

// Removes and deletes the back Block.
// Requires: !empty()
void BlockMap::pop_back_block() {
    // TODO: delete the back Block and update count
}
