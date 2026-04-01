// unittester.cpp: Lab 07: Sets and Maps
//
// Compile:
//   g++ -std=c++11 -Wall -Werror unittester.cpp -o tester
// Run:
//   ./tester
//
// How it works:
//   #define TESTING suppresses the main() in each source file (via the
//   #ifndef TESTING guard in the skeletons), then includes them directly so
//   their functions are callable here without linking separate object files.

#define TESTING
#include "friends.cpp"
#include "roman.cpp"
#include "morse.cpp"
#include "topk.cpp"

#include <iostream>
#include <string>

// --- test harness ------------------------------------------------------------

static int passed = 0;
static int failed = 0;

void check(bool condition, const std::string& name) {
    if (condition) {
        std::cout << "  PASS  " << name << "\n";
        ++passed;
    } else {
        std::cout << "  FAIL  " << name << "\n";
        ++failed;
    }
}

// --- Morse helpers (reference implementations used by the tester itself) -----

// Builds the encode map (same table as morse.cpp) so the tester can construct
// the decode map independently of the student's buildDecodeMap implementation.
std::map<char, std::string> referenceEncodeMap() {
    std::map<char, std::string> m;
    m['A']=".-";   m['B']="-..."; m['C']="-.-."; m['D']="-..";
    m['E']=".";    m['F']="..-."; m['G']="--.";  m['H']="....";
    m['I']="..";   m['J']=".---"; m['K']="-.-";  m['L']=".-..";
    m['M']="--";   m['N']="-.";   m['O']="---";  m['P']=".--.";
    m['Q']="--.-"; m['R']=".-.";  m['S']="...";  m['T']="-";
    m['U']="..-";  m['V']="...-"; m['W']=".--";  m['X']="-..-";
    m['Y']="-.--"; m['Z']="--..";
    return m;
}

// Builds the reference decode map by flipping the encode map.
std::map<std::string, char> referenceDecodeMap() {
    auto enc = referenceEncodeMap();
    std::map<std::string, char> dec;
    for (const auto& e : enc) dec[e.second] = e.first;
    return dec;
}

// --- Part 1: friends ---------------------------------------------------------

void testFriends() {
    std::cout << "\n--- Part 1: Friends (Sets) ---\n";

    std::set<std::string> A = {"Bob", "Carol", "Dave", "Eve"};
    std::set<std::string> B = {"Carol", "Dave", "Frank", "Grace"};
    std::set<std::string> empty;

    // intersect
    check(intersect(A, B) == std::set<std::string>({"Carol", "Dave"}),
          "intersect: common friends");
    check(intersect(A, empty).empty(),
          "intersect: anything ∩ empty = empty");
    check(intersect(empty, B).empty(),
          "intersect: empty ∩ anything = empty");
    check(intersect(A, A) == A,
          "intersect: A ∩ A = A");

    // difference
    check(difference(A, B) == std::set<std::string>({"Bob", "Eve"}),
          "difference: A − B");
    check(difference(B, A) == std::set<std::string>({"Frank", "Grace"}),
          "difference: B − A");
    check(difference(A, A).empty(),
          "difference: A − A = empty");
    check(difference(A, empty) == A,
          "difference: A − empty = A");
    check(difference(empty, A).empty(),
          "difference: empty − A = empty");

    // unionOf
    std::set<std::string> full = {"Bob", "Carol", "Dave", "Eve", "Frank", "Grace"};
    check(unionOf(A, B) == full,
          "union: A ∪ B contains all elements");
    check(unionOf(A, empty) == A,
          "union: A ∪ empty = A");
    check(unionOf(empty, B) == B,
          "union: empty ∪ B = B");
    check(unionOf(A, A) == A,
          "union: A ∪ A = A (no duplicates)");
}

// --- Part 2: Roman numerals --------------------------------------------------

void testRoman() {
    std::cout << "\n--- Part 2: Roman Numerals (Maps) ---\n";

    // single symbols
    check(decodeRoman("I") == 1,    "I = 1");
    check(decodeRoman("V") == 5,    "V = 5");
    check(decodeRoman("X") == 10,   "X = 10");
    check(decodeRoman("L") == 50,   "L = 50");
    check(decodeRoman("C") == 100,  "C = 100");
    check(decodeRoman("D") == 500,  "D = 500");
    check(decodeRoman("M") == 1000, "M = 1000");

    // purely additive
    check(decodeRoman("III")  == 3,  "III = 3");
    check(decodeRoman("VIII") == 8,  "VIII = 8");
    check(decodeRoman("XXX")  == 30, "XXX = 30");

    // subtractive pairs
    check(decodeRoman("IV") == 4,   "IV = 4");
    check(decodeRoman("IX") == 9,   "IX = 9");
    check(decodeRoman("XL") == 40,  "XL = 40");
    check(decodeRoman("XC") == 90,  "XC = 90");
    check(decodeRoman("CD") == 400, "CD = 400");
    check(decodeRoman("CM") == 900, "CM = 900");

    // mixed
    check(decodeRoman("XIV")     == 14,   "XIV = 14");
    check(decodeRoman("XLII")    == 42,   "XLII = 42");
    check(decodeRoman("XCIX")    == 99,   "XCIX = 99");
    check(decodeRoman("CDXLIV")  == 444,  "CDXLIV = 444");
    check(decodeRoman("MCMXCIV") == 1994, "MCMXCIV = 1994");
    check(decodeRoman("MMXXVI")  == 2026, "MMXXVI = 2026");
}

// --- Part 3: Morse code ------------------------------------------------------

void testMorse() {
    std::cout << "\n--- Part 3: Morse Code (Bidirectional Maps) ---\n";

    auto enc = referenceEncodeMap();
    // Reference decode map: used to test encodeMessage and decodeMessage
    // independently of the student's buildDecodeMap implementation.
    auto refDec = referenceDecodeMap();

    // buildDecodeMap: tests the student's reverse-map construction
    auto studentDec = buildDecodeMap(enc);
    check(studentDec.size() == 26,    "buildDecodeMap: all 26 letters present");
    check(studentDec["..."]  == 'S',  "buildDecodeMap: ... -> S");
    check(studentDec["---"]  == 'O',  "buildDecodeMap: --- -> O");
    check(studentDec[".-"]   == 'A',  "buildDecodeMap: .- -> A");
    check(studentDec["-"]    == 'T',  "buildDecodeMap: - -> T");

    // encodeMessage
    check(encodeMessage("E", enc)           == ".",
          "encode: single letter E");
    check(encodeMessage("SOS", enc)         == "... --- ...",
          "encode: SOS");
    check(encodeMessage("HI", enc)          == ".... ..",
          "encode: HI (two letters)");
    check(encodeMessage("HELLO WORLD", enc) == ".... . .-.. .-.. --- / .-- --- .-. .-.. -..",
          "encode: HELLO WORLD");

    // decodeMessage (uses reference decode map to isolate from buildDecodeMap)
    check(decodeMessage(".", refDec)                                           == "E",
          "decode: single dot");
    check(decodeMessage("... --- ...", refDec)                                 == "SOS",
          "decode: SOS");
    check(decodeMessage(".... . .-.. .-.. --- / .-- --- .-. .-.. -..", refDec) == "HELLO WORLD",
          "decode: HELLO WORLD");

    // roundtrip encode -> decode (uses student's decode map end-to-end)
    auto roundtrip = [&](const std::string& s) -> bool {
        return decodeMessage(encodeMessage(s, enc), studentDec) == s;
    };
    check(roundtrip("SOS"),             "roundtrip: SOS");
    check(roundtrip("HELLO WORLD"),     "roundtrip: HELLO WORLD");
    check(roundtrip("QUICK BROWN FOX"), "roundtrip: QUICK BROWN FOX");
    check(roundtrip("A"),               "roundtrip: single letter");
}

// --- Part 4: Top-K words -----------------------------------------------------

void testTopK() {
    std::cout << "\n--- Part 4: Top-K Words (Maps + Sorting) ---\n";

    // buildFreqMap: basic counts
    auto f1 = buildFreqMap("to be or not to be that is the question to be");
    check(f1["be"]       == 3, "freq: 'be' appears 3 times");
    check(f1["to"]       == 3, "freq: 'to' appears 3 times");
    check(f1["or"]       == 1, "freq: 'or' appears 1 time");
    check(f1["question"] == 1, "freq: 'question' appears 1 time");
    check(f1.count("xyz") == 0, "freq: unknown word is absent");

    // normalization: case folding
    auto f2 = buildFreqMap("The the THE");
    check(f2["the"] == 3, "normalize: 'The', 'the', 'THE' all count as 'the'");

    // normalization: punctuation stripping
    auto f3 = buildFreqMap("hello, world! hello.");
    check(f3["hello"] == 2, "normalize: 'hello,' and 'hello.' both become 'hello'");
    check(f3["world"] == 1, "normalize: 'world!' becomes 'world'");

    // topK: result size
    auto r1 = topK(f1, 3);
    check(r1.size() == 3, "topk: returns exactly k entries when vocab >= k");

    // topK: sorted descending
    check(r1[0].second >= r1[1].second, "topk: [0] count >= [1] count");
    check(r1[1].second >= r1[2].second, "topk: [1] count >= [2] count");

    // topK: k larger than vocabulary
    auto fSmall = buildFreqMap("hello world");
    auto rSmall = topK(fSmall, 100);
    check(rSmall.size() == 2, "topk: k > vocab size returns all words");

    // topK: ties broken alphabetically
    // "bat", "cat", "hat" each appear once; alphabetical order must be preserved
    auto fTies = buildFreqMap("cat bat hat");
    auto rTies = topK(fTies, 3);
    check(rTies.size() == 3 &&
          rTies[0].first == "bat" &&
          rTies[1].first == "cat" &&
          rTies[2].first == "hat",
          "topk: ties broken alphabetically");

    // topK: the top word is correct
    auto r2 = topK(f1, 1);
    check(r2.size() == 1 && r2[0].second == 3,
          "topk: top word has count 3");
}

// --- main --------------------------------------------------------------------

int main() {
    testFriends();
    testRoman();
    testMorse();
    testTopK();

    int total = passed + failed;
    std::cout << "\nResults: " << passed << "/" << total << " passed\n";

    return (failed == 0) ? 0 : 1;
}
