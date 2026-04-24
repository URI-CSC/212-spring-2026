// ============================================================
// Assignment 07 - Part 2: NMS tests  (50 pts)
// test_nms.cpp  --  provided autograder, do not modify
// ============================================================
// Compile and run:
//   g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp test_nms.cpp -o test_nms
//   ./test_nms
// ============================================================

#include "bbox.h"
#include "nms.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

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
        std::cout << "  [" << pts << "/" << pts << "] " << name << ": PASSED\n";
    } catch (const std::exception& e) {
        std::cout << "  [0/" << pts << "] " << name << ": FAILED ("
                  << e.what() << ")\n";
    }
}

// ---- helpers -----------------------------------------------

static BBox box(float x, float y, float w, float h, float score, int cat = 0) {
    BBox b;
    b.image_id = 1; b.category_id = cat;
    b.x = x; b.y = y; b.w = w; b.h = h; b.score = score;
    return b;
}

static bool has_score(const std::vector<BBox>& v, float t, float eps = 1e-4f) {
    for (size_t i = 0; i < v.size(); i++)
        if (std::fabs(v[i].score - t) < eps) return true;
    return false;
}

static void check_nms_invariant(const std::vector<BBox>& s, const char* label) {
    for (size_t i = 0; i < s.size(); i++)
        for (size_t j = i + 1; j < s.size(); j++)
            if (s[i].iou(s[j]) > IOU_THRESHOLD + 1e-5f)
                throw std::runtime_error(
                    std::string(label) + ": two survivors IoU > threshold");
}

// Returns true if index j appears in adj[i].
// O(1) because adj[i] is an unordered_set.
static bool edge_exists(
    const std::unordered_map<int, std::unordered_set<int>>& adj, int i, int j) {
    auto it = adj.find(i);
    if (it == adj.end()) return false;
    return it->second.count(j) > 0;
}

// ---- shared scenarios --------------------------------------

static std::vector<BBox> sc_empty() { return std::vector<BBox>(); }

static std::vector<BBox> sc_single() {
    std::vector<BBox> v;
    v.push_back(box(10, 20, 30, 40, 0.8f));
    return v;
}

// Two non-overlapping boxes (100-pixel gap).  IoU = 0.
static std::vector<BBox> sc_no_overlap() {
    std::vector<BBox> v;
    v.push_back(box(  0, 0, 10, 10, 0.9f)); // A
    v.push_back(box(110, 0, 10, 10, 0.7f)); // B
    return v;
}

// Two overlapping boxes.
// A(0,0,10,10) B(2,0,10,10): inter=80, union=120, IoU=0.667 > 0.5
// B has higher score -> B wins.
static std::vector<BBox> sc_overlap() {
    std::vector<BBox> v;
    v.push_back(box(0, 0, 10, 10, 0.3f)); // A -- lower score
    v.push_back(box(2, 0, 10, 10, 0.9f)); // B -- higher score
    return v;
}

// Transitive chain A--B--C; A and C NOT direct neighbours.
// IoU(A,B)=70/130~=0.538>0.5  IoU(B,C)~=0.538>0.5  IoU(A,C)=40/160=0.25<0.5
// One component {A,B,C}, winner A(0.9).
static std::vector<BBox> sc_chain() {
    std::vector<BBox> v;
    v.push_back(box(0, 0, 10, 10, 0.9f)); // A
    v.push_back(box(3, 0, 10, 10, 0.7f)); // B
    v.push_back(box(6, 0, 10, 10, 0.5f)); // C
    return v;
}

// Two separate clusters.
// Cluster 1: A(0.9) B(0.6)  IoU=0.667>0.5
// Cluster 2: C(0.8) D(0.4)  IoU=0.667>0.5  No cross-cluster overlap.
static std::vector<BBox> sc_two_clusters() {
    std::vector<BBox> v;
    v.push_back(box(  0,   0, 10, 10, 0.9f)); // A
    v.push_back(box(  2,   0, 10, 10, 0.6f)); // B
    v.push_back(box(100, 100, 10, 10, 0.8f)); // C
    v.push_back(box(102, 100, 10, 10, 0.4f)); // D
    return v;
}

// ============================================================
// SECTION 1: build_overlap_graph  (10 pts)
// ============================================================
static void test_graph() {
    std::cout << "\n--- Section 1: build_overlap_graph (10 pts) ---\n";

    run_test("graph: empty input -> empty map", 1, [](){
        auto adj = build_overlap_graph(sc_empty());
        check(adj.empty(), "empty input must produce empty adjacency map");
    });

    run_test("graph: single box -> one entry, empty neighbor set", 1, [](){
        auto adj = build_overlap_graph(sc_single());
        check(adj.size() == 1,    "single box must produce one map entry");
        check(adj.count(0) == 1,  "the single entry must be at key 0");
        check(adj.at(0).empty(),  "a single box has no neighbours");
    });

    run_test("graph: two non-overlapping -> no edges", 2, [](){
        auto adj = build_overlap_graph(sc_no_overlap());
        check(adj.size() == 2,       "must have entries for both indices");
        check(adj.at(0).empty(),     "index 0 must have no neighbours");
        check(adj.at(1).empty(),     "index 1 must have no neighbours");
    });

    run_test("graph: two overlapping -> symmetric edge, O(1) lookup", 3, [](){
        std::vector<BBox> sorted;
        sorted.push_back(box(2, 0, 10, 10, 0.9f)); // [0] B -- higher score first
        sorted.push_back(box(0, 0, 10, 10, 0.3f)); // [1] A
        auto adj = build_overlap_graph(sorted);
        check(edge_exists(adj, 0, 1), "adj[0] must contain 1");
        check(edge_exists(adj, 1, 0), "adj[1] must contain 0 (undirected)");
        check(!edge_exists(adj, 0, 0),"no self-loops");
        // Verify the unordered_set gives O(1) membership via .count()
        check(adj.at(0).count(1) == 1, "unordered_set::count must return 1");
    });

    run_test("graph: chain A-B-C -> correct adjacency", 3, [](){
        auto adj = build_overlap_graph(sc_chain());
        check( edge_exists(adj, 0, 1), "A overlaps B");
        check( edge_exists(adj, 1, 0), "symmetric");
        check( edge_exists(adj, 1, 2), "B overlaps C");
        check( edge_exists(adj, 2, 1), "symmetric");
        check(!edge_exists(adj, 0, 2), "A does NOT overlap C directly");
        check(!edge_exists(adj, 2, 0), "symmetric");
    });
}


// ============================================================
// SECTION 2: nms_queue  (25 pts)
// ============================================================
static void test_queue() {
    std::cout << "\n--- Section 2: nms_queue (25 pts) ---\n";

    run_test("queue: empty input -> empty output", 2, [](){
        check(nms_queue(sc_empty()).empty(), "empty input -> empty output");
    });

    run_test("queue: single box survives", 3, [](){
        auto out = nms_queue(sc_single());
        check(out.size() == 1 && has_score(out, 0.8f),
              "single input: score 0.8 must survive");
    });

    run_test("queue: two non-overlapping -> both survive", 4, [](){
        auto out = nms_queue(sc_no_overlap());
        check(out.size() == 2,      "both must survive");
        check(has_score(out, 0.9f), "score 0.9 survives");
        check(has_score(out, 0.7f), "score 0.7 survives");
        check_nms_invariant(out, "queue no-overlap");
    });

    run_test("queue: overlapping -> highest score wins", 5, [](){
        auto out = nms_queue(sc_overlap());
        check(out.size() == 1,       "one component -> one survivor");
        check(has_score(out, 0.9f),  "winner must be score 0.9");
        check(!has_score(out, 0.3f), "score 0.3 suppressed");
        check_nms_invariant(out, "queue overlap");
    });

    run_test("queue: transitive chain -> one survivor", 6, [](){
        auto out = nms_queue(sc_chain());
        check(out.size() == 1,      "chain is one component");
        check(has_score(out, 0.9f), "winner is A(0.9)");
        check_nms_invariant(out, "queue chain");
    });

    run_test("queue: two clusters -> two survivors", 5, [](){
        auto out = nms_queue(sc_two_clusters());
        check(out.size() == 2,      "two clusters -> two survivors");
        check(has_score(out, 0.9f), "cluster-1 winner score 0.9");
        check(has_score(out, 0.8f), "cluster-2 winner score 0.8");
        check_nms_invariant(out, "queue two-clusters");
    });
}


// ============================================================
// SECTION 3: nms_stack  (15 pts)
// ============================================================
static void test_stack() {
    std::cout << "\n--- Section 3: nms_stack (15 pts) ---\n";

    run_test("stack: empty input -> empty output", 1, [](){
        check(nms_stack(sc_empty()).empty(), "empty input -> empty output");
    });

    run_test("stack: single box survives", 2, [](){
        auto out = nms_stack(sc_single());
        check(out.size() == 1 && has_score(out, 0.8f),
              "single input: score 0.8 must survive");
    });

    run_test("stack: two non-overlapping -> both survive", 2, [](){
        auto out = nms_stack(sc_no_overlap());
        check(out.size() == 2,      "both must survive");
        check(has_score(out, 0.9f), "score 0.9 survives");
        check(has_score(out, 0.7f), "score 0.7 survives");
        check_nms_invariant(out, "stack no-overlap");
    });

    run_test("stack: overlapping -> highest score wins", 3, [](){
        auto out = nms_stack(sc_overlap());
        check(out.size() == 1,       "one component -> one survivor");
        check(has_score(out, 0.9f),  "winner must be score 0.9");
        check(!has_score(out, 0.3f), "score 0.3 suppressed");
        check_nms_invariant(out, "stack overlap");
    });

    run_test("stack: transitive chain -> one survivor", 4, [](){
        auto out = nms_stack(sc_chain());
        check(out.size() == 1,      "chain is one component");
        check(has_score(out, 0.9f), "winner is A(0.9)");
        check_nms_invariant(out, "stack chain");
    });

    run_test("stack: two clusters -> two survivors", 3, [](){
        auto out = nms_stack(sc_two_clusters());
        check(out.size() == 2,      "two clusters -> two survivors");
        check(has_score(out, 0.9f), "cluster-1 winner score 0.9");
        check(has_score(out, 0.8f), "cluster-2 winner score 0.8");
        check_nms_invariant(out, "stack two-clusters");
    });
}


// ============================================================
int main() {
    std::cout << "=== Part 2: NMS  (50 pts) ===\n";
    test_graph();
    test_queue();
    test_stack();
    std::cout << "\n========================================\n";
    std::cout << "  Score: " << g_earned << " / " << g_total << "\n";
    std::cout << "========================================\n";
    return (g_earned == g_total) ? 0 : 1;
}
