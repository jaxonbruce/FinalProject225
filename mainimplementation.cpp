#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
using namespace std;
using namespace chrono;

class AVLNode {
public:
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int k) {
        key = k;
        left = right = nullptr;
        height = 1;
    }
};

class AVLTree {
public:
    AVLNode* root;
    int rotationCount;

    AVLTree() {
        root = nullptr;
        rotationCount = 0;
    }

    int height(AVLNode* n) {
        return n ? n->height : 0;
    }

    int getBalance(AVLNode* n) {
        return n ? height(n->left) - height(n->right) : 0;
    }

    AVLNode* rightRotate(AVLNode* y) {
        rotationCount++;
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        rotationCount++;
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // LL
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // RR
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // LR
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RL
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }
};

enum Color { RED, BLACK };

class RBNode {
public:
    int key;
    Color color;
    RBNode *left, *right, *parent;

    RBNode(int k) {
        key = k;
        color = RED;
        left = right = parent = nullptr;
    }
};

class RBTree {
public:
    RBNode* root;
    int rotationCount;

    RBTree() {
        root = nullptr;
        rotationCount = 0;
    }

    void leftRotate(RBNode*& root, RBNode*& x) {
        rotationCount++;
        RBNode* y = x->right;
        x->right = y->left;

        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode*& root, RBNode*& y) {
        rotationCount++;
        RBNode* x = y->left;
        y->left = x->right;

        if (x->right)
            x->right->parent = y;

        x->parent = y->parent;

        if (!y->parent)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void fixInsert(RBNode*& root, RBNode*& pt) {
        while (pt != root && pt->parent->color == RED) {
            RBNode* parent = pt->parent;
            RBNode* grandparent = parent->parent;

            if (parent == grandparent->left) {
                RBNode* uncle = grandparent->right;

                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    pt = grandparent;
                } else {
                    if (pt == parent->right) {
                        leftRotate(root, parent);
                        pt = parent;
                        parent = pt->parent;
                    }
                    rightRotate(root, grandparent);
                    swap(parent->color, grandparent->color);
                    pt = parent;
                }
            } else {
                RBNode* uncle = grandparent->left;

                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    pt = grandparent;
                } else {
                    if (pt == parent->left) {
                        rightRotate(root, parent);
                        pt = parent;
                        parent = pt->parent;
                    }
                    leftRotate(root, grandparent);
                    swap(parent->color, grandparent->color);
                    pt = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void insert(int key) {
        RBNode* pt = new RBNode(key);
        RBNode* parent = nullptr;
        RBNode* curr = root;

        while (curr) {
            parent = curr;
            if (pt->key < curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }

        pt->parent = parent;

        if (!parent)
            root = pt;
        else if (pt->key < parent->key)
            parent->left = pt;
        else
            parent->right = pt;

        fixInsert(root, pt);
    }
};

void testSorted(int N) {
    AVLTree avl;
    RBTree rb;

    cout << "\n--- Sorted Input ---\n";

    auto start = high_resolution_clock::now();
    for (int i = 1; i <= N; i++)
        avl.root = avl.insert(avl.root, i);
    auto end = high_resolution_clock::now();

    cout << "AVL Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "AVL Rotations: " << avl.rotationCount << endl;

    start = high_resolution_clock::now();
    for (int i = 1; i <= N; i++)
        rb.insert(i);
    end = high_resolution_clock::now();

    cout << "RB Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "RB Rotations: " << rb.rotationCount << endl;
}

void testReverse(int N) {
    AVLTree avl;
    RBTree rb;

    cout << "\n--- Reverse Sorted Input ---\n";

    auto start = high_resolution_clock::now();
    for (int i = N; i >= 1; i--)
        avl.root = avl.insert(avl.root, i);
    auto end = high_resolution_clock::now();

    cout << "AVL Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "AVL Rotations: " << avl.rotationCount << endl;

    start = high_resolution_clock::now();
    for (int i = N; i >= 1; i--)
        rb.insert(i);
    end = high_resolution_clock::now();

    cout << "RB Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "RB Rotations: " << rb.rotationCount << endl;
}

void testRandom(int N) {
    AVLTree avl;
    RBTree rb;

    cout << "\n--- Random Input ---\n";

    vector<int> data;
    for (int i = 1; i <= N; i++)
        data.push_back(i);

    shuffle(data.begin(), data.end(), default_random_engine(42));

    auto start = high_resolution_clock::now();
    for (int x : data)
        avl.root = avl.insert(avl.root, x);
    auto end = high_resolution_clock::now();

    cout << "AVL Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "AVL Rotations: " << avl.rotationCount << endl;

    start = high_resolution_clock::now();
    for (int x : data)
        rb.insert(x);
    end = high_resolution_clock::now();

    cout << "RB Time: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";
    cout << "RB Rotations: " << rb.rotationCount << endl;
}

int main() {
    int N = 1000;

    testSorted(N);
    testReverse(N);
    testRandom(N);

    return 0;
}
