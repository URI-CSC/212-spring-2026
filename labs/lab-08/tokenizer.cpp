// tokenizer.cpp — Tiny LLM Tokenizer
//
// Large language models (GPT-4, Llama-3, Claude) never see raw text.
// Before a prompt reaches the model, every word is converted to an integer
// token ID using a lookup table called the vocabulary.  The model then
// predicts the *next token ID* — which is why these systems are called
// "next-token predictors."
//
// This file implements a miniature version of that lookup using a BST keyed
// on strings.  The core idea is identical to Part 1: instead of integers,
// the keys are words; instead of printing a boolean result, the BST returns
// an ID.  The search is still O(log V) where V = vocabulary size.
//
// Real LLMs have 32 000 – 128 000 vocabulary entries.  Our toy vocabulary
// has 68.  The [UNK] tokens you'll see reveal exactly why larger vocabularies
// matter.
//
// Compile:  g++ -std=c++11 tokenizer.cpp -o tokenizer
// Run:      ./tokenizer
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

// ============================================================================
// TokenBST — a BST keyed on std::string, storing integer token IDs.
//
// This is the same structure from Part 1, extended to string keys.
// The insert/search logic is identical; only the comparison changes
// (string < / == / > instead of int < / == / >).
// Fully provided — read it, then use it in tokenize() below.
// ============================================================================
class TokenBST {
    struct Node {
        std::string token;
        int         id;
        Node*       left;
        Node*       right;

        Node(const std::string& t, int i)
            : token(t), id(i), left(nullptr), right(nullptr) {}
    };

    Node* root_;

    Node* insert(Node* n, const std::string& token, int id) {
        if (!n) return new Node(token, id);
        if      (token < n->token) n->left  = insert(n->left,  token, id);
        else if (token > n->token) n->right = insert(n->right, token, id);
        return n;
    }

    int lookup(Node* n, const std::string& token) const {
        if (!n) return -1;
        if (token == n->token) return n->id;
        if (token <  n->token) return lookup(n->left,  token);
        return lookup(n->right, token);
    }

    void clear(Node* n) {
        if (!n) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }

public:
    TokenBST() : root_(nullptr) {}
    ~TokenBST() { clear(root_); }

    void insert(const std::string& token, int id) {
        root_ = insert(root_, token, id);
    }

    // Returns the token ID for token, or -1 if not in the vocabulary.
    int lookup(const std::string& token) const {
        return lookup(root_, token);
    }
};

// ============================================================================
// normalize — lowercase a word and strip leading/trailing non-alpha chars.
// "Language," -> "language"   "THE" -> "the"   "'hello'" -> "hello"
// Fully provided.
// ============================================================================
static std::string normalize(const std::string& word) {
    std::string s = word;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    while (!s.empty() && !std::isalpha(static_cast<unsigned char>(s.front())))
        s.erase(s.begin());
    while (!s.empty() && !std::isalpha(static_cast<unsigned char>(s.back())))
        s.pop_back();
    return s;
}

// ============================================================================
// tokenize — TODO
//
// Split text into whitespace-separated words using std::istringstream.
// For each word:
//   1. Call normalize() to lowercase it and strip punctuation.
//   2. Skip empty strings that result from normalize() (e.g. bare punctuation).
//   3. Call vocab.lookup() to get the token ID (-1 if unknown).
//   4. Append {normalized_word, id} to the tokens vector.
//
// Return the filled tokens vector.
//
// Hint: declare  std::istringstream stream(text);  then loop with
//       while (stream >> word) { ... }
// ============================================================================
static std::vector<std::pair<std::string, int>>
tokenize(const std::string& text, const TokenBST& vocab) {
    std::vector<std::pair<std::string, int>> tokens;

    // TODO

    return tokens;
}

// ============================================================================
// Vocabulary — 66 tokens hand-picked to cover common English words and
// several AI / ML terms.  Stored in alphabetical order here for readability;
// the BST imposes its own ordering via string comparisons.
// ============================================================================
static const std::pair<const char*, int> VOCAB[] = {
    {"a", 0},           {"able", 1},        {"about", 2},
    {"after", 3},       {"all", 4},         {"also", 5},
    {"an", 6},          {"and", 7},         {"are", 8},
    {"as", 9},          {"at", 10},         {"be", 11},
    {"been", 12},       {"but", 13},        {"by", 14},
    {"can", 15},        {"data", 16},       {"do", 17},
    {"each", 18},       {"for", 19},        {"from", 20},
    {"generate", 21},   {"has", 22},        {"have", 23},
    {"he", 24},         {"her", 25},        {"his", 26},
    {"how", 27},        {"i", 28},          {"in", 29},
    {"input", 30},      {"is", 31},         {"it", 32},
    {"its", 33},        {"language", 34},   {"large", 35},
    {"learn", 36},      {"like", 37},       {"llm", 38},
    {"model", 39},      {"more", 40},       {"my", 41},
    {"neural", 42},     {"new", 43},        {"next", 44},
    {"not", 45},        {"of", 46},         {"on", 47},
    {"or", 48},         {"our", 49},        {"predict", 50},
    {"she", 51},        {"text", 52},       {"the", 53},
    {"their", 54},      {"they", 55},       {"this", 56},
    {"to", 57},         {"token", 58},      {"train", 59},
    {"transformer", 60},{"use", 61},        {"was", 62},
    {"we", 63},         {"what", 64},       {"with", 65},
    {"word", 66},       {"you", 67}
};

static const int VOCAB_SIZE = static_cast<int>(sizeof(VOCAB) / sizeof(VOCAB[0]));

// ============================================================================
// main
// ============================================================================
int main() {
    // Build the vocabulary BST.
    TokenBST vocab;
    for (int i = 0; i < VOCAB_SIZE; i++)
        vocab.insert(VOCAB[i].first, VOCAB[i].second);

    std::cout << "=== Tiny LLM Tokenizer ===\n";
    std::cout << "Vocabulary size : " << VOCAB_SIZE << " tokens  "
              << "(GPT-4 uses ~100 000)\n\n";

    // Sentences to tokenize.
    const std::string sentences[] = {
        "the large language model can predict the next word",
        "we train neural networks on text data",
        "a transformer can learn to generate new text",
        "how do LLMs use each token in the input"
    };

    for (const auto& sentence : sentences) {
        auto tokens = tokenize(sentence, vocab);

        std::cout << "Input  : \"" << sentence << "\"\n";
        std::cout << "Tokens : ";

        int known = 0;
        for (const auto& t : tokens) {
            if (t.second == -1) {
                std::cout << "[UNK] ";
            } else {
                std::cout << t.first << "(" << t.second << ") ";
                ++known;
            }
        }

        int total = static_cast<int>(tokens.size());
        double pct = total ? (100.0 * known / total) : 0.0;

        std::cout << "\nCoverage: " << known << "/" << total
                  << " tokens recognized  ("
                  << std::fixed << std::setprecision(0) << pct << "%)\n\n";
    }

    std::cout << "Each lookup is O(log " << VOCAB_SIZE << ") = ~6 comparisons.\n"
              << "A linear scan would need up to " << VOCAB_SIZE << ".\n"
              << "Scale to GPT-4's vocabulary and the gap becomes enormous.\n";

    return 0;
}
