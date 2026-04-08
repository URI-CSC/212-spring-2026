#ifndef BST_H
#define BST_H

#include <iostream>
#include <cstddef>

class BST {
private:
    struct Node {
        int   key;
        Node* left;
        Node* right;

        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root_;

    // -----------------------------------------------------------------------
    // Provided helpers — do not modify.
    // -----------------------------------------------------------------------
    void  clear(Node* node);
    void  inorder(Node* node) const;
    int   height(Node* node) const;
    Node* minNode(Node* node) const;             // leftmost node in a subtree
    int   searchDepth(Node* node, int key, int depth) const;

    // -----------------------------------------------------------------------
    // TODO helpers — implement these in bst.cpp.
    // -----------------------------------------------------------------------

    // Recursively inserts key into the subtree rooted at node.
    // Returns the (possibly new) subtree root.
    // Duplicates are silently ignored (return node unchanged).
    Node* insert(Node* node, int key);

    // Recursively removes key from the subtree rooted at node.
    // Returns the (possibly changed) subtree root.
    // If key is not found, returns node unchanged.
    Node* remove(Node* node, int key);

public:
    BST() : root_(nullptr) {}
    ~BST() { clear(root_); }

    // Inserts key. No-op if key already exists.      Provided (calls private insert)
    void insert(int key);

    // Removes key. No-op if key not found.           Provided (calls private remove)
    void remove(int key);

    // Searches for key iteratively. Prints a memory  -- TODO
    // diagnostic log for the node where the search
    // terminates. Returns true if found.
    bool search(int key) const;

    // Prints all keys in ascending (sorted) order.   Provided
    void inorder() const;

    // Returns the height of the tree.                Provided
    // Height = edges on longest root-to-leaf path.
    // Returns -1 for an empty tree, 0 for a single node.
    int  height() const;

    // Returns true if the tree has no nodes.         Provided
    bool empty() const { return root_ == nullptr; }

    // Returns the depth at which key is found        Provided
    // (root = depth 1). Returns -1 if not found.
    // Used by performance.cpp — do not modify.
    int searchDepth(int key) const;
};

#endif
