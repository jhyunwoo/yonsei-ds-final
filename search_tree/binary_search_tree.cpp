//
// Created by 전현우 on 25. 6. 12.
//
struct BinaryNode {
    int data;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode(int val, BinaryNode* l = nullptr, BinaryNode* r = nullptr)
    : data(val), left(l), right(r) {}
};

bool contains(BinaryNode* T, int x) {
    if (T == nullptr)
        return false;
    if (x < T->data)
        return contains(T->left, x);
    else if (T->data < x)
        return contains(T->right, x);
    else
        return true; // Match found

    }

bool contains(BinaryNode* T, int x) {
    if (T == nullptr)
        return false;
    if (x < T->data)
        return contains(T->left, x);
    else if (T->data < x)
        return contains(T->right, x);
    else
        return true; // Match found
    }
}


BinaryNode* find_min(BinaryNode* T) {
    if (T == nullptr)
        return nullptr;
    else if (T->left == nullptr)
        return T;
    else
        return find_min(T->left);
}


BinaryNode* insert(BinaryNode* T, int x) {
    if (T == nullptr) {
        return new BinaryNode(x);
        6
        }
    if (x < T->data) {
        T->left = insert(T->left, x);
    } else if (T->data < x) {
        T->right = insert(T->right, x);
    }
    // If x == T->data, do nothing
    // (assumes no duplicates)
    return t;

    }

BinaryNode* remove(BinaryNode* T, int x) {
    if (T == nullptr)
        return nullptr; // Item not found; do nothing
    if (x < T->data) {
        T->left = remove(T->left, x);
    } else if (T->data < x) {
        T->right = remove(t->right, x);
    } else { if (T->left != nullptr && T->right != nullptr) {
        // Two children: replace with smallest in right subtree
        BinaryNode* min_node = find_min(T->right);
        T->data = min_node->data;
        T->right = remove(T->right, T->data);
    } else {
        // One or zero children
        BinaryNode* old_node = T;
        T = (T->left != nullptr) ? T->left : T->right;
        delete old_node;
        // Found node to remove
    }
    }
    return t;

    }