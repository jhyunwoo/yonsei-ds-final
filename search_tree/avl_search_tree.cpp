#include <iostream>

class AVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
        Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;
    int height(Node* n) {
        return n ? n->height : 0;
    }
    int getBalance(Node* n) {
        return n ? height(n->left) - height(n->right) : 0;
    }
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y;
    }
    Node* insert(Node* node, int key) {
        if (!node)
            return new Node(key);
        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node; // 중복 키 허용 안 함
        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }
    Node* deleteNode(Node* node, int key) {
        if (!node)
            return node;
        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = deleteNode(node->right, temp->key);
            }
        }
        if (!node)
            return node;
        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    bool search(Node* node, int key) const {
        if (!node) return false;
        if (key == node->key) return true;
        else if (key < node->key) return search(node->left, key);
        else return search(node->right, key);
    }
    void preOrder(Node* node) const {
        if (node) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { 
        destroyTree(root);
    }
    void insert(int key) {
        root = insert(root, key);
    }
    void remove(int key) {
        root = deleteNode(root, key);
    }
    bool contains(int key) const {
        return search(root, key);
    }
    void printPreOrder() const {
        preOrder(root);
        std::cout << std::endl;
    }
};

int main() {
    AVLTree avl;
    
    // 삽입 테스트
    std::cout << "AVL 트리 삽입 테스트:\n";
    int values[] = {10, 20, 30, 40, 50, 25};
    
    for (int val : values) {
        avl.insert(val);
        std::cout << "삽입 " << val << " 후: ";
        avl.printPreOrder();
    }
    
    // 검색 테스트
    std::cout << "\n검색 테스트:\n";
    std::cout << "25 존재: " << (avl.contains(25) ? "예" : "아니오") << "\n";
    std::cout << "15 존재: " << (avl.contains(15) ? "예" : "아니오") << "\n";
    
    // 삭제 테스트
    std::cout << "\n삭제 테스트:\n";
    avl.remove(20);
    std::cout << "20 삭제 후: ";
    avl.printPreOrder();
    
    return 0;
}
