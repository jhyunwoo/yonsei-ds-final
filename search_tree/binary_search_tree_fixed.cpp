#include <iostream>

struct BinaryNode {
    int data;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode(int val, BinaryNode* l = nullptr, BinaryNode* r = nullptr)
        : data(val), left(l), right(r) {}
};

class BinarySearchTree {
private:
    BinaryNode* root;
    
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
        }
        if (x < T->data) {
            T->left = insert(T->left, x);
        } else if (T->data < x) {
            T->right = insert(T->right, x);
        }
        // If x == T->data, do nothing (assumes no duplicates)
        return T;
    }
    
    BinaryNode* remove(BinaryNode* T, int x) {
        if (T == nullptr)
            return nullptr; // Item not found; do nothing
            
        if (x < T->data) {
            T->left = remove(T->left, x);
        } else if (T->data < x) {
            T->right = remove(T->right, x);
        } else {
            // Found node to remove
            if (T->left != nullptr && T->right != nullptr) {
                // Two children: replace with smallest in right subtree
                BinaryNode* min_node = find_min(T->right);
                T->data = min_node->data;
                T->right = remove(T->right, T->data);
            } else {
                // One or zero children
                BinaryNode* old_node = T;
                T = (T->left != nullptr) ? T->left : T->right;
                delete old_node;
            }
        }
        return T;
    }
    
    void inorderHelper(BinaryNode* node) {
        if (node != nullptr) {
            inorderHelper(node->left);
            std::cout << node->data << " ";
            inorderHelper(node->right);
        }
    }
    
    void destroyTree(BinaryNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
public:
    BinarySearchTree() : root(nullptr) {}
    
    ~BinarySearchTree() {
        destroyTree(root);
    }
    
    void insert(int x) {
        root = insert(root, x);
    }
    
    void remove(int x) {
        root = remove(root, x);
    }
    
    bool contains(int x) {
        return contains(root, x);
    }
    
    void printInorder() {
        inorderHelper(root);
        std::cout << std::endl;
    }
};

int main() {
    BinarySearchTree bst;
    
    std::cout << "Binary Search Tree Test:" << std::endl;
    
    // 삽입 테스트
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int val : values) {
        bst.insert(val);
    }
    
    std::cout << "After insertion: ";
    bst.printInorder();
    
    // 검색 테스트
    std::cout << "Contains 40: " << (bst.contains(40) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 25: " << (bst.contains(25) ? "Yes" : "No") << std::endl;
    
    // 삭제 테스트
    bst.remove(30);
    std::cout << "After removing 30: ";
    bst.printInorder();
    
    return 0;
}
