#ifndef CSV_H
#define CSV_H

// ============================================================
// Assignment 07 - Part 2: CSV parser
// csv.h  --  provided, do not modify
// ============================================================

#include "bbox.h"
#include <string>
#include <vector>

// Parses the CSV file at path into a vector of BBox objects.
// Expected header line (skipped automatically):
//   image_id,category_id,lt_x,lt_y,width,height,score
// Returns an empty vector if the file cannot be opened.
std::vector<BBox> parse_csv(const std::string& path);

#endif // CSV_H
