#include "linkedlist.h"
#include <cassert>
#include <iostream>

int main() {
    int  points = 0;
    bool caught = false;

    // -----------------------------------------------------------------------
    // Task 1: SLList Core Operations
    // -----------------------------------------------------------------------
    std::cout << "=== Task 1: SLList Core Operations ===" << std::endl;

    // push_back, front, back
    {
        SLList lst;
        lst.push_back(10);
        lst.push_back(20);
        lst.push_back(30);
        assert(lst.front() == 10);   // first element pushed is at the front
        assert(lst.back()  == 30);   // last element pushed is at the back
        assert(lst.size()  == 3);
        points += 2;
        std::cout << "[+2] push_back + front/back: PASSED" << std::endl;
    }

    // push_front
    {
        SLList lst;
        lst.push_front(30);
        lst.push_front(20);
        lst.push_front(10);          // 10 pushed last => it is now the front
        assert(lst.front() == 10);
        assert(lst.back()  == 30);
        assert(lst.size()  == 3);

        // mix push_front and push_back
        SLList lst2;
        lst2.push_back(5);
        lst2.push_front(1);
        lst2.push_back(9);
        assert(lst2.front() == 1);
        assert(lst2.back()  == 9);
        assert(lst2.size()  == 3);
        points += 1;
        std::cout << "[+1] push_front: PASSED" << std::endl;
    }

    // size and empty
    {
        SLList lst;
        assert(lst.empty() == true);
        assert(lst.size()  == 0);
        lst.push_back(5);
        assert(lst.empty() == false);
        assert(lst.size()  == 1);
        lst.push_back(10);
        assert(lst.size()  == 2);
        lst.pop_back();
        assert(lst.size()  == 1);
        lst.pop_back();
        assert(lst.empty() == true);
        assert(lst.size()  == 0);
        points += 1;
        std::cout << "[+1] size and empty: PASSED" << std::endl;
    }

    // pop_back
    {
        SLList lst;
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);

        lst.pop_back();
        assert(lst.back() == 2);
        assert(lst.size() == 2);

        lst.pop_back();
        assert(lst.back() == 1);
        assert(lst.size() == 1);

        lst.pop_back();              // remove the only remaining element
        assert(lst.empty() == true);

        // push again after emptying to verify head/tail are properly reset
        lst.push_back(42);
        assert(lst.front() == 42);
        assert(lst.back()  == 42);
        assert(lst.size()  == 1);
        points += 2;
        std::cout << "[+2] pop_back: PASSED" << std::endl;
    }

    // exceptions: underflow on empty list (provided methods)
    {
        SLList lst;

        caught = false;
        try { lst.front(); } catch (const std::underflow_error&) { caught = true; }
        assert(caught);
        std::cout << "    front() on empty list: exception PASSED" << std::endl;

        caught = false;
        try { lst.back(); } catch (const std::underflow_error&) { caught = true; }
        assert(caught);
        std::cout << "    back() on empty list: exception PASSED" << std::endl;

        caught = false;
        try { lst.pop_back(); } catch (const std::underflow_error&) { caught = true; }
        assert(caught);
        std::cout << "    pop_back() on empty list: exception PASSED" << std::endl;

        points += 1;
        std::cout << "[+1] exceptions (provided methods): PASSED" << std::endl;
    }

    std::cout << "\n    --- provided methods OK: now testing your TODO methods ---\n"
              << std::endl;

    // -----------------------------------------------------------------------
    // The two sections below test pop_front() and clear() -- the methods you
    // must implement.  If your program aborts here with an assertion failure,
    // that method is not yet correctly implemented.
    // -----------------------------------------------------------------------

    // pop_front (TODO)
    {
        SLList lst;
        lst.push_back(10);
        lst.push_back(20);
        lst.push_back(30);

        lst.pop_front();
        assert(lst.front() == 20);   // 10 removed; new front is 20
        assert(lst.size()  == 2);

        lst.pop_front();
        assert(lst.front() == 30);
        assert(lst.size()  == 1);

        lst.pop_front();             // remove the only remaining element
        assert(lst.empty() == true);

        // push again after emptying to verify head/tail are properly reset
        lst.push_back(99);
        assert(lst.front() == 99);
        assert(lst.back()  == 99);
        assert(lst.size()  == 1);

        // interleave push_front and pop_front
        SLList lst2;
        lst2.push_front(5);
        lst2.push_front(3);
        lst2.push_front(1);          // list: 1 -> 3 -> 5
        lst2.pop_front();            // list: 3 -> 5
        assert(lst2.front() == 3);
        assert(lst2.size()  == 2);
        lst2.pop_front();            // list: 5
        assert(lst2.front() == 5);
        assert(lst2.back()  == 5);

        points += 2;
        std::cout << "[+2] pop_front: PASSED" << std::endl;

        // pop_front on empty list must throw
        caught = false;
        SLList empty_lst;
        try { empty_lst.pop_front(); } catch (const std::underflow_error&) { caught = true; }
        assert(caught);
        std::cout << "    pop_front() on empty list: exception PASSED" << std::endl;
    }

    // clear (TODO)
    {
        SLList lst;
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);

        lst.clear();
        assert(lst.empty() == true);
        assert(lst.size()  == 0);

        // the list must be fully usable again after clearing
        lst.push_back(99);
        assert(lst.front() == 99);
        assert(lst.back()  == 99);
        assert(lst.size()  == 1);

        // clear a second time (single-element)
        lst.clear();
        assert(lst.empty() == true);

        // clear an already-empty list must not crash
        lst.clear();
        assert(lst.empty() == true);

        points += 1;
        std::cout << "[+1] clear: PASSED" << std::endl;
    }

    // -----------------------------------------------------------------------
    // Final banner
    // -----------------------------------------------------------------------
    std::cout << "\n>>> Task 1 Complete! (" << points << "/10 pts) <<<\n" << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "  ALL TESTS PASSED!" << std::endl;
    std::cout << "  Total: " << points << "/10 points" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
