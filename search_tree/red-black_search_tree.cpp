#include <iostream>
#include <queue>
#include <iomanip>

enum Color { RED, BLACK };

class RedBlackTree {
private:
    struct Node {
        int key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;
        
        Node(int k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    
    Node* root;
    Node* NIL;  // 센티널 노드
    
    // 좌회전
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        
        if (y->left != NIL) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
    }
    
    // 우회전
    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        
        if (x->right != NIL) {
            x->right->parent = y;
        }
        
        x->parent = y->parent;
        
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
        
        x->right = y;
        y->parent = x;
    }
    
    // 삽입 후 RB 트리 속성 복원
    void insertFixup(Node* z) {
        while (z->parent != nullptr && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                
                if (y->color == RED) {
                    // Case 1: 삼촌이 빨간색
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // Case 2: 삼촌이 검은색이고 z가 오른쪽 자식
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Case 3: 삼촌이 검은색이고 z가 왼쪽 자식
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                // 대칭적인 경우
                Node* y = z->parent->parent->left;
                
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }
    
    // 노드 삽입
    void insertNode(int key) {
        Node* z = new Node(key);
        z->left = NIL;
        z->right = NIL;
        
        Node* y = nullptr;
        Node* x = root;
        
        while (x != NIL) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        z->parent = y;
        
        if (y == nullptr) {
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }
        
        insertFixup(z);
    }
    
    // 서브트리의 최소값 찾기
    Node* minimum(Node* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }
    
    // 노드 교체
    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    
    // 삭제 후 RB 트리 속성 복원
    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                
                if (w->color == RED) {
                    // Case 1: 형제가 빨간색
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    // Case 2: 형제가 검은색이고 형제의 두 자식도 검은색
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        // Case 3: 형제가 검은색이고 형제의 왼쪽 자식은 빨간색, 오른쪽은 검은색
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    // Case 4: 형제가 검은색이고 형제의 오른쪽 자식이 빨간색
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                // 대칭적인 경우
                Node* w = x->parent->left;
                
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }
    
    // 노드 삭제
    void deleteNode(Node* z) {
        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;
        
        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        delete z;
        
        if (yOriginalColor == BLACK) {
            deleteFixup(x);
        }
    }
    
    // 노드 검색
    Node* searchNode(Node* node, int key) {
        if (node == NIL || key == node->key) {
            return node;
        }
        
        if (key < node->key) {
            return searchNode(node->left, key);
        } else {
            return searchNode(node->right, key);
        }
    }
    
    // 중위 순회
    void inorderHelper(Node* node) {
        if (node != NIL) {
            inorderHelper(node->left);
            std::cout << node->key << " ";
            inorderHelper(node->right);
        }
    }
    
    // 트리 구조 출력
    void printTreeHelper(Node* node, std::string indent, bool last) {
        if (node != NIL) {
            std::cout << indent;
            if (last) {
                std::cout << "R----";
                indent += "     ";
            } else {
                std::cout << "L----";
                indent += "|    ";
            }
            
            std::string sColor = node->color == RED ? "RED" : "BLACK";
            std::cout << node->key << "(" << sColor << ")" << std::endl;
            
            printTreeHelper(node->left, indent, false);
            printTreeHelper(node->right, indent, true);
        }
    }
    
    // 트리 삭제 (메모리 해제)
    void deleteTree(Node* node) {
        if (node != NIL) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
    
    // Red-Black 트리 속성 검증
    bool isValidRedBlackTree(Node* node, int& blackHeight) {
        if (node == NIL) {
            blackHeight = 1;
            return true;
        }
        
        int leftBlackHeight, rightBlackHeight;
        
        // 속성 4: 빨간 노드의 자식은 모두 검은색
        if (node->color == RED) {
            if ((node->left != NIL && node->left->color == RED) ||
                (node->right != NIL && node->right->color == RED)) {
                std::cout << "속성 위반: 빨간 노드 " << node->key << "의 자식이 빨간색입니다." << std::endl;
                return false;
            }
        }
        
        // 재귀적으로 좌우 서브트리 검증
        if (!isValidRedBlackTree(node->left, leftBlackHeight) ||
            !isValidRedBlackTree(node->right, rightBlackHeight)) {
            return false;
        }
        
        // 속성 5: 모든 경로의 검은 노드 수가 같은지 확인
        if (leftBlackHeight != rightBlackHeight) {
            std::cout << "속성 위반: 노드 " << node->key << "의 좌우 검은 높이가 다릅니다." << std::endl;
            return false;
        }
        
        blackHeight = leftBlackHeight + (node->color == BLACK ? 1 : 0);
        return true;
    }
    
public:
    RedBlackTree() {
        NIL = new Node(0);
        NIL->color = BLACK;
        NIL->left = nullptr;
        NIL->right = nullptr;
        root = NIL;
    }
    
    ~RedBlackTree() {
        deleteTree(root);
        delete NIL;
    }
    
    // 삽입
    void insert(int key) {
        insertNode(key);
    }
    
    // 삭제
    void remove(int key) {
        Node* node = searchNode(root, key);
        if (node != NIL) {
            deleteNode(node);
        }
    }
    
    // 검색
    bool search(int key) {
        return searchNode(root, key) != NIL;
    }
    
    // 중위 순회 출력
    void printInorder() {
        inorderHelper(root);
        std::cout << std::endl;
    }
    
    // 트리 구조 출력
    void printTree() {
        if (root != NIL) {
            printTreeHelper(root, "", true);
        } else {
            std::cout << "트리가 비어있습니다." << std::endl;
        }
    }
    
    // Red-Black 트리 속성 검증
    bool validate() {
        // 속성 2: 루트는 검은색
        if (root != NIL && root->color != BLACK) {
            std::cout << "속성 위반: 루트가 검은색이 아닙니다." << std::endl;
            return false;
        }
        
        int blackHeight;
        return isValidRedBlackTree(root, blackHeight);
    }
    
    // 트리 통계
    void printStats() {
        if (root == NIL) {
            std::cout << "트리가 비어있습니다." << std::endl;
            return;
        }
        
        int nodeCount = 0;
        int redCount = 0;
        int blackCount = 0;
        int height = 0;
        
        // BFS를 사용하여 통계 수집
        std::queue<std::pair<Node*, int>> q;
        q.push({root, 1});
        
        while (!q.empty()) {
            Node* current = q.front().first;
            int level = q.front().second;
            q.pop();
            
            if (current != NIL) {
                nodeCount++;
                if (current->color == RED) redCount++;
                else blackCount++;
                
                height = std::max(height, level);
                
                q.push({current->left, level + 1});
                q.push({current->right, level + 1});
            }
        }
        
        std::cout << "\n=== Red-Black 트리 통계 ===" << std::endl;
        std::cout << "총 노드 수: " << nodeCount << std::endl;
        std::cout << "빨간 노드 수: " << redCount << std::endl;
        std::cout << "검은 노드 수: " << blackCount << std::endl;
        std::cout << "트리 높이: " << height << std::endl;
    }
};

// 테스트 코드
int main() {
    RedBlackTree rbt;
    
    std::cout << "=== Red-Black 트리 테스트 ===" << std::endl;
    
    // 1. 삽입 테스트
    std::cout << "\n1. 삽입 테스트:" << std::endl;
    int values[] = {7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13};
    
    for (int val : values) {
        std::cout << "\n" << val << " 삽입 후:" << std::endl;
        rbt.insert(val);
        rbt.printTree();
        
        // 각 삽입 후 검증
        if (!rbt.validate()) {
            std::cout << "Red-Black 트리 속성 위반!" << std::endl;
        } else {
            std::cout << "Red-Black 트리 속성 만족" << std::endl;
        }
    }
    
    // 2. 중위 순회
    std::cout << "\n2. 중위 순회 (정렬된 순서): ";
    rbt.printInorder();
    
    // 3. 검색 테스트
    std::cout << "\n3. 검색 테스트:" << std::endl;
    std::cout << "11 존재: " << (rbt.search(11) ? "예" : "아니오") << std::endl;
    std::cout << "15 존재: " << (rbt.search(15) ? "예" : "아니오") << std::endl;
    
    // 4. 트리 통계
    rbt.printStats();
    
    // 5. 삭제 테스트
    std::cout << "\n\n5. 삭제 테스트:" << std::endl;
    int deleteValues[] = {18, 11, 3, 10, 22};
    
    for (int val : deleteValues) {
        std::cout << "\n" << val << " 삭제 후:" << std::endl;
        rbt.remove(val);
        rbt.printTree();
        
        // 각 삭제 후 검증
        if (!rbt.validate()) {
            std::cout << "Red-Black 트리 속성 위반!" << std::endl;
        } else {
            std::cout << "Red-Black 트리 속성 만족" << std::endl;
        }
    }
    
    // 6. 최종 상태
    std::cout << "\n6. 최종 중위 순회: ";
    rbt.printInorder();
    rbt.printStats();
    
    // 7. 대량 데이터 테스트
    std::cout << "\n\n7. 대량 데이터 테스트:" << std::endl;
    RedBlackTree rbt2;
    
    // 순차적 삽입 (최악의 경우)
    std::cout << "순차적으로 1-20 삽입:" << std::endl;
    for (int i = 1; i <= 20; i++) {
        rbt2.insert(i);
    }
    
    rbt2.printStats();
    if (rbt2.validate()) {
        std::cout << "순차 삽입 후에도 Red-Black 트리 속성 만족" << std::endl;
    }
    
    return 0;
} 