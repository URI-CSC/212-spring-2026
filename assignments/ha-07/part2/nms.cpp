// ============================================================
// Assignment 07 - Part 2: NMS
// nms.cpp  --  YOUR WORK GOES HERE
// ============================================================
// Compile:
//   g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp test_nms.cpp -o test_nms
//   ./test_nms
//
// Do NOT modify nms.h or test_nms.cpp.
// ============================================================
//
// THE PROBLEM
//
// A detector fires hundreds of candidate boxes per object.  Most are
// near-duplicates.  You must return ONE box per distinct object -- the
// one with the highest confidence score.
//
// WHAT MAKES TWO BOXES "THE SAME OBJECT"?
//
// Two boxes describe the same object when IoU > IOU_THRESHOLD (strictly
// greater than 0.50).  The relationship is TRANSITIVE: if A overlaps B
// and B overlaps C, all three are the same object even if A and C do not
// overlap directly.  You must find CONNECTED COMPONENTS in the overlap graph.
//
// ============================================================
// THE THREE-PHASE ALGORITHM
//
// Phase 1 -- Sort by score (descending).
//   Use std::priority_queue<BBox> to pull all candidates out in
//   descending score order into a flat array sorted[0..n-1].
//   sorted[0] is the most confident candidate.
//
// Phase 2 -- Build the overlap graph.
//   Call build_overlap_graph(sorted).  The result is an adjacency list:
//     adj[i] = set of indices j where IoU(sorted[i], sorted[j]) > threshold.
//   Every index 0..n-1 has an entry (even if its neighbor set is empty).
//
//   adj[i] is an unordered_set<int>, so:
//     adj[i].insert(j)  -- O(1) amortised
//     adj[i].count(k)   -- O(1) membership test  (vs O(n) for vector)
//
// Phase 3 -- Find connected components.
//   Input:  sorted[0..n-1] and adj from Phase 2.
//   Output: survivors -- one winner per connected component.
//
//   Walk sorted[] left to right.  The first ungrouped index i you encounter
//   holds the highest-scoring box not yet assigned to any component -- it is
//   automatically the WINNER.  Add sorted[i] to survivors EXACTLY ONCE, here.
//
//   Flood-fill the rest of the component with a QUEUE (nms_queue) or STACK
//   (nms_stack).  Boxes reached during the flood-fill are SUPPRESSED: marked
//   grouped so the outer loop skips them, but never added to survivors.
//
//     Mark i as grouped.  Add sorted[i] to survivors.   // i is the winner
//     Place i in the frontier.
//     While the frontier is not empty:
//       Remove one index j from the frontier.
//       For each neighbor k in adj[j]:
//         If k is not yet grouped:
//           mark k as grouped           // k is suppressed, not a winner
//           add k to the frontier       // explore k's neighbors next
//
// QUEUE vs STACK
//
//   Queue (FIFO): expands the component outward wave by wave -- BFS.
//   Stack (LIFO): follows each overlap chain as deep as it goes -- DFS.
//   Both find the same components and return the same survivors.
//   The order in which boxes WITHIN a component are visited differs.
//
// WORKED EXAMPLE -- transitive chain
// (A overlaps B, B overlaps C, A and C do NOT directly overlap):
//
//   sorted = [A(0.92), B(0.81), C(0.74)]
//   adj[0]={1}  adj[1]={0,2}  adj[2]={1}
//
//   -- QUEUE (BFS) --              -- STACK (DFS) --
//   i=0: winner=A. Q=[0]           i=0: winner=A. S=[0]
//   dequeue 0: visit adj[0]={1}    pop 0: visit adj[0]={1}
//     1 ungrouped -> enqueue 1       1 ungrouped -> push 1
//   Q=[1]                           S=[1]
//   dequeue 1: visit adj[1]={2}    pop 1: visit adj[1]={2}
//     (0 already grouped)             (0 already grouped)
//     2 ungrouped -> enqueue 2       2 ungrouped -> push 2
//   Q=[2]                           S=[2]
//   dequeue 2: adj[2]={1} done     pop 2: adj[2]={1} done
//   Survivors: [A(0.92)]           Survivors: [A(0.92)]
//
// ============================================================

#include "nms.h"
#include <vector>
#include <queue>
#include <stack>

// build_overlap_graph  (10 pts)
// Iterate over all pairs (i, j) with i < j.
// If IoU(sorted[i], sorted[j]) > IOU_THRESHOLD:
//   adj[i].insert(j) and adj[j].insert(i)  (undirected edge).
// Ensure every index 0..n-1 has an entry in adj, even if its set is empty.
// Hint: default-construct adj[i] before the inner loop so that nodes with
// no neighbours still appear in the map.
std::unordered_map<int, std::unordered_set<int>>
build_overlap_graph(const std::vector<BBox>& sorted) {
    // TODO
    return std::unordered_map<int, std::unordered_set<int>>();
}

// nms_queue  (25 pts)
// Phase 1 -> Phase 2 -> Phase 3 using a std::queue<int> as the frontier.
std::vector<BBox> nms_queue(const std::vector<BBox>& candidates) {
    // TODO
    return std::vector<BBox>();
}

// ============================================================
// nms_stack  -- BONUS (15 pts)
// ============================================================
// Attempt this ONLY after nms_queue passes all required tests.
//
// The logic is identical to nms_queue.  The only change:
//   replace std::queue<int> with std::stack<int>
//   replace  .front() / .pop()  with  .top() / .pop()
//   replace  .push(k)           with  .push(k)   (same for stack)
//
// Both functions must return the same survivor count on every input.
// Use ./nms --compare ../images/000000191845.csv to verify.
// ============================================================
std::vector<BBox> nms_stack(const std::vector<BBox>& candidates) {
    // TODO (BONUS)
    return std::vector<BBox>();
}
