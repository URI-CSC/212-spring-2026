// ============================================================
// Assignment 07 - Part 2: Query functions  (13 pts)
// query.cpp  --  YOUR WORK GOES HERE
// ============================================================
// Compile (part of the pipeline build):
//   g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp csv.cpp query.cpp \
//       test_pipeline.cpp -o test_pipeline
//
// Do NOT modify query.h or test_pipeline.cpp.
// ============================================================
//
// FOUR QUERY FUNCTIONS, TWO CONTAINER CLASSES
//
// The functions below all operate on a flat vector of NMS survivors.
// Your task is to choose and use the right container inside each function.
//
// ---- HASH TABLE  (unordered_map) ----
// best_per_category
//   Walk survivors.  For each box, update unordered_map<int, BBox>:
//   if the category is absent, add it; if present and new score is higher,
//   replace.  O(n) build, O(1) lookup.
//
// ---- RED-BLACK TREE  (multimap, map) ----
// top_k_global
//   Insert all survivors into multimap<float, BBox, greater<float>>.
//   Walk from begin(), collecting up to k entries.
//   The RB-tree is already sorted -- no std::sort needed.
//   O(n log n) build, O(k) walk.
//
// find_by_name
//   Build map<string, int> from all 91 COCO labels (category_names.h).
//   Walk it; for each entry whose key contains substr, collect survivors
//   with that category_id.
//   Why map and not unordered_map?  For substring search both require a
//   full scan, but map is the better habit when key ORDER matters.
//   For prefix search, map::lower_bound gives O(log n + k) -- a hash
//   map cannot do better than O(n) for prefix queries.
//
// score_range
//   Insert all survivors into multimap<float, BBox, greater<float>>.
//   The multimap is stored DESCENDING (greater<float>), so:
//     lower_bound(hi) -> first entry with key <= hi  (start of range)
//     upper_bound(lo) -> first entry with key <  lo  (end, exclusive)
//   Walk the half-open range [lower_bound(hi), upper_bound(lo)).
//   This is the same iterator trick shown in SpatialIndex::score_range
//   in index.cpp -- re-read that worked example if you get stuck.
//   O(n log n) build, O(log n + k) query.  Compare to O(n) linear scan.
// ============================================================

#include "query.h"
#include "category_names.h"
#include <map>

// best_per_category  (5 pts)
std::unordered_map<int, BBox>
best_per_category(const std::vector<BBox>& survivors) {
    // TODO
    return std::unordered_map<int, BBox>();
}

// top_k_global  (3 pts)
std::vector<BBox>
top_k_global(const std::vector<BBox>& survivors, size_t k) {
    // TODO
    (void)k;
    return std::vector<BBox>();
}

// find_by_name  (2 pts)
std::vector<BBox>
find_by_name(const std::vector<BBox>& survivors, const std::string& substr) {
    // TODO
    (void)substr;
    return std::vector<BBox>();
}

// score_range  (3 pts)
// Return all survivors with score in [lo, hi] in descending score order.
//
// Steps:
//   1. Insert all survivors into a multimap<float, BBox, greater<float>>.
//   2. Use lower_bound(hi) and upper_bound(lo) to find the range.
//   3. Walk [lower_bound(hi), upper_bound(lo)) and push each value.
//
// See SpatialIndex::score_range in index.cpp for a fully commented
// version of this same pattern.
std::vector<BBox>
score_range(const std::vector<BBox>& survivors, float lo, float hi) {
    // TODO
    (void)lo; (void)hi;
    return std::vector<BBox>();
}
