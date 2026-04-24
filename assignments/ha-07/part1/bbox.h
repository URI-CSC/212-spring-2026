#ifndef BBOX_H
#define BBOX_H

// ============================================================
// Assignment 07 - Part 1: BBox Primitives
// bbox.h  --  provided, do not modify
// ============================================================

struct BBox {
    int   image_id;
    int   category_id;
    float x;      // left edge  (lt_x in CSV)
    float y;      // top  edge  (lt_y in CSV)
    float w;      // width
    float h;      // height
    float score;  // detection confidence in [0, 1]

    // Returns w * h.
    float area() const;

    // Returns Intersection-over-Union with other.
    // IoU = area(intersection) / area(union).
    // Returns 0.0 if the boxes do not overlap.
    float iou(const BBox& other) const;

    // Compares by score (ascending). Used by std::priority_queue (max-heap).
    bool operator<(const BBox& other) const;
};

#endif // BBOX_H
