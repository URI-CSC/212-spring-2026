// ============================================================
// Assignment 07 - Part 2: CSV parser
// csv.cpp  --  provided, do not modify
// ============================================================

#include "csv.h"
#include <fstream>
#include <sstream>
#include <string>

std::vector<BBox> parse_csv(const std::string& path) {
    std::vector<BBox> boxes;
    std::ifstream file(path.c_str());
    if (!file.is_open()) return boxes;

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        BBox b;
        char sep;
        std::istringstream ss(line);
        ss >> b.image_id    >> sep
           >> b.category_id >> sep
           >> b.x           >> sep
           >> b.y           >> sep
           >> b.w           >> sep
           >> b.h           >> sep
           >> b.score;
        if (ss) boxes.push_back(b);
    }
    return boxes;
}
