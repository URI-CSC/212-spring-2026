// ============================================================
// Assignment 07 - Part 1: SpatialIndex  (30 pts)
// index.cpp  --  YOUR WORK GOES HERE  (except score_range)
// ============================================================
// Compile and run with the autograder:
//   g++ -std=c++17 -Wall -Werror bbox.cpp index.cpp test_index.cpp -o test_index
//   ./test_index
//
// Do NOT modify index.h or test_index.cpp.
// ============================================================
//
// SpatialIndex stores boxes in two containers chosen for different
// query patterns:
//
//   by_category  (unordered_map<int, vector<BBox>>)
//     -- HASH TABLE: O(1) amortised lookup by category_id
//     -- "give me all boxes for category 1"
//
//   by_score  (multimap<float, BBox, greater<float>>)
//     -- RED-BLACK TREE sorted by score DESCENDING
//     -- begin() always points to the highest-scoring box
//     -- supports O(log n + k) range queries via lower_bound/upper_bound
//     -- multimap allows duplicate scores (unlike map, which would drop one)
//
// insert() must keep both containers in sync.
//
// THINK THROUGH THESE BEFORE YOU WRITE:
//   - What is the amortised time complexity of insert()?
//   - What is the time complexity of top_k(k)?
//   - Why can't an unordered_map support score_range efficiently?
// ============================================================

#include "index.h"

SpatialIndex::SpatialIndex() {
    // Both containers default-construct to empty.
}

// insert  (8 pts)
// Add b to by_category[b.category_id] and to by_score.
void SpatialIndex::insert(const BBox& b) {
    // TODO
    (void)b;
}

// get_category  (2 pts)
// Return the vector stored at by_category[cat_id].
// Use .at() -- it throws std::out_of_range if cat_id is absent,
// which is the correct behaviour when the precondition is violated.
const std::vector<BBox>& SpatialIndex::get_category(int cat_id) const {
    // TODO
    static std::vector<BBox> empty;
    (void)cat_id;
    return empty;
}

// has_category  (2 pts)
// Return true iff by_category contains an entry for cat_id.
bool SpatialIndex::has_category(int cat_id) const {
    // TODO
    (void)cat_id;
    return false;
}

// category_ids  (3 pts)
// Collect every key from by_category into a vector and return it.
// Order is unspecified (hash map iteration order is not defined).
std::vector<int> SpatialIndex::category_ids() const {
    // TODO
    return std::vector<int>();
}

// top_k  (7 pts)
// Walk by_score from begin() forward; push each box's value into
// the result vector until you have k elements or reach end().
// Because by_score uses greater<float>, begin() is the highest score.
std::vector<BBox> SpatialIndex::top_k(size_t k) const {
    // TODO
    (void)k;
    return std::vector<BBox>();
}

// score_range  -- PROVIDED AS A WORKED EXAMPLE, do not modify  (5 pts, awarded automatically if insert is correct)
//
// Goal: return all boxes with score in [lo, hi] in descending order.
//
// The trick: by_score uses greater<float>, so it is stored HIGH -> LOW.
// That flips the meaning of lower_bound and upper_bound:
//
//   lower_bound(hi) finds the first entry with key <= hi.
//     (In a normal ascending map, lower_bound finds the first key >= x.
//      With greater<float> the comparator is reversed, so it finds
//      the first key that is NOT greater-than hi -- i.e., key <= hi.)
//     This is the START of our range.
//
//   upper_bound(lo) finds the first entry with key < lo.
//     (In a normal ascending map, upper_bound finds the first key > x.
//      With greater<float> reversed, it finds the first key that is
//      strictly less-than lo.)
//     This is one-past-the-END of our range.
//
// Walking [lower_bound(hi), upper_bound(lo)) visits every entry whose
// score satisfies lo <= score <= hi, already in descending order.
//
// Complexity: O(log n) for each bound + O(k) to collect k results.
// A linear scan over an unordered container would cost O(n) regardless
// of how many results there are -- that is why we pay O(log n) at insert
// to keep the tree sorted.
std::vector<BBox> SpatialIndex::score_range(float lo, float hi) const {
    std::vector<BBox> result;

    // Step 1: jump to the first entry with score <= hi.
    auto start = by_score.lower_bound(hi);

    // Step 2: find the first entry with score < lo (exclusive end).
    auto stop  = by_score.upper_bound(lo);

    // Step 3: walk the half-open range [start, stop).
    // Every entry here satisfies lo <= score <= hi.
    // Because by_score is descending, the walk is already sorted high to low.
    for (auto it = start; it != stop; ++it) {
        result.push_back(it->second);
    }

    return result;
}

// total  (2 pts)
// Hint: every insert adds exactly one entry to by_score.
size_t SpatialIndex::total() const {
    // TODO
    return 0;
}

// num_categories  (1 pt)
size_t SpatialIndex::num_categories() const {
    // TODO
    return 0;
}
