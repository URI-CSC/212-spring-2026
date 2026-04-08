#include "bst.h"

// ============================================================================
// PROVIDED — do not modify the methods in this section.
// ============================================================================

void BST::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void BST::inorder(Node* node) const {
    if (!node) return;
    inorder(node->left);
    std::cout << node->key << " ";
    inorder(node->right);
}

void BST::inorder() const {
    inorder(root_);
    std::cout << "\n";
}

int BST::height(Node* node) const {
    if (!node) return -1;
    int lh = height(node->left);
    int rh = height(node->right);
    return 1 + (lh > rh ? lh : rh);
}

int BST::height() const {
    return height(root_);
}

BST::Node* BST::minNode(Node* node) const {
    while (node->left)
        node = node->left;
    return node;
}

int BST::searchDepth(Node* node, int key, int depth) const {
    if (!node) return -1;
    if (key == node->key) return depth;
    if (key <  node->key) return searchDepth(node->left,  key, depth + 1);
    return searchDepth(node->right, key, depth + 1);
}

int BST::searchDepth(int key) const {
    return searchDepth(root_, key, 1);
}

// Public wrappers — provided. They delegate to the private recursive helpers.
void BST::insert(int key) { root_ = insert(root_, key); }
void BST::remove(int key) { root_ = remove(root_, key); }

// ============================================================================
// Task 1 — implement the three methods below.
// ============================================================================

// ----------------------------------------------------------------------------
// insert (private recursive helper)
//
// Base case  : node is nullptr → allocate a new Node(key) and return it.
// key < node->key → recurse left:  node->left  = insert(node->left,  key)
// key > node->key → recurse right: node->right = insert(node->right, key)
// key == node->key → duplicate; return node unchanged.
//
// Always return node so the parent's pointer stays connected.
// ----------------------------------------------------------------------------
BST::Node* BST::insert(Node* node, int key) {
    // TODO

    return node;
}

// ----------------------------------------------------------------------------
// remove (private recursive helper)
//
// First, navigate to the target node exactly as you would in insert.
// Once found, handle one of three cases:
//
//   Case 1 — leaf (no children):
//       delete node; return nullptr.
//
//   Case 2 — one child:
//       Save the non-null child, delete node, return the saved child.
//
//   Case 3 — two children:
//       Find the in-order successor: the minimum of the RIGHT subtree
//       (call minNode(node->right)). Copy its key into node->key.
//       Then recursively remove that successor key from node->right.
//       Do NOT delete node itself — only the successor gets deleted.
//
// If key is not found, return node unchanged.
// Always return node so the parent's pointer stays connected.
// ----------------------------------------------------------------------------
BST::Node* BST::remove(Node* node, int key) {
    // TODO

    return node;
}

// ----------------------------------------------------------------------------
// search (public — iterative)
//
// Walk the tree from root_ using the BST ordering property.
// At each step:
//   key == node->key  →  found; stop here.
//   key <  node->key  →  go left.
//   key >  node->key  →  go right.
//   node == nullptr   →  fell off the tree; not found.
//
// When the search ends, print EXACTLY this diagnostic log:
//
//   [SEARCH] key = <key>  ->  FOUND          (or NOT FOUND)
//     node  addr : <address>                 (or null if not found)
//     left  addr : <address of node->left>   (or null)
//     right addr : <address of node->right>  (or null)
//     node  size : <sizeof(Node)> bytes
//
// Cast any Node* to void* before streaming it to print the hex address.
// Example:  std::cout << static_cast<void*>(node);
//
// sizeof(Node) works without an instance and is always safe to print.
//
// Return true if found, false otherwise.
// ----------------------------------------------------------------------------
bool BST::search(int key) const {
    // TODO

    return false;
}
