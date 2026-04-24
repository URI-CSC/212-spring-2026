// ============================================================
// Assignment 07 - Part 1: BBox Primitives  (10 pts)
// test_bbox.cpp  --  provided autograder, do not modify
// ============================================================
// Compile and run:
//   g++ -std=c++17 -Wall -Werror bbox.cpp test_bbox.cpp -o test_bbox
//   ./test_bbox
// ============================================================

#include "bbox.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string>
#include <cmath>

int g_earned = 0;
int g_total  = 0;

void check(bool cond, const char* msg = "condition failed") {
    if (!cond) throw std::runtime_error(msg);
}

static bool approx(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
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

static BBox make(float x, float y, float w, float h, float score, int cat = 0) {
    BBox b;
    b.image_id = 1; b.category_id = cat;
    b.x = x; b.y = y; b.w = w; b.h = h; b.score = score;
    return b;
}


// ============================================================
// Section 1 - BBox area()  (2 pts)
// ============================================================
static void test_area() {
    std::cout << "\n--- Section 1: area() (2 pts) ---\n";

    run_test("area() correctness", 2, [](){
        check(approx(make(0,0,10,10,0).area(), 100.0f), "10x10 box area should be 100");
        check(approx(make(5,5, 3, 4,0).area(),  12.0f), "3x4 box area should be 12");
        check(approx(make(0,0, 1, 1,0).area(),   1.0f), "1x1 box area should be 1");
        check(approx(make(0,0,20, 5,0).area(), 100.0f), "20x5 box area should be 100");
    });
}


// ============================================================
// Section 1 - BBox iou()  (6 pts)
// ============================================================
static void test_iou() {
    std::cout << "\n--- Section 1: iou() (6 pts) ---\n";

    run_test("iou() - identical boxes", 1, [](){
        BBox a = make(0, 0, 10, 10, 0.5f);
        check(approx(a.iou(a), 1.0f), "IoU of a box with itself must be 1.0");
    });

    run_test("iou() - no overlap (gap between boxes)", 2, [](){
        BBox a = make(  0, 0, 10, 10, 0.9f);
        BBox b = make(100, 0, 10, 10, 0.5f);
        check(approx(a.iou(b), 0.0f), "Non-overlapping boxes must have IoU 0");
        check(approx(b.iou(a), 0.0f), "iou() must be symmetric");

        BBox c = make(0,  0, 10, 10, 0.9f);
        BBox d = make(0, 10, 10, 10, 0.5f);
        check(approx(c.iou(d), 0.0f), "Boxes touching at edge only: IoU 0");
    });

    run_test("iou() - partial overlap and symmetry", 3, [](){
        // A: (0,0,10,10)  B: (5,0,10,10)
        // intersection = (5..10, 0..10) = 5*10 = 50
        // union        = 100 + 100 - 50 = 150
        // IoU          = 50 / 150 = 1/3
        BBox a = make(0, 0, 10, 10, 0.9f);
        BBox b = make(5, 0, 10, 10, 0.5f);
        float expected = 50.0f / 150.0f;
        check(approx(a.iou(b), expected), "IoU should be 50/150 (~0.333)");
        check(approx(b.iou(a), expected), "iou() must be symmetric");

        // Two boxes where A is fully inside B
        // A: (2,2,3,3) area=9   B: (0,0,10,10) area=100
        // intersection = A = 9,  union = 100 + 9 - 9 = 100
        BBox inner = make(2, 2,  3,  3, 0.7f);
        BBox outer = make(0, 0, 10, 10, 0.4f);
        check(approx(inner.iou(outer), 9.0f / 100.0f),
              "Contained box: IoU should equal inner.area / outer.area");
    });
}


// ============================================================
// Section 1 - BBox operator<  (2 pts)
// ============================================================
static void test_operator_less() {
    std::cout << "\n--- Section 1: operator< (2 pts) ---\n";

    run_test("operator< compares by score", 2, [](){
        BBox lo = make(0, 0, 10, 10, 0.3f);
        BBox hi = make(0, 0, 10, 10, 0.9f);
        check( (lo < hi),  "lower-score box must be < higher-score box");
        check(!(hi < lo),  "higher-score box must not be < lower-score box");
        check(!(lo < lo),  "a box must not be < itself (strict less-than)");
        check(!(hi < hi),  "a box must not be < itself (strict less-than)");
    });
}


// ============================================================
int main() {
    std::cout << "=== Part 1: BBox Primitives (10 pts) ===\n";

    test_area();
    test_iou();
    test_operator_less();

    std::cout << "\n========================================\n";
    std::cout << "  Score: " << g_earned << " / " << g_total << "\n";
    std::cout << "========================================\n";
    return (g_earned == g_total) ? 0 : 1;
}
