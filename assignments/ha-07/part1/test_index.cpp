// ============================================================
// Assignment 07 - Part 1: SpatialIndex tests  (30 pts)
// test_index.cpp  --  provided autograder, do not modify
// ============================================================
// Compile and run:
//   g++ -std=c++17 -Wall -Werror bbox.cpp index.cpp test_index.cpp -o test_index
//   ./test_index
// ============================================================

#include "bbox.h"
#include "index.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

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

static BBox box(int cat, float score) {
    BBox b;
    b.image_id = 1; b.category_id = cat;
    b.x = 0; b.y = 0; b.w = 10; b.h = 10; b.score = score;
    return b;
}

static bool has_score(const std::vector<BBox>& v, float s, float eps = 1e-5f) {
    for (size_t i = 0; i < v.size(); i++)
        if (std::fabs(v[i].score - s) < eps) return true;
    return false;
}

static bool has_id(const std::vector<int>& ids, int id) {
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}

static bool is_descending(const std::vector<BBox>& v) {
    for (size_t i = 1; i < v.size(); i++)
        if (v[i].score > v[i-1].score + 1e-6f) return false;
    return true;
}

// ============================================================
int main() {
    std::cout << "=== Part 1: SpatialIndex (30 pts) ===\n";

    // ---- empty index ----------------------------------------
    run_test("empty index: total() == 0", 1, [](){
        SpatialIndex idx;
        check(idx.total() == 0, "empty index must report total == 0");
    });

    run_test("empty index: num_categories() == 0", 1, [](){
        SpatialIndex idx;
        check(idx.num_categories() == 0,
              "empty index must report num_categories == 0");
    });

    run_test("empty index: has_category returns false", 1, [](){
        SpatialIndex idx;
        check(!idx.has_category(1),  "has_category(1) must be false on empty index");
        check(!idx.has_category(99), "has_category(99) must be false on empty index");
    });

    run_test("empty index: category_ids() is empty", 1, [](){
        SpatialIndex idx;
        check(idx.category_ids().empty(),
              "category_ids() must be empty on an empty index");
    });

    // ---- single insert ----------------------------------------
    run_test("single insert: total == 1", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        check(idx.total() == 1, "total must be 1 after one insert");
    });

    run_test("single insert: has_category correct", 2, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        check( idx.has_category(1), "has_category(1) must be true after inserting cat 1");
        check(!idx.has_category(2), "has_category(2) must be false");
    });

    run_test("single insert: get_category returns that box", 2, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        const std::vector<BBox>& v = idx.get_category(1);
        check(v.size() == 1,       "get_category must return exactly 1 box");
        check(std::fabs(v[0].score - 0.9f) < 1e-5f,
              "the returned box must have score 0.9");
    });

    // ---- multiple inserts, same category ----------------------
    run_test("same-category inserts: all boxes stored", 3, [](){
        SpatialIndex idx;
        idx.insert(box(5, 0.9f));
        idx.insert(box(5, 0.7f));
        idx.insert(box(5, 0.5f));
        check(idx.total() == 3,              "total must be 3");
        check(idx.num_categories() == 1,     "only one distinct category");
        const std::vector<BBox>& v = idx.get_category(5);
        check(v.size() == 3,                 "get_category must return all 3 boxes");
    });

    // ---- multiple categories ----------------------------------
    run_test("multiple categories: total and num_categories", 2, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        idx.insert(box(1, 0.8f));
        idx.insert(box(2, 0.7f));
        idx.insert(box(3, 0.6f));
        check(idx.total()          == 4, "total must be 4");
        check(idx.num_categories() == 3, "num_categories must be 3");
    });

    run_test("multiple categories: category_ids contains all", 3, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        idx.insert(box(2, 0.7f));
        idx.insert(box(3, 0.5f));
        std::vector<int> ids = idx.category_ids();
        check(ids.size() == 3,     "category_ids must have 3 entries");
        check(has_id(ids, 1),      "category_ids must contain 1");
        check(has_id(ids, 2),      "category_ids must contain 2");
        check(has_id(ids, 3),      "category_ids must contain 3");
    });

    run_test("multiple categories: get_category isolates correctly", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        idx.insert(box(1, 0.8f));
        idx.insert(box(2, 0.7f));
        check(idx.get_category(1).size() == 2, "cat 1 must have 2 boxes");
        check(idx.get_category(2).size() == 1, "cat 2 must have 1 box");
    });

    // ---- top_k ------------------------------------------------
    run_test("top_k: returns 0 when k == 0", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        check(idx.top_k(0).empty(), "top_k(0) must return empty vector");
    });

    run_test("top_k: single element, k=1", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.8f));
        std::vector<BBox> r = idx.top_k(1);
        check(r.size() == 1,                           "top_k(1) must return 1 box");
        check(std::fabs(r[0].score - 0.8f) < 1e-5f,   "must be the box with score 0.8");
    });

    run_test("top_k: correct top-2 from 4 boxes, two categories", 3, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        idx.insert(box(2, 0.7f));
        idx.insert(box(1, 0.8f));
        idx.insert(box(3, 0.5f));
        std::vector<BBox> r = idx.top_k(2);
        check(r.size() == 2,           "top_k(2) must return 2 boxes");
        check(has_score(r, 0.9f),      "top-2 must include score 0.9");
        check(has_score(r, 0.8f),      "top-2 must include score 0.8");
        check(!has_score(r, 0.7f),     "score 0.7 is NOT in top-2");
        check(is_descending(r),        "top_k result must be in descending score order");
    });

    run_test("top_k(k > total) returns all in descending order", 2, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.6f));
        idx.insert(box(2, 0.9f));
        idx.insert(box(1, 0.3f));
        std::vector<BBox> r = idx.top_k(100);
        check(r.size() == 3,       "top_k(100) with 3 boxes must return 3");
        check(is_descending(r),    "must be in descending score order");
        check(has_score(r, 0.9f) && has_score(r, 0.6f) && has_score(r, 0.3f),
              "all boxes must be present");
    });

    // ---- score_range (NEW) ------------------------------------
    run_test("score_range: empty result when no boxes in range", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        idx.insert(box(2, 0.8f));
        std::vector<BBox> r = idx.score_range(0.1f, 0.5f);
        check(r.empty(), "no boxes in [0.1, 0.5] so result must be empty");
    });

    run_test("score_range: returns only boxes in [lo, hi]", 2, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.9f));
        idx.insert(box(1, 0.7f));
        idx.insert(box(2, 0.5f));
        idx.insert(box(2, 0.3f));
        std::vector<BBox> r = idx.score_range(0.4f, 0.8f);
        check(r.size() == 2, "exactly 2 boxes have score in [0.4, 0.8]");
        check(has_score(r, 0.7f), "score 0.7 must be included");
        check(has_score(r, 0.5f), "score 0.5 must be included");
        check(!has_score(r, 0.9f), "score 0.9 is above hi and must be excluded");
        check(!has_score(r, 0.3f), "score 0.3 is below lo and must be excluded");
    });

    run_test("score_range: result is in descending score order", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.8f));
        idx.insert(box(1, 0.6f));
        idx.insert(box(2, 0.4f));
        idx.insert(box(2, 0.2f));
        std::vector<BBox> r = idx.score_range(0.3f, 0.9f);
        check(r.size() == 3, "three boxes in [0.3, 0.9]");
        check(is_descending(r), "score_range result must be in descending order");
    });

    run_test("score_range: boundary values are inclusive", 1, [](){
        SpatialIndex idx;
        idx.insert(box(1, 0.5f));
        idx.insert(box(2, 0.8f));
        idx.insert(box(3, 0.2f));
        // [0.5, 0.8] should include both endpoints
        std::vector<BBox> r = idx.score_range(0.5f, 0.8f);
        check(r.size() == 2, "lo and hi endpoints must be included");
        check(has_score(r, 0.5f), "lo boundary must be included");
        check(has_score(r, 0.8f), "hi boundary must be included");
    });

    std::cout << "\n========================================\n";
    std::cout << "  Score: " << g_earned << " / " << g_total << "\n";
    std::cout << "========================================\n";
    return (g_earned == g_total) ? 0 : 1;
}
