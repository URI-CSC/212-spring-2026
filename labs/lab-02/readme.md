# Lab 02 - Memory Management in C++

## Overview

In this lab, you will explore fundamental concepts of memory management in C++ by building a March Madness bracket manager. You'll start with exercises that illustrate the difference between *stack* and *heap* allocation, observing how object lifetimes differ in each memory region. Then, you'll design and implement two classes: `Team`, which represents a tournament participant, and `Bracket`, which manages the field of teams. Through careful use of constructors and destructors, you'll gain hands-on experience with the resource acquisition and release patterns that underpin every dynamic data structure you'll encounter in this course.

All tasks are expected to be completed during the lab session. However, if you run out of time, please make every effort to complete them outside of lab hours.

> [!CAUTION]
> For this lab, **AVOID** using large language models (LLMs) to **generate code or solutions** for the exercises. In particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE while working on this lab. You may ask specific questions to LLMs about concepts or language features/syntax, but we strongly discourage using LLMs to write code for you.

## Task 1: Stack vs. Heap

Before building the bracket manager, you must understand where objects live in memory and who is responsible for cleaning them up. In this task, you will experiment with a simple class to observe the differences between stack and heap allocation. The stack is a region of memory that stores local variables. Objects allocated on the stack are automatically destroyed when they go out of scope. The heap is a larger pool of memory used for dynamic allocation; objects created on the heap persist until explicitly deleted by the programmer.

### The `School` Class

Create a file named `school.cpp` and add the following code:
```cpp
#include <iostream>
#include <string>

class School {
    public:
        std::string name;
        int seed;
        School(std::string n, int s) : name(n), seed(s) { 
            std::cout << "#" << seed << " " << name << " has entered the tournament (address: " << this << ")" << std::endl; 
        }
        ~School() { 
            std::cout << "#" << seed << " " << name << " has been eliminated" << std::endl; 
        }
};

int main() {
    std::cout << "=== Stack Allocation ===" << std::endl;
    {
        std::cout << "Inside inner scope ... (defined by the curly braces)" << std::endl;
        School s1("Duke", 1);
        School s2("UConn", 2);
        std::cout << "School objects created on stack ..." << std::endl;
    }
    std::cout << "Exited inner scope ... objects s1 and s2 are out of scope now." << std::endl;

    std::cout << "=== Heap Allocation ===" << std::endl;
    School* s3 = new School("Gonzaga", 3);
    std::cout << "School object created on heap ..." << std::endl;
    School* s4 = new School("Purdue", 4);
    std::cout << "School object created on heap ..." << std::endl;

    std::cout << "=== Deleting s3 only! ===" << std::endl;
    delete s3;

    std::cout << "=== Program ending ===" << std::endl;
    return 0;
}
```

### Exercises

1. Compile and run the program:
```bash
$ g++ -std=c++11 -Wall school.cpp -o school
$ ./school
```

2. Observe the output carefully and answer the following questions in a file named `answers.txt`:
   - When do the destructors for `s1` and `s2` run? Why?
   - Does the destructor for `s4` (Purdue) ever run? What is the consequence of this?
   - What is the technical term for memory that is allocated but never freed?
   - In the stack allocation section, which school gets eliminated first — Duke or UConn? Why does this order occur? (hint: think about how the stack works)

3. Modify the program to properly delete `s4` before the program ends. Recompile and verify that all destructors now run.

> [!NOTE]
> The key insight here is **ownership**: stack objects are automatically managed by the compiler, while heap objects require explicit management by the programmer. Forgetting to `delete` heap memory causes **memory leaks**. 

## Task 2: The `Team` Class

Now you will design a class that represents a team in the NCAA tournament. Each team has a name, seed, and region.

### Specification

Create a file named `team.h` with the following class declaration:
```cpp
#ifndef TEAM_H
#define TEAM_H

#include <iostream>
#include <string>

class Team {
private:
    std::string name;
    int seed;
    std::string region;
    int wins;

public:
    // Constructor: initializes team with name, seed, and region
    // Sets wins to 0
    // Prints: "[ENTERED] #<seed> <name> (<region>)"
    Team(std::string team_name, int team_seed, std::string team_region);

    // Destructor: prints "[ELIMINATED] #<seed> <name> - Tournament over after <wins> wins"
    ~Team();

    // Returns the team's name
    std::string get_name() const;

    // Returns the team's seed
    int get_seed() const;

    // Returns the team's region
    std::string get_region() const;

    // Returns the number of wins
    int get_wins() const;

    // Increments win count by 1 (team advances)
    void advance();
};

#endif
```

### Implementation

Create a file named `team.cpp` and implement the member functions declared above:
```cpp
#include "team.h"

Team::Team(std::string team_name, int team_seed, std::string team_region) {
    // TODO: Initialize name, seed, region
    // TODO: Set wins to 0
    // TODO: Print "[ENTERED] #<seed> <name> (<region>)"
}

Team::~Team() {
    // TODO: Print "[ELIMINATED] #<seed> <name> - Tournament over after <wins> wins"
}

std::string Team::get_name() const {
    // TODO: Return the team's name
}

int Team::get_seed() const {
    // TODO: Return the team's seed
}

std::string Team::get_region() const {
    // TODO: Return the team's region
}

int Team::get_wins() const {
    // TODO: Return wins
}

void Team::advance() {
    // TODO: Increment wins by 1
    // TODO: Print "[ADVANCE] <name> wins! (<wins> total wins)"
}
```

### Testing

Create a file named `test_team.cpp` to verify your implementation:
```cpp
#include "team.h"
#include <cassert>

int main() {
    std::cout << "=== Stack-allocated team ===" << std::endl;
    {
        Team t1("UConn", 1, "East");
        // as t1 is an object, we use dot notation to access its members
        assert(t1.get_name() == "UConn");
        assert(t1.get_seed() == 1);
        assert(t1.get_region() == "East");
        assert(t1.get_wins() == 0);
        
        t1.advance();  // Win first round
        t1.advance();  // Win second round
        assert(t1.get_wins() == 2);
    }
    std::cout << "Scope ended.\n" << std::endl;

    std::cout << "=== Heap-allocated team ===" << std::endl;
    Team* t2 = new Team("Duke", 4, "West");
    // as t2 is a pointer, we use arrow notation to access its members
    assert(t2->get_name() == "Duke");
    assert(t2->get_seed() == 4);
    assert(t2->get_region() == "West");
    assert(t2->get_wins() == 0);
    t2->advance();
    std::cout << t2->get_name() << " has " << t2->get_wins() << " win(s)" << std::endl;
    
    std::cout << "\n=== Duke gets eliminated ===" << std::endl;
    delete t2;

    std::cout << "\n=== Program ending ===" << std::endl;
    return 0;
}
```

Compile and run:
```bash
$ g++ -std=c++11 -Wall -Werror team.cpp test_team.cpp -o test_team
$ ./test_team
```

You should see `ENTERED` messages when teams are created and `ELIMINATED` messages when they are destroyed. The stack-allocated team should be eliminated automatically when the scope ends.

## Task 3: The `Bracket` Class

Now you will implement a class that manages the tournament bracket. The `Bracket` owns all teams — when the tournament ends (destructor runs), all remaining teams are eliminated.

### Specification

Create a file named `bracket.h`:
```cpp
#ifndef BRACKET_H
#define BRACKET_H

#include "team.h"

class Bracket {
private:
    static const int MAX_TEAMS = 16;  // Sweet 16 for simplicity
    int team_count; // Current number of teams in bracket
    Team* teams[MAX_TEAMS]; // We are using raw arrays here to make ownership and memory layout explicit. std::vector could also manage this for us.

public:
    // Constructor: initializes all pointers to nullptr, team_count to 0
    // Prints: "=== NCAA Tournament Bracket Created ==="
    Bracket();

    // Destructor: eliminates all remaining teams
    // Prints: "=== Tournament Complete ===" after cleanup
    ~Bracket();

    // Adds a team to the bracket, takes ownership of the pointer
    // Returns true if successful, false if bracket is full
    bool add_team(Team* t);

    // Eliminates team by name, returns true if found and removed
    bool eliminate(std::string name);

    // Returns the current number of teams remaining
    int get_team_count() const;

    // Returns total wins across all remaining teams
    int get_total_wins() const;

    // Finds and returns pointer to team by name, nullptr if not found
    // Note: bracket retains ownership, caller should NOT delete
    Team* find_team(std::string name) const;

    // Prints all remaining teams in the bracket
    void print_bracket() const;
};

#endif
```

### Implementation

Create a file named `bracket.cpp` and implement the member functions:
```cpp
#include "bracket.h"

Bracket::Bracket() {
    // TODO: Initialize all elements of teams[] to nullptr
    // TODO: Set team_count to 0
    // TODO: Print "=== NCAA Tournament Bracket Created ==="
}

Bracket::~Bracket() {
    // TODO: Iterate through teams[]
    // TODO: Delete any non-null teams (they will print "ELIMINATED")
    // TODO: Print "=== Tournament Complete ==="
}

bool Bracket::add_team(Team* t) {
    // TODO: Check if bracket is full (team_count == MAX_TEAMS), return false if so
    // TODO: Find first nullptr slot in teams[]
    // TODO: Store the team pointer in that slot
    // TODO: Increment team_count
    // TODO: Return true
}

bool Bracket::eliminate(std::string name) {
    // TODO: Search teams[] for a team with matching name
    // TODO: If found: delete the team, set slot to nullptr, decrement team_count, return true
    // TODO: If not found: return false
}

int Bracket::get_team_count() const {
    // TODO: Return team_count
}

int Bracket::get_total_wins() const {
    // TODO: Iterate through teams[], sum up wins from non-null teams
    // TODO: Return total
}

Team* Bracket::find_team(std::string name) const {
    // TODO: Search teams[] for matching name
    // TODO: Return pointer if found, nullptr otherwise
}

void Bracket::print_bracket() const {
    std::cout << "\n--- Teams Remaining: " << team_count << " ---" << std::endl;
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (teams[i] != nullptr) {
            std::cout << "  #" << teams[i]->get_seed() << " " 
                      << teams[i]->get_name() 
                      << " (" << teams[i]->get_region() << ") - " 
                      << teams[i]->get_wins() << " wins" << std::endl;
        }
    }
    std::cout << std::endl;
}
```

> [!NOTE]
> Once `add_team()` is called, the `Bracket` **owns** that team. The caller must not delete the pointer themselves (doing so would cause a double-free error when the bracket later tries to delete the same memory. Only `eliminate()` or the bracket's destructor should do that. This is the concept of **ownership transfer**.

## Task 4: Tournament Simulation

Create a file named `tournament.cpp` to simulate a mini March Madness tournament:
```cpp
#include "bracket.h"
#include <cassert>

void simulate_sweet_sixteen() {
    std::cout << "========================================" << std::endl;
    std::cout << "      MARCH MADNESS SIMULATION" << std::endl;
    std::cout << "========================================\n" << std::endl;

    Bracket bracket;
    assert(bracket.get_team_count() == 0);

    std::cout << "\n--- Sweet 16 Teams Entering ---\n" << std::endl;
    
    // East Region
    bracket.add_team(new Team("UConn", 1, "East"));
    bracket.add_team(new Team("Iowa State", 2, "East"));
    bracket.add_team(new Team("Illinois", 3, "East"));
    bracket.add_team(new Team("Auburn", 4, "East"));
    
    // West Region
    bracket.add_team(new Team("North Carolina", 1, "West"));
    bracket.add_team(new Team("Arizona", 2, "West"));
    bracket.add_team(new Team("Baylor", 3, "West"));
    bracket.add_team(new Team("Duke", 4, "West"));
    
    // South Region
    bracket.add_team(new Team("Houston", 1, "South"));
    bracket.add_team(new Team("Marquette", 2, "South"));
    bracket.add_team(new Team("Kentucky", 3, "South"));
    bracket.add_team(new Team("Purdue", 4, "South"));
    
    // Midwest Region
    bracket.add_team(new Team("Kansas", 1, "Midwest"));
    bracket.add_team(new Team("Tennessee", 2, "Midwest"));
    bracket.add_team(new Team("Creighton", 3, "Midwest"));
    bracket.add_team(new Team("NC State", 11, "Midwest"));  // Cinderella!
    
    assert(bracket.get_team_count() == 16);
    bracket.print_bracket();

    std::cout << "--- Sweet 16 Games ---\n" << std::endl;
    
    // Simulate Sweet 16 - 8 games, 8 winners advance
    Team* uconn = bracket.find_team("UConn");
    if (uconn) uconn->advance();
    
    Team* auburn = bracket.find_team("Auburn");
    if (auburn) auburn->advance();
    
    Team* north_carolina = bracket.find_team("North Carolina");
    if (north_carolina) north_carolina->advance();
    
    Team* duke = bracket.find_team("Duke");
    if (duke) duke->advance();
    
    Team* houston = bracket.find_team("Houston");
    if (houston) houston->advance();
    
    Team* purdue = bracket.find_team("Purdue");
    if (purdue) purdue->advance();
    
    Team* kansas = bracket.find_team("Kansas");
    if (kansas) kansas->advance();
    
    Team* nc_state = bracket.find_team("NC State");
    if (nc_state) nc_state->advance();  // Cinderella run continues!
    
    // Eliminate Sweet 16 losers
    bracket.eliminate("Iowa State");
    bracket.eliminate("Illinois");
    bracket.eliminate("Arizona");
    bracket.eliminate("Baylor");
    bracket.eliminate("Marquette");
    bracket.eliminate("Kentucky");
    bracket.eliminate("Tennessee");
    bracket.eliminate("Creighton");
    
    assert(bracket.get_team_count() == 8);
    std::cout << "\n>>> Elite 8 Set! <<<" << std::endl;
    bracket.print_bracket();
    
    std::cout << "--- Elite 8 Games ---\n" << std::endl;
    
    // Simulate Elite 8 - 4 games, 4 winners advance
    uconn = bracket.find_team("UConn");
    if (uconn) uconn->advance();
    
    purdue = bracket.find_team("Purdue");
    if (purdue) purdue->advance();
    
    kansas = bracket.find_team("Kansas");
    if (kansas) kansas->advance();
    
    nc_state = bracket.find_team("NC State");
    if (nc_state) nc_state->advance();  // Miracle run!
    
    // Eliminate Elite 8 losers
    bracket.eliminate("Auburn");
    bracket.eliminate("North Carolina");
    bracket.eliminate("Houston");
    bracket.eliminate("Duke");
    
    assert(bracket.get_team_count() == 4);
    std::cout << "\n>>> Final Four Set! <<<" << std::endl;
    bracket.print_bracket();
    
    std::cout << "Total wins among remaining teams: " << bracket.get_total_wins() << std::endl;

    // NOTE: We stop at the Final Four for this simulation.
    // The remaining 4 teams will be cleaned up by the Bracket destructor,
    // demonstrating that the destructor properly frees all remaining memory.

    std::cout << "\n--- Attempting to eliminate team not in bracket ---\n" << std::endl;

    bool result = bracket.eliminate("Kentucky");
    assert(result == false);
    std::cout << "Kentucky was already eliminated.\n" << std::endl;

    std::cout << "--- Tournament Ending (remaining teams eliminated) ---\n" << std::endl;
}

void test_bracket_capacity() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      TESTING BRACKET CAPACITY" << std::endl;
    std::cout << "========================================\n" << std::endl;

    Bracket bracket;
    
    // Fill bracket to capacity (16 teams)
    const char* team_names[] = {
        "Team01", "Team02", "Team03", "Team04",
        "Team05", "Team06", "Team07", "Team08",
        "Team09", "Team10", "Team11", "Team12",
        "Team13", "Team14", "Team15", "Team16"
    };
    
    for (int i = 0; i < 16; i++) {
        bool added = bracket.add_team(new Team(team_names[i], (i % 4) + 1, "Test"));
        assert(added == true);
    }
    assert(bracket.get_team_count() == 16);
    std::cout << "\nBracket is at capacity!\n" << std::endl;

    // Try to add 17th team
    Team* extra = new Team("ExtraTeam", 1, "None");
    bool added = bracket.add_team(extra);
    
    if (!added) {
        std::cout << "Bracket full! ExtraTeam cannot enter tournament.\n" << std::endl;
        delete extra;  // We must clean up since bracket didn't take ownership!
    }

    std::cout << "--- Full bracket tournament ending ---\n" << std::endl;
}

int main() {
    simulate_sweet_sixteen();
    test_bracket_capacity();
    
    std::cout << "========================================" << std::endl;
    std::cout << "      ALL TESTS PASSED!" << std::endl;
    std::cout << "========================================" << std::endl;
    return 0;
}
```

Compile and run:
```bash
$ g++ -std=c++11 -Wall -Werror team.cpp bracket.cpp tournament.cpp -o tournament
$ ./tournament
```

### Verification

Count the `ENTERED` and `ELIMINATED` messages in your output. **They must match exactly.** If you see more entries than eliminations, you have ghost teams — a memory leak! Every team that enters must eventually be eliminated.

## Task 5: Bug Hunt

A classmate tried to implement their own bracket system but introduced some bugs that cause memory leaks. Your task is to identify and fix them.

Create a file named `buggy_bracket.cpp` with the following **buggy** implementation:
```cpp
#include <iostream>
#include <string>

class SimpleTeam {
    public:
        std::string name;
        int seed;
        SimpleTeam(std::string n, int s) : name(n), seed(s) { 
            std::cout << "[ENTERED] #" << seed << " " << name << std::endl; 
        }
        ~SimpleTeam() { 
            std::cout << "[ELIMINATED] #" << seed << " " << name << std::endl; 
        }
};

class BuggyBracket {
    private:
        static const int CAPACITY = 8;
        SimpleTeam* teams[CAPACITY];
        int count;

    public:
        BuggyBracket() : count(0) {
            for (int i = 0; i < CAPACITY; i++) {
                teams[i] = nullptr;
            }
            std::cout << "Tournament started!" << std::endl;
        }

        ~BuggyBracket() {
            // BUG #1: Something is missing here!
            std::cout << "Tournament ended!" << std::endl;
        }

        bool add_team(SimpleTeam* t) {
            if (count >= CAPACITY) return false;
            for (int i = 0; i < CAPACITY; i++) {
                if (teams[i] == nullptr) {
                    teams[i] = t;
                    count++;
                    return true;
                }
            }
            return false;
        }

        bool eliminate(std::string name) {
            for (int i = 0; i < CAPACITY; i++) {
                if (teams[i] != nullptr && teams[i]->name == name) {
                    // BUG #2: Something is wrong here!
                    teams[i] = nullptr;
                    count--;
                    return true;
                }
            }
            return false;
        }
};

int main() {
    std::cout << "=== Creating buggy bracket ===\n" << std::endl;
    BuggyBracket bracket;

    bracket.add_team(new SimpleTeam("UConn", 1));
    bracket.add_team(new SimpleTeam("Duke", 4));
    bracket.add_team(new SimpleTeam("Kentucky", 3));
    bracket.add_team(new SimpleTeam("Kansas", 1));

    std::cout << "\n=== Duke eliminated in upset! ===\n" << std::endl;
    bracket.eliminate("Duke");

    std::cout << "\n=== Tournament ending ===\n" << std::endl;
    return 0;
}
```

### Exercises

1. Compile and run the buggy program:
```bash
$ g++ -std=c++11 -Wall -Werror buggy_bracket.cpp -o buggy_bracket
$ ./buggy_bracket
```

2. Count the `ENTERED` and `ELIMINATED` messages. How many teams became ghosts (leaked)?

3. There are **two bugs** in this code. Identify them and write your answers in `answers.txt`:
   - Bug #1: What is missing in the destructor?
   - Bug #2: What is missing in `eliminate()` before setting the pointer to `nullptr`?

4. Fix both bugs in `buggy_bracket.cpp` and verify that all teams are properly eliminated (entered count equals eliminated count).

> [!WARNING]
> Memory leaks are insidious — they don't cause immediate crashes, but in long-running programs they gradually consume all available memory. In our tournament metaphor, imagine running March Madness simulations for years with ghost teams accumulating in memory. Eventually, you run out of space! Learning to spot and fix these issues now will save you hours of debugging later in the course.

## Libraries

For solving the exercises in this lab, you will need the following standard library components:
```cpp
#include <iostream>   // console input/output (std::cin, std::cout)
#include <string>     // string handling for team names and regions
#include <cassert>    // assert statements for testing
```

## Submission

After you have completed all the tasks, submit the files below to Gradescope. Your code should be well-formatted and easy to read. Make sure to test your code thoroughly before submitting it. Full credit for this lab requires **attendance**, **collaboration with your group**, and **active participation**. No remote submissions will be accepted. If you have any questions, please ask the instructor or the TA for help.

- `answers.txt`
- `team.h`
- `team.cpp`
- `bracket.h`
- `bracket.cpp`
- `tournament.cpp`
- `buggy_bracket.cpp` (with your fixes)
