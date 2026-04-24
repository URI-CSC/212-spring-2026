// ============================================================
// Assignment 07 - Part 2: BBox Primitives
// bbox.cpp  --  provided, do not modify
// ============================================================

#include "bbox.h"
#include <algorithm>

float BBox::area() const {
    return w * h;
}

float BBox::iou(const BBox& other) const {
    float x_left   = std::max(x, other.x);
    float y_top    = std::max(y, other.y);
    float x_right  = std::min(x + w, other.x + other.w);
    float y_bottom = std::min(y + h, other.y + other.h);
    if (x_right <= x_left || y_bottom <= y_top) return 0.0f;
    float inter = (x_right - x_left) * (y_bottom - y_top);
    return inter / (area() + other.area() - inter);
}

bool BBox::operator<(const BBox& other) const {
    return score < other.score;
}
