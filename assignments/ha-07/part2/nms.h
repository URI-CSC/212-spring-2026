#ifndef NMS_H
#define NMS_H

// ============================================================
// Assignment 07 - Part 2: NMS
// nms.h  --  provided, do not modify
// ============================================================

#include "bbox.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Two candidate boxes are considered to belong to the same object when
// their IoU strictly exceeds this threshold.
static const float IOU_THRESHOLD = 0.50f;

// ------------------------------------------------------------
// Phase 1 helper: build the overlap graph
// ------------------------------------------------------------
// Given a list of candidate boxes ALREADY SORTED by score descending,
// returns an adjacency list representing which pairs of boxes overlap.
//
// Return type: unordered_map<int, unordered_set<int>>
//   Key   = index i (0-based).
//   Value = SET of indices j where IoU(sorted[i], sorted[j]) > IOU_THRESHOLD
//           and j != i.
//
// Why unordered_set<int> instead of vector<int> for the neighbor list?
//   - Membership test: adj[i].count(j) is O(1) average vs O(degree) for vector.
//   - Duplicate edges are impossible by construction (set ignores re-inserts).
//   - Iteration with range-for works identically for both containers.
//
// The graph is UNDIRECTED: if j appears in adj[i], then i appears in adj[j].
// Every index 0..n-1 has an entry in the map (even if its neighbor set is
// empty) so that traversal code can safely call adj.at(i) for any valid i.
std::unordered_map<int, std::unordered_set<int>>
    build_overlap_graph(const std::vector<BBox>& sorted);

// ------------------------------------------------------------
// Phase 2: NMS via graph traversal
// ------------------------------------------------------------
// Given a flat list of candidate boxes (same category), returns one
// representative per connected component in the overlap graph.
// The representative is the box with the HIGHEST score in its component.
//
// nms_queue uses a QUEUE (FIFO) as its traversal frontier -> BFS.
// nms_stack uses a STACK (LIFO) as its traversal frontier -> DFS.
//
// Both functions:
//   1. Sort candidates by score descending.
//   2. Call build_overlap_graph on the sorted array.
//   3. Walk sorted[] left to right; the first ungrouped box is the winner.
//      Expand its component through the adjacency list using queue or stack.

std::vector<BBox> nms_queue(const std::vector<BBox>& candidates);
std::vector<BBox> nms_stack(const std::vector<BBox>& candidates);

#endif // NMS_H
