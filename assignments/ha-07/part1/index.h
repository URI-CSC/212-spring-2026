#ifndef INDEX_H
#define INDEX_H

// ============================================================
// Assignment 07 - Part 1: SpatialIndex
// index.h  --  provided, do not modify
// ============================================================

#include "bbox.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>   // std::greater

// SpatialIndex stores bounding boxes under two access patterns
// simultaneously, with each container chosen deliberately:
//
//   by_category  --  unordered_map<int, vector<BBox>>
//                    Backed by a HASH TABLE.
//                    Key = category_id.  Value = all boxes for that id.
//                    Insert: O(1) amortised.
//                    Lookup: O(1) amortised.
//                    Use when: "give me all boxes for category 3."
//
//   by_score     --  multimap<float, BBox, greater<float>>
//                    Backed by a RED-BLACK TREE.
//                    Entries kept in DESCENDING score order.
//                    begin() always points to the highest-scoring box.
//                    Insert: O(log n).  Walk k entries: O(k).
//                    Range query by score: O(log n + k) using lower_bound /
//                    upper_bound -- only possible with an ordered tree.
//                    Use when: "give me the 5 most confident detections"
//                    or "give me all detections with score in [0.4, 0.8]."
//
// Every call to insert(b) must update BOTH containers.
// The two containers hold independent copies of each box (no aliasing).
//
// Container design lesson:
//   The hash table gives O(1) category lookup but has NO ordering.
//   The RB-tree gives O(log n) operations but supports range queries and
//   sorted iteration that a hash map cannot provide efficiently.
//   Neither container alone serves both query patterns well.

class SpatialIndex {
    std::unordered_map<int, std::vector<BBox>> by_category;
    std::multimap<float, BBox, std::greater<float>> by_score;

public:
    // Constructs an empty index.
    SpatialIndex();

    // Inserts b into both by_category and by_score.
    void insert(const BBox& b);

    // Returns the vector of boxes stored under cat_id.
    // Precondition: has_category(cat_id) == true.
    // Behaviour is undefined (may throw) if cat_id is absent.
    const std::vector<BBox>& get_category(int cat_id) const;

    // Returns true iff at least one box with cat_id was inserted.
    bool has_category(int cat_id) const;

    // Returns a vector containing every distinct category_id inserted.
    // Order is unspecified (hash map iteration order is undefined).
    std::vector<int> category_ids() const;

    // Returns up to k boxes with the highest scores, descending.
    // If total() <= k, returns all boxes.
    // Iterates the RB-tree (by_score) from begin() -- no sort needed.
    std::vector<BBox> top_k(size_t k) const;

    // Returns every box whose score falls in [lo, hi] (inclusive),
    // in descending score order.
    //
    // Implementation hint:
    //   by_score uses greater<float>, so larger scores sort BEFORE smaller
    //   ones.  That means:
    //     lower_bound(hi) points to the first entry with score <= hi
    //     upper_bound(lo) points to the first entry with score < lo
    //   Walk the half-open range [lower_bound(hi), upper_bound(lo)).
    //
    // Why is this O(log n + k) while a linear scan would be O(n)?
    // Because the RB-tree lets you jump directly to the boundary in
    // O(log n) -- that is the core advantage of an ordered structure.
    std::vector<BBox> score_range(float lo, float hi) const;

    // Total number of boxes inserted (across all categories).
    size_t total() const;

    // Number of distinct category_ids currently in the index.
    size_t num_categories() const;
};

#endif // INDEX_H
