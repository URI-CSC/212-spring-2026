#include "bst.h"
#include <cassert>
#include <iostream>
#include <sstream>

// Suppresses search() diagnostic output during automated tests.
// The return value is still checked by the asserts.
static bool silentSearch(const BST& t, int key) {
    std::streambuf* old = std::cout.rdbuf();
    std::ostringstream sink;
    std::cout.rdbuf(sink.rdbuf());
    bool result = t.search(key);
    std::cout.rdbuf(old);
    return result;
}

int main() {
    int  points = 0;

    std::cout << "=== BST Test Suite ===\n\n";

    // -----------------------------------------------------------------------
    // Visual demo — run this first to eyeball your search log format.
    // -----------------------------------------------------------------------
    {
        BST demo;
        demo.insert(50);
        demo.insert(30);
        demo.insert(70);
        demo.insert(20);
        demo.insert(40);

        std::cout << "--- Sample search log (visual check) ---\n";
        demo.search(40);   // should print FOUND with real addresses
        demo.search(99);   // should print NOT FOUND with null addresses
        std::cout << "----------------------------------------\n\n";
    }

    std::cout << "--- Automated tests begin ---\n\n";

    // -----------------------------------------------------------------------
    // insert + inorder (in-order must produce sorted output)
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(50);
        t.insert(30);
        t.insert(70);
        t.insert(20);
        t.insert(40);
        t.insert(90);

        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        t.inorder();
        std::cout.rdbuf(old);

        assert(buf.str() == "20 30 40 50 70 90 \n");
        points += 2;
        std::cout << "[+2] insert + inorder: PASSED\n";
    }

    // -----------------------------------------------------------------------
    // Duplicate insert — tree must not grow
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(10);
        t.insert(10);
        t.insert(10);

        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        t.inorder();
        std::cout.rdbuf(old);

        assert(buf.str() == "10 \n");
        points += 1;
        std::cout << "[+1] duplicate insert (no-op): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // search — found
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(50); t.insert(30); t.insert(70);

        bool found = silentSearch(t, 30);
        assert(found == true);
        points += 1;
        std::cout << "[+1] search (found): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // search — not found
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(50); t.insert(30); t.insert(70);

        bool found = silentSearch(t, 99);
        assert(found == false);
        points += 1;
        std::cout << "[+1] search (not found): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // remove — leaf node
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(50); t.insert(30); t.insert(70);
        t.remove(30);   // leaf

        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        t.inorder();
        std::cout.rdbuf(old);

        assert(buf.str() == "50 70 \n");
        points += 1;
        std::cout << "[+1] remove leaf: PASSED\n";
    }

    // -----------------------------------------------------------------------
    // remove — one child
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(50); t.insert(30); t.insert(70); t.insert(20);
        t.remove(30);   // 30 has one child: 20

        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        t.inorder();
        std::cout.rdbuf(old);

        assert(buf.str() == "20 50 70 \n");
        points += 1;
        std::cout << "[+1] remove (one child): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // remove — two children
    //
    // Tree before:      50            After removing 50, the in-order
    //                  /  \           successor (70) takes its place:
    //                30    70
    //               /  \     \                70
    //             20   40    90              /  \
    //                                      30   90
    //                                     /  \
    //                                   20   40
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(50); t.insert(30); t.insert(70);
        t.insert(20); t.insert(40); t.insert(90);
        t.remove(50);   // root, two children

        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        t.inorder();
        std::cout.rdbuf(old);

        assert(buf.str() == "20 30 40 70 90 \n");
        points += 2;
        std::cout << "[+2] remove (two children): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // remove — key not found (must not crash or corrupt the tree)
    // -----------------------------------------------------------------------
    {
        BST t;
        t.insert(10); t.insert(20);
        t.remove(99);

        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        t.inorder();
        std::cout.rdbuf(old);

        assert(buf.str() == "10 20 \n");
        points += 1;
        std::cout << "[+1] remove (not found, no crash): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // height
    // -----------------------------------------------------------------------
    {
        BST t;
        assert(t.height() == -1);   // empty

        t.insert(50);
        assert(t.height() == 0);    // single node

        t.insert(30); t.insert(70);
        assert(t.height() == 1);    // two levels

        t.insert(20); t.insert(40); t.insert(90);
        assert(t.height() == 2);    // three levels

        points += 1;
        std::cout << "[+1] height: PASSED\n";
    }

    // -----------------------------------------------------------------------
    // Big-tree search — 15-node perfect 4-level BST
    //
    // Insertion order builds this structure:
    //
    //               50
    //            /      \
    //          25        75
    //         /  \      /  \
    //        12   37   62   87
    //       / \  / \  / \  / \
    //      6  18 31 43 56 68 81 93
    //
    // Tests search at root (depth 1), interior nodes (depths 2–3),
    // and all eight leaves (depth 4).
    // -----------------------------------------------------------------------
    {
        BST t;
        int keys[] = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93};
        for (int k : keys) t.insert(k);

        // Every inserted key must be found.
        for (int k : keys) assert(silentSearch(t, k) == true);

        // Keys that fall between nodes must not be found.
        assert(silentSearch(t,  1) == false);  // less than minimum
        assert(silentSearch(t, 10) == false);  // between  6 and 12
        assert(silentSearch(t, 20) == false);  // between 18 and 25
        assert(silentSearch(t, 45) == false);  // between 43 and 50
        assert(silentSearch(t, 60) == false);  // between 56 and 62
        assert(silentSearch(t, 95) == false);  // greater than maximum

        points += 2;
        std::cout << "[+2] big-tree search (15 nodes, all levels, found + not-found): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // searchDepth — verify depths in a known 4-level tree
    // -----------------------------------------------------------------------
    {
        BST t;
        for (int k : {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93})
            t.insert(k);

        assert(t.searchDepth(50) == 1);   // root
        assert(t.searchDepth(25) == 2);   // level 2
        assert(t.searchDepth(75) == 2);
        assert(t.searchDepth(12) == 3);   // level 3
        assert(t.searchDepth(68) == 4);   // leaf
        assert(t.searchDepth(93) == 4);   // leaf (rightmost)
        assert(t.searchDepth(99) == -1);  // not found

        points += 1;
        std::cout << "[+1] searchDepth (known 4-level tree): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // Stress test — insert 300 keys, search every one, remove 150, re-verify
    //
    // Keys inserted: even numbers 2, 4, 6, ..., 600 (sorted order →
    // degenerate right-leaning chain — correctness must still hold).
    // Keys NOT inserted: odd numbers 1, 3, 5, ..., 599.
    // After removing every multiple of 4, the remaining even-but-not-div-by-4
    // keys must still be found and the removed keys must be gone.
    // -----------------------------------------------------------------------
    {
        BST t;
        const int N = 300;

        // Insert even numbers 2 .. 2N in sorted order (worst-case tree shape).
        for (int i = 1; i <= N; i++) t.insert(i * 2);

        // All even keys in range must be found.
        for (int i = 1; i <= N; i++) assert(silentSearch(t, i * 2) == true);

        // All odd keys in range must NOT be found.
        for (int i = 1; i <= N; i++) assert(silentSearch(t, i * 2 - 1) == false);

        // Remove every multiple of 4 (half the keys).
        for (int i = 1; i <= N; i++)
            if ((i * 2) % 4 == 0) t.remove(i * 2);

        // Multiples of 4 must now be gone; evens that are not multiples of 4
        // must still be present.
        for (int i = 1; i <= N; i++) {
            int key = i * 2;
            if (key % 4 == 0)
                assert(silentSearch(t, key) == false);
            else
                assert(silentSearch(t, key) == true);
        }

        points += 3;
        std::cout << "[+3] stress test (300 inserts, search all, remove 150, re-verify): PASSED\n";
    }

    // -----------------------------------------------------------------------
    // Final banner
    // -----------------------------------------------------------------------
    std::cout << "\n>>> Task 1 Complete! (" << points << "/17 pts) <<<\n\n";

    if (points == 17) {
        std::cout << "========================================\n";
        std::cout << "  ALL TESTS PASSED!   17/17 points\n";
        std::cout << "========================================\n";
    }

    return 0;
}
