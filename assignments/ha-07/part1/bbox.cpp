// ============================================================
// Assignment 07 - Part 1: BBox Primitives  (10 pts)
// bbox.cpp  --  YOUR WORK GOES HERE
// ============================================================
// Compile and run with the autograder:
//   g++ -std=c++17 -Wall -Werror bbox.cpp test_bbox.cpp -o test_bbox
//   ./test_bbox
//
// Do NOT modify bbox.h or test_bbox.cpp.
// ============================================================

#include "bbox.h"

// Returns the area of this bounding box.
float BBox::area() const {
    // TODO (Section 1 - 2 pts)
    return 0.0f;
}

// Returns the Intersection-over-Union of this box and other.
//
// To find the intersection rectangle, clamp each edge:
//   left   = max of the two left edges
//   top    = max of the two top  edges
//   right  = min of the two right  edges  (x + w)
//   bottom = min of the two bottom edges  (y + h)
// If right <= left or bottom <= top, the boxes do not overlap.
// Otherwise  inter = (right - left) * (bottom - top).
// IoU = inter / (area() + other.area() - inter).
float BBox::iou(const BBox& other) const {
    // TODO (Section 1 - 6 pts)
    return 0.0f;
}

// Returns true when this box's score is strictly less than other's score.
bool BBox::operator<(const BBox& other) const {
    // TODO (Section 1 - 2 pts)
    return false;
}
