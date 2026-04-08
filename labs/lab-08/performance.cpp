// performance.cpp — BST Performance Visualizer
//
// Builds BSTs from random integer data and demonstrates that the average
// search depth tracks 1.39 * log2(N) — the theoretical expectation for a
// randomly built BST.  You do not need to modify this file.
//
// Compile:  g++ -std=c++11 -O2 bst.cpp performance.cpp -o performance
// Run:      ./performance
//

#include "bst.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <cmath>
#include <algorithm>

// Fisher-Yates shuffle using rand().
static void shuffle(std::vector<int>& v) {
    for (int i = static_cast<int>(v.size()) - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(v[i], v[j]);
    }
}

// Generate n unique random keys, insert them into a BST, then measure
// the search depth for each key.  Returns the average depth and fills
// hist with the depth-frequency histogram.
static double benchmark(int n, std::map<int,int>& hist) {
    // Build a pool of n unique integers in [0, 10n).
    std::set<int> seen;
    std::vector<int> keys;
    keys.reserve(n);
    while (static_cast<int>(keys.size()) < n) {
        int v = rand() % (n * 10);
        if (seen.insert(v).second)
            keys.push_back(v);
    }
    shuffle(keys);

    BST tree;
    for (int k : keys) tree.insert(k);

    long long total = 0;
    hist.clear();
    for (int k : keys) {
        int d = tree.searchDepth(k);
        total += d;
        hist[d]++;
    }
    return static_cast<double>(total) / n;
}

static void printHistogram(const std::map<int,int>& hist, int n) {
    int max_count = 0;
    for (auto& p : hist) max_count = std::max(max_count, p.second);

    const int BAR_WIDTH = 40;
    std::cout << "\nSearch depth histogram  (N = " << n << ")\n";
    std::cout << std::string(60, '-') << "\n";

    for (auto& p : hist) {
        int depth = p.first;
        int count = p.second;
        int bar   = (max_count > 0) ? (count * BAR_WIDTH / max_count) : 0;

        std::cout << "  depth " << std::setw(2) << depth << " | "
                  << std::string(bar, '#')
                  << " (" << count << ")\n";
    }

    double log2n = std::log2(n);
    std::cout << std::string(60, '-') << "\n";
    std::cout << "  log2(" << n << ") = " << std::fixed << std::setprecision(2)
              << log2n << "\n\n";
}

int main() {
    std::srand(42);   // fixed seed — reproducible output

    std::cout << "=== BST Performance Visualizer ===\n\n";

    // -----------------------------------------------------------------------
    // Part A: Scaling table
    //
    // For each tree size N, insert N random keys and measure average search
    // depth.  The ratio column shows  avg_depth / log2(N).
    // Theory predicts this converges to ~1.39 for random BSTs.
    // -----------------------------------------------------------------------
    std::cout << "Part A — Average search depth vs. log2(N)\n\n";
    std::cout << std::left
              << std::setw(8)  << "N"
              << std::setw(12) << "log2(N)"
              << std::setw(15) << "avg depth"
              << "ratio\n";
    std::cout << std::string(48, '-') << "\n";

    std::vector<int> sizes = {16, 64, 256, 1024, 4096};
    for (int n : sizes) {
        std::map<int,int> hist;
        double avg   = benchmark(n, hist);
        double log2n = std::log2(n);
        double ratio = avg / log2n;

        std::cout << std::left
                  << std::setw(8)  << n
                  << std::setw(12) << std::fixed << std::setprecision(2) << log2n
                  << std::setw(15) << avg
                  << ratio << "\n";
    }

    std::cout << "\n  -> The ratio converges to ~1.39: a random BST is about\n"
                 "     39% deeper than a perfect tree, but still O(log N).\n";

    // -----------------------------------------------------------------------
    // Part B: Depth histogram for N = 1024
    //
    // Look at the shape of the distribution.  Questions to discuss:
    //   1. What is the most common search depth?  How does it compare
    //      to log2(1024) = 10?
    //   2. What is the deepest search path you see?  Is it much worse
    //      than the average?
    //   3. Why is there almost no searches at depth 1 or 2?
    // -----------------------------------------------------------------------
    std::cout << "\nPart B — Depth distribution for N = 1024\n";

    std::srand(42);
    std::map<int,int> hist1024;
    benchmark(1024, hist1024);
    printHistogram(hist1024, 1024);

    std::cout << "  Notice the bell-curve shape centered well above log2(1024) = 10.\n"
                 "  Even the worst-case depths are far short of O(N).\n\n";

    // -----------------------------------------------------------------------
    // Part C: Compare random vs. sorted insertion
    //
    // Inserting keys in sorted order builds a degenerate BST (a linked list).
    // This is the worst case: height = N-1, every search is O(N).
    // -----------------------------------------------------------------------
    std::cout << "Part C — What happens with sorted insertion?\n\n";

    for (int n : {16, 64, 256}) {
        BST sorted_tree;
        for (int i = 0; i < n; i++) sorted_tree.insert(i);

        int h = sorted_tree.height();
        double log2n = std::log2(n);

        std::cout << "  N = " << std::setw(4) << n
                  << "  height (random) ~ " << std::setw(5)
                  << std::fixed << std::setprecision(1) << (1.39 * log2n)
                  << "   height (sorted) = " << h << "\n";
    }

    std::cout << "\n  -> Sorted insertion degrades the BST to O(N) height.\n"
                 "     This is why self-balancing trees (AVL, Red-Black) exist.\n\n";

    return 0;
}
