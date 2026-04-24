// ============================================================
// Assignment 07 - Part 2: Full Pipeline
// main.cpp  --  PROVIDED, do not modify
// ============================================================
// This file is given to you complete.  Read it carefully --
// it shows how all the pieces fit together.
//
// Build:
//   g++ -std=c++17 -Wall -Werror bbox.cpp nms.cpp csv.cpp query.cpp \
//       main.cpp -o nms
//
// Run (normal mode -- produces an SVG):
//   ./nms <csv_file> <output_svg>
//   ./nms ../images/000000074646.csv out.svg
//   open out.svg
//
// Run (compare mode -- times nms_queue vs nms_stack):
//   ./nms --compare <csv_file>
//   ./nms --compare ../images/000000191845.csv
//
// ============================================================
// WHAT THIS FILE DOES -- read before you write nms.cpp / query.cpp
//
// Normal mode (run_normal):
//   1. parse_csv() loads all candidate boxes from the CSV.
//   2. Boxes are grouped by category_id into a map<int, vector<BBox>>.
//   3. nms_queue() is called separately on each category group.
//      (Boxes from different categories must not suppress each other.)
//   4. The SVG shows:
//        - every original candidate as a thin grey outline
//        - every survivor as a semi-transparent filled rectangle
//          coloured by category_color(category_id)
//        - each survivor's category name and score as a text label
//
// Compare mode (run_compare):
//   Runs both nms_queue and nms_stack 5 times each on all categories.
//   Prints average wall-clock time in ms for each.
//   Checks that both return the same survivor count.
// ============================================================

#include "bbox.h"
#include "nms.h"
#include "svg.h"
#include "csv.h"
#include "query.h"
#include "category_names.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>

// ---- helpers ------------------------------------------------

static std::string box_label(const BBox& b) {
    std::ostringstream oss;
    oss << category_name(b.category_id) << " "
        << std::fixed << std::setprecision(2) << b.score;
    return oss.str();
}

static void compute_viewport(const std::vector<BBox>& boxes,
                              float& vw, float& vh) {
    float max_x = 0.0f, max_y = 0.0f;
    for (size_t i = 0; i < boxes.size(); i++) {
        float rx = boxes[i].x + boxes[i].w;
        float ry = boxes[i].y + boxes[i].h;
        if (rx > max_x) max_x = rx;
        if (ry > max_y) max_y = ry;
    }
    vw = max_x + 10.0f;
    vh = max_y + 10.0f;
}

// ---- normal mode --------------------------------------------

static int run_normal(const std::string& csv_path,
                      const std::string& svg_path) {
    std::vector<BBox> all = parse_csv(csv_path);
    if (all.empty()) {
        std::cerr << "Error: no boxes loaded from " << csv_path << "\n";
        return 1;
    }

    std::map<int, std::vector<BBox>> by_cat;
    for (size_t i = 0; i < all.size(); i++)
        by_cat[all[i].category_id].push_back(all[i]);

    std::vector<BBox> survivors;
    for (auto it = by_cat.begin(); it != by_cat.end(); ++it) {
        auto s = nms_queue(it->second);
        for (size_t i = 0; i < s.size(); i++) survivors.push_back(s[i]);
    }

    std::ofstream out(svg_path.c_str());
    if (!out.is_open()) {
        std::cerr << "Error: cannot open " << svg_path << " for writing\n";
        return 1;
    }

    float vw, vh;
    compute_viewport(all, vw, vh);
    svg_begin(out, vw, vh);

    // embed the source image (replace .csv extension with .jpg)
    std::string img_path = csv_path;
    size_t dot = img_path.rfind('.');
    if (dot != std::string::npos) img_path.replace(dot, std::string::npos, ".jpg");
    svg_image(out, 0.0f, 0.0f, vw, vh, img_path);

    for (size_t i = 0; i < all.size(); i++)
        svg_rect(out, all[i].x, all[i].y, all[i].w, all[i].h,
                 "none", "#aaaaaa", 0.5f, 1.0f);

    for (size_t i = 0; i < survivors.size(); i++) {
        const BBox& b = survivors[i];
        std::string color = category_color(b.category_id);
        svg_rect(out, b.x, b.y, b.w, b.h, color, color, 1.5f, 0.45f);
        svg_text(out, b.x + 2.0f, b.y + 12.0f, box_label(b), "#000000", 10.0f);
    }

    svg_end(out);
    std::cout << "Loaded  : " << all.size()       << " candidates\n";
    std::cout << "Survived: " << survivors.size() << " boxes\n";
    std::cout << "SVG     : " << svg_path         << "\n";
    return 0;
}

// ---- compare mode -------------------------------------------

static int run_compare(const std::string& csv_path) {
    std::vector<BBox> all = parse_csv(csv_path);
    if (all.empty()) {
        std::cerr << "Error: no boxes loaded from " << csv_path << "\n";
        return 1;
    }

    std::map<int, std::vector<BBox>> by_cat;
    for (size_t i = 0; i < all.size(); i++)
        by_cat[all[i].category_id].push_back(all[i]);

    const int REPS = 5;

    double total_q = 0.0; size_t survivors_q = 0;
    for (int r = 0; r < REPS; r++) {
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t cnt = 0;
        for (auto it = by_cat.begin(); it != by_cat.end(); ++it)
            cnt += nms_queue(it->second).size();
        total_q += std::chrono::duration<double, std::milli>(
            std::chrono::high_resolution_clock::now() - t0).count();
        if (r == 0) survivors_q = cnt;
    }

    double total_s = 0.0; size_t survivors_s = 0;
    for (int r = 0; r < REPS; r++) {
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t cnt = 0;
        for (auto it = by_cat.begin(); it != by_cat.end(); ++it)
            cnt += nms_stack(it->second).size();
        total_s += std::chrono::duration<double, std::milli>(
            std::chrono::high_resolution_clock::now() - t0).count();
        if (r == 0) survivors_s = cnt;
    }

    std::cout << "=== Compare mode  (" << REPS << " runs each) ===\n";
    std::cout << "Input  : " << all.size() << " candidates, "
              << by_cat.size() << " categories\n\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "nms_queue  avg: " << total_q / REPS << " ms"
              << "   survivors: " << survivors_q << "\n";
    std::cout << "nms_stack  avg: " << total_s / REPS << " ms"
              << "   survivors: " << survivors_s << "\n\n";

    size_t total_pairs = 0;
    for (auto it = by_cat.begin(); it != by_cat.end(); ++it) {
        size_t n = it->second.size();
        total_pairs += n * (n - 1) / 2;
    }
    std::cout << "Total IoU pairs evaluated (graph construction): "
              << total_pairs << "\n";

    if (survivors_q != survivors_s) {
        std::cerr << "\nWARNING: queue and stack returned different survivor "
                     "counts!\nnms_queue=" << survivors_q
                  << "  nms_stack=" << survivors_s << "\n";
        return 1;
    }
    return 0;
}

// ---- main ---------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc == 3 && std::string(argv[1]) == "--compare")
        return run_compare(argv[2]);
    if (argc == 3)
        return run_normal(argv[1], argv[2]);
    std::cerr << "Usage:\n"
              << "  " << argv[0] << " <csv_file> <output_svg>\n"
              << "  " << argv[0] << " --compare <csv_file>\n";
    return 1;
}
