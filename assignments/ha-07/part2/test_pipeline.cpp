// ============================================================
// Assignment 07 - Part 2: Pipeline tests  (35 pts)
// test_pipeline.cpp  --  provided autograder, do not modify
// ============================================================
// Compile and run (from the part2/ directory):
//   g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp csv.cpp query.cpp \
//       test_pipeline.cpp -o test_pipeline
//   ./test_pipeline
//
// This test reads ../images/000000074646.csv and
//                 ../images/000000191845.csv
// Both files are part of the distributed assignment package.
// ============================================================

#include "bbox.h"
#include "nms.h"
#include "csv.h"
#include "query.h"
#include "category_names.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <cmath>
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

static void assert_nms_invariant(const std::vector<BBox>& survivors) {
    for (size_t i = 0; i < survivors.size(); i++)
        for (size_t j = i + 1; j < survivors.size(); j++)
            if (survivors[i].iou(survivors[j]) > IOU_THRESHOLD + 1e-5f)
                throw std::runtime_error(
                    "NMS invariant violated: two survivors have IoU > threshold");
}

static std::vector<BBox> run_nms(const std::vector<BBox>& all) {
    std::map<int, std::vector<BBox>> by_cat;
    for (size_t i = 0; i < all.size(); i++)
        by_cat[all[i].category_id].push_back(all[i]);
    std::vector<BBox> survivors;
    for (auto it = by_cat.begin(); it != by_cat.end(); ++it) {
        auto s = nms_queue(it->second);
        for (size_t i = 0; i < s.size(); i++) survivors.push_back(s[i]);
    }
    return survivors;
}

// ============================================================
int main() {
    std::cout << "=== Part 2: Pipeline Tests (35 pts) ===\n";

    std::vector<BBox> boxes_small = parse_csv("../images/000000074646.csv");
    std::vector<BBox> boxes_large = parse_csv("../images/000000191845.csv");

    // ---- SECTION A: CSV loading (4 pts) ---------------------
    std::cout << "\n--- Section A: CSV loading (4 pts) ---\n";

    run_test("parse_csv loads 000000074646.csv", 2, [&](){
        check(!boxes_small.empty(), "parse_csv returned empty -- check file path");
        check(boxes_small.size() > 100, "expected > 100 candidate boxes");
        check(boxes_small[0].w > 0 && boxes_small[0].h > 0,
              "boxes must have positive width and height");
    });

    run_test("parse_csv loads 000000191845.csv", 2, [&](){
        check(!boxes_large.empty(), "parse_csv returned empty -- check file path");
        check(boxes_large.size() > 500, "expected > 500 candidate boxes");
    });

    // ---- SECTION B: NMS correctness (18 pts) ----------------
    std::cout << "\n--- Section B: NMS correctness (18 pts) ---\n";

    run_test("NMS reduces candidate count (small image)", 3, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto s = run_nms(boxes_small);
        check(!s.empty(),                       "survivors must not be empty");
        check(s.size() < boxes_small.size(),    "NMS must reduce box count");
        check(s.size() * 2 < boxes_small.size(),"expected > 50% reduction");
    });

    run_test("NMS reduces candidate count (large image)", 3, [&](){
        if (boxes_large.empty()) throw std::runtime_error("CSV not loaded");
        auto s = run_nms(boxes_large);
        check(!s.empty(),                       "survivors must not be empty");
        check(s.size() < boxes_large.size(),    "NMS must reduce box count");
        check(s.size() * 4 < boxes_large.size(),"expected > 75% reduction");
    });

    run_test("NMS invariant per category (small image)", 3, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        std::map<int, std::vector<BBox>> by_cat;
        for (size_t i = 0; i < boxes_small.size(); i++)
            by_cat[boxes_small[i].category_id].push_back(boxes_small[i]);
        for (auto it = by_cat.begin(); it != by_cat.end(); ++it)
            assert_nms_invariant(nms_queue(it->second));
    });

    run_test("NMS invariant per category (large image)", 3, [&](){
        if (boxes_large.empty()) throw std::runtime_error("CSV not loaded");
        std::map<int, std::vector<BBox>> by_cat;
        for (size_t i = 0; i < boxes_large.size(); i++)
            by_cat[boxes_large[i].category_id].push_back(boxes_large[i]);
        for (auto it = by_cat.begin(); it != by_cat.end(); ++it)
            assert_nms_invariant(nms_queue(it->second));
    });

    run_test("queue and stack agree on survivor count per category", 3, [&](){
        if (boxes_large.empty()) throw std::runtime_error("CSV not loaded");
        std::map<int, std::vector<BBox>> by_cat;
        for (size_t i = 0; i < boxes_large.size(); i++)
            by_cat[boxes_large[i].category_id].push_back(boxes_large[i]);
        for (auto it = by_cat.begin(); it != by_cat.end(); ++it) {
            size_t q = nms_queue(it->second).size();
            size_t s = nms_stack(it->second).size();
            check(q == s, "queue and stack must return same survivor count");
        }
    });

    run_test("all survivors come from the original input", 3, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto s = run_nms(boxes_small);
        for (size_t i = 0; i < s.size(); i++) {
            bool found = false;
            for (size_t j = 0; j < boxes_small.size(); j++) {
                if (std::fabs(boxes_small[j].score - s[i].score) < 1e-6f &&
                    std::fabs(boxes_small[j].x     - s[i].x)     < 1e-4f &&
                    std::fabs(boxes_small[j].y     - s[i].y)     < 1e-4f) {
                    found = true; break;
                }
            }
            check(found, "every survivor must be an exact box from the CSV");
        }
    });

    // ---- SECTION C: Query functions (13 pts) ----------------
    std::cout << "\n--- Section C: Query functions (13 pts) ---\n";

    run_test("best_per_category: one entry per category", 2, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto survivors = run_nms(boxes_small);
        auto best = best_per_category(survivors);
        std::map<int, int> cat_count;
        for (size_t i = 0; i < survivors.size(); i++)
            cat_count[survivors[i].category_id]++;
        check(best.size() == cat_count.size(),
              "best_per_category must have exactly one entry per category");
    });

    run_test("best_per_category: stored score is max for that category", 3, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto survivors = run_nms(boxes_small);
        auto best = best_per_category(survivors);
        for (size_t i = 0; i < survivors.size(); i++) {
            int cat = survivors[i].category_id;
            check(best.count(cat) == 1,
                  "every survivor's category must appear in best_per_category");
            check(best.at(cat).score >= survivors[i].score - 1e-5f,
                  "stored score must be >= every survivor in that category");
        }
    });

    run_test("top_k_global: returns k boxes in descending order", 3, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto survivors = run_nms(boxes_small);
        size_t k = 5;
        auto top = top_k_global(survivors, k);
        check(top.size() == k, "top_k_global must return exactly k boxes");
        for (size_t i = 1; i < top.size(); i++)
            check(top[i].score <= top[i-1].score + 1e-5f,
                  "must be in non-ascending score order");
        for (size_t i = 0; i < top.size(); i++) {
            bool found = false;
            for (size_t j = 0; j < survivors.size(); j++)
                if (std::fabs(survivors[j].score - top[i].score) < 1e-6f)
                    { found = true; break; }
            check(found, "top_k result must come from survivors");
        }
    });

    run_test("find_by_name: 'person' matches category 1", 2, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto survivors = run_nms(boxes_small);
        auto people = find_by_name(survivors, "person");
        check(category_name(1) == "person",
              "category 1 must be 'person' -- check category_names.h");
        check(!people.empty(),
              "find_by_name('person') must return at least one box");
        for (size_t i = 0; i < people.size(); i++)
            check(people[i].category_id == 1,
                  "all returned boxes must belong to category 1");
    });

    run_test("score_range: returns exactly boxes in [lo, hi], descending", 3, [&](){
        if (boxes_small.empty()) throw std::runtime_error("CSV not loaded");
        auto survivors = run_nms(boxes_small);
        float lo = 0.3f, hi = 0.7f;
        auto r = score_range(survivors, lo, hi);
        for (size_t i = 0; i < r.size(); i++) {
            check(r[i].score >= lo - 1e-5f, "score below lo");
            check(r[i].score <= hi + 1e-5f, "score above hi");
        }
        int expected = 0;
        for (size_t i = 0; i < survivors.size(); i++)
            if (survivors[i].score >= lo - 1e-5f && survivors[i].score <= hi + 1e-5f)
                expected++;
        check((int)r.size() == expected, "must return all boxes in [lo, hi]");
        for (size_t i = 1; i < r.size(); i++)
            check(r[i].score <= r[i-1].score + 1e-5f,
                  "score_range must be in descending order");
    });

    std::cout << "\n========================================\n";
    std::cout << "  Score: " << g_earned << " / " << g_total << "\n";
    std::cout << "========================================\n";
    return (g_earned == g_total) ? 0 : 1;
}
