#ifndef QUERY_H
#define QUERY_H

// ============================================================
// Assignment 07 - Part 2: Query functions
// query.h  --  provided, do not modify
// ============================================================

#include "bbox.h"
#include <vector>
#include <string>
#include <unordered_map>

// Given a flat list of NMS survivors (any order), returns a map from
// category_id to the single best (highest-scoring) survivor in that
// category.
//
// Container: unordered_map<int, BBox>  (HASH TABLE)
//   Walk survivors; for each box, update the entry if this box has a
//   higher score.  O(n) build time, O(1) amortised lookup after that.
//   Use when callers need fast per-category access.
std::unordered_map<int, BBox>
    best_per_category(const std::vector<BBox>& survivors);

// Returns up to k survivors with the highest scores across ALL categories,
// in strictly descending order.  Returns all survivors if size <= k.
//
// Container: multimap<float, BBox, greater<float>>  (RED-BLACK TREE)
//   Insert all survivors in O(n log n); walk from begin() in O(k).
//   The RB-tree keeps entries sorted, so no separate sort step is needed.
std::vector<BBox>
    top_k_global(const std::vector<BBox>& survivors, size_t k);

// Returns every survivor whose category name CONTAINS substr
// (case-sensitive substring match).
//
// Container: map<string, int>  (RED-BLACK TREE keyed by name)
//   Build a map of all 91 COCO labels.  Walk it; for each entry whose
//   name contains substr, collect the matching survivors.
//   map (not unordered_map) is the better habit here because key
//   ordering matters when you want prefix or range queries.
//
// Example: find_by_name(survivors, "sport") matches "sports ball" (id 37).
std::vector<BBox>
    find_by_name(const std::vector<BBox>& survivors, const std::string& substr);

// Returns every survivor with score in [lo, hi] (inclusive),
// in descending score order.
//
// Container: multimap<float, BBox, greater<float>>  (RED-BLACK TREE)
//   Insert all survivors.  Use lower_bound(hi) and upper_bound(lo)
//   to jump directly to the score boundaries in O(log n), then walk
//   the O(k) matching entries.  Total: O(n log n + k).
//
//   A linear scan through a vector would cost O(n) -- the same order,
//   but without the O(k) walk advantage when k << n.  More importantly,
//   if the index were persistent (built once, queried many times),
//   the O(log n) query cost would amortise over many calls.
//
// This is the same range-query pattern as SpatialIndex::score_range
// in Part 1 -- same data structure, different context.
std::vector<BBox>
    score_range(const std::vector<BBox>& survivors, float lo, float hi);

#endif // QUERY_H
