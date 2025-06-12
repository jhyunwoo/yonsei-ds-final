#include <iostream>
#include <vector>
#include <algorithm>

class MarySearchTree {
private:
    int m;  // m-ary 트리의 차수
    int maxKeys;  // 노드가 가질 수 있는 최대 키 개수 (m-1)
    int minKeys;  // 노드가 가져야 하는 최소 키 개수 (ceil(m/2) - 1)
    
    struct Node {
        std::vector<int> keys;      // 노드의 키들
        std::vector<Node*> children; // 자식 노드들
        Node* parent;               // 부모 노드
        bool isLeaf;                // 리프 노드 여부
        
        Node() : parent(nullptr), isLeaf(true) {}
        
        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }
        
        int getKeyCount() const { return keys.size(); }
        int getChildCount() const { return children.size(); }
    };
    
    Node* root;
    
    // 키 삽입 위치 찾기
    int findInsertPosition(Node* node, int key) {
        int pos = 0;
        while (pos < node->getKeyCount() && key > node->keys[pos]) {
            pos++;
        }
        return pos;
    }
    
    // 노드가 가득 찼는지 확인
    bool isFull(Node* node) const {
        return node->keys.size() >= maxKeys;
    }
    
    // 노드가 최소 키 개수보다 적은지 확인
    bool hasUnderflow(Node* node) const {
        return node != root && node->keys.size() < minKeys;
    }
    
    // 노드 분할
    void splitNode(Node* node) {
        int midIndex = node->getKeyCount() / 2;
        int midKey = node->keys[midIndex];
        
        // 새로운 오른쪽 노드 생성
        Node* rightNode = new Node();
        rightNode->isLeaf = node->isLeaf;
        
        // 중간 키 이후의 키들을 오른쪽 노드로 이동
        for (int i = midIndex + 1; i < node->getKeyCount(); i++) {
            rightNode->keys.push_back(node->keys[i]);
        }
        
        // 자식 노드들도 분할 (리프가 아닌 경우)
        if (!node->isLeaf) {
            for (int i = midIndex + 1; i <= node->getChildCount() - 1; i++) {
                rightNode->children.push_back(node->children[i]);
                node->children[i]->parent = rightNode;
            }
            node->children.erase(node->children.begin() + midIndex + 1, node->children.end());
        }
        
        // 원래 노드의 키 조정
        node->keys.erase(node->keys.begin() + midIndex, node->keys.end());
        
        // 부모 노드가 없으면 (루트 분할)
        if (!node->parent) {
            Node* newRoot = new Node();
            newRoot->isLeaf = false;
            newRoot->keys.push_back(midKey);
            newRoot->children.push_back(node);
            newRoot->children.push_back(rightNode);
            node->parent = newRoot;
            rightNode->parent = newRoot;
            root = newRoot;
        } else {
            // 부모 노드에 중간 키 삽입
            Node* parent = node->parent;
            int insertPos = findInsertPosition(parent, midKey);
            
            parent->keys.insert(parent->keys.begin() + insertPos, midKey);
            parent->children.insert(parent->children.begin() + insertPos + 1, rightNode);
            rightNode->parent = parent;
            
            // 부모 노드가 가득 찼으면 재귀적으로 분할
            if (isFull(parent)) {
                splitNode(parent);
            }
        }
    }
    
    // 리프 노드에 키 삽입
    void insertIntoLeaf(Node* node, int key) {
        int pos = findInsertPosition(node, key);
        node->keys.insert(node->keys.begin() + pos, key);
        
        if (isFull(node)) {
            splitNode(node);
        }
    }
    
    // 삽입을 위한 리프 노드 찾기
    Node* findLeafNode(Node* node, int key) {
        if (node->isLeaf) {
            return node;
        }
        
        int pos = findInsertPosition(node, key);
        return findLeafNode(node->children[pos], key);
    }
    
    // 검색 헬퍼 함수
    bool searchHelper(Node* node, int key) {
        if (!node) return false;
        
        int pos = 0;
        while (pos < node->getKeyCount() && key > node->keys[pos]) {
            pos++;
        }
        
        if (pos < node->getKeyCount() && key == node->keys[pos]) {
            return true;
        }
        
        if (node->isLeaf) {
            return false;
        }
        
        return searchHelper(node->children[pos], key);
    }
    
    // 중위 순회 출력
    void inorderHelper(Node* node) {
        if (!node) return;
        
        int i;
        for (i = 0; i < node->getKeyCount(); i++) {
            if (!node->isLeaf && i < node->getChildCount()) {
                inorderHelper(node->children[i]);
            }
            std::cout << node->keys[i] << " ";
        }
        
        if (!node->isLeaf && i < node->getChildCount()) {
            inorderHelper(node->children[i]);
        }
    }
    
    // 최소값 찾기
    int findMin(Node* node) {
        if (node->isLeaf) {
            return node->keys[0];
        }
        return findMin(node->children[0]);
    }
    
    // 최대값 찾기
    int findMax(Node* node) {
        if (node->isLeaf) {
            return node->keys.back();
        }
        return findMax(node->children.back());
    }
    
    // 형제 노드로부터 키 빌리기
    bool borrowFromSibling(Node* node, int childIndex) {
        Node* child = node->children[childIndex];
        
        // 왼쪽 형제에서 빌리기
        if (childIndex > 0) {
            Node* leftSibling = node->children[childIndex - 1];
            if (leftSibling->keys.size() > minKeys) {
                // 부모의 키를 자식으로 내리고
                child->keys.insert(child->keys.begin(), node->keys[childIndex - 1]);
                
                // 왼쪽 형제의 마지막 키를 부모로 올리기
                node->keys[childIndex - 1] = leftSibling->keys.back();
                leftSibling->keys.pop_back();
                
                // 자식 노드도 이동 (리프가 아닌 경우)
                if (!child->isLeaf) {
                    child->children.insert(child->children.begin(), leftSibling->children.back());
                    leftSibling->children.back()->parent = child;
                    leftSibling->children.pop_back();
                }
                return true;
            }
        }
        
        // 오른쪽 형제에서 빌리기
        if (childIndex < node->getChildCount() - 1) {
            Node* rightSibling = node->children[childIndex + 1];
            if (rightSibling->keys.size() > minKeys) {
                // 부모의 키를 자식으로 내리고
                child->keys.push_back(node->keys[childIndex]);
                
                // 오른쪽 형제의 첫 번째 키를 부모로 올리기
                node->keys[childIndex] = rightSibling->keys[0];
                rightSibling->keys.erase(rightSibling->keys.begin());
                
                // 자식 노드도 이동 (리프가 아닌 경우)
                if (!child->isLeaf) {
                    child->children.push_back(rightSibling->children[0]);
                    rightSibling->children[0]->parent = child;
                    rightSibling->children.erase(rightSibling->children.begin());
                }
                return true;
            }
        }
        
        return false;
    }
    
    // 노드 병합
    void mergeNodes(Node* node, int childIndex) {
        Node* child = node->children[childIndex];
        Node* sibling;
        int keyIndex;
        
        // 오른쪽 형제와 병합
        if (childIndex < node->getChildCount() - 1) {
            sibling = node->children[childIndex + 1];
            keyIndex = childIndex;
            
            // 부모 키를 자식으로 내리기
            child->keys.push_back(node->keys[keyIndex]);
            
            // 형제의 모든 키를 자식으로 이동
            for (int key : sibling->keys) {
                child->keys.push_back(key);
            }
            
            // 형제의 자식들도 이동 (리프가 아닌 경우)
            if (!child->isLeaf) {
                for (Node* grandChild : sibling->children) {
                    child->children.push_back(grandChild);
                    grandChild->parent = child;
                }
            }
            
            // 부모에서 키와 포인터 제거
            node->keys.erase(node->keys.begin() + keyIndex);
            node->children.erase(node->children.begin() + childIndex + 1);
            sibling->children.clear();
            delete sibling;
        }
        // 왼쪽 형제와 병합
        else {
            sibling = node->children[childIndex - 1];
            keyIndex = childIndex - 1;
            
            // 부모 키를 형제로 내리기
            sibling->keys.push_back(node->keys[keyIndex]);
            
            // 자식의 모든 키를 형제로 이동
            for (int key : child->keys) {
                sibling->keys.push_back(key);
            }
            
            // 자식의 자식들도 이동 (리프가 아닌 경우)
            if (!sibling->isLeaf) {
                for (Node* grandChild : child->children) {
                    sibling->children.push_back(grandChild);
                    grandChild->parent = sibling;
                }
            }
            
            // 부모에서 키와 포인터 제거
            node->keys.erase(node->keys.begin() + keyIndex);
            node->children.erase(node->children.begin() + childIndex);
            child->children.clear();
            delete child;
        }
        
        // 부모가 루트이고 비어있으면
        if (node == root && node->keys.empty()) {
            root = node->children[0];
            root->parent = nullptr;
            node->children.clear();
            delete node;
        }
        // 부모가 언더플로우 상태면
        else if (node->parent && hasUnderflow(node)) {
            fixUnderflow(node);
        }
    }
    
    // 언더플로우 수정
    void fixUnderflow(Node* node) {
        if (node == root) {
            if (node->keys.empty() && !node->children.empty()) {
                root = node->children[0];
                root->parent = nullptr;
                node->children.clear();
                delete node;
            }
            return;
        }
        
        Node* parent = node->parent;
        int childIndex = 0;
        for (int i = 0; i < parent->getChildCount(); i++) {
            if (parent->children[i] == node) {
                childIndex = i;
                break;
            }
        }
        
        // 형제에서 빌릴 수 있으면 빌리기
        if (!borrowFromSibling(parent, childIndex)) {
            // 빌릴 수 없으면 병합
            mergeNodes(parent, childIndex);
        }
    }
    
    // 노드에서 키 삭제
    void deleteFromNode(Node* node, int key) {
        int pos = 0;
        while (pos < node->getKeyCount() && key > node->keys[pos]) {
            pos++;
        }
        
        if (pos < node->getKeyCount() && key == node->keys[pos]) {
            // 리프 노드에서 삭제
            if (node->isLeaf) {
                node->keys.erase(node->keys.begin() + pos);
                
                // 언더플로우 처리
                if (hasUnderflow(node)) {
                    fixUnderflow(node);
                }
            }
            // 내부 노드에서 삭제
            else {
                // 왼쪽 서브트리의 최대값으로 대체
                if (node->children[pos]->keys.size() > minKeys) {
                    int predecessor = findMax(node->children[pos]);
                    node->keys[pos] = predecessor;
                    deleteHelper(node->children[pos], predecessor);
                }
                // 오른쪽 서브트리의 최소값으로 대체
                else if (node->children[pos + 1]->keys.size() > minKeys) {
                    int successor = findMin(node->children[pos + 1]);
                    node->keys[pos] = successor;
                    deleteHelper(node->children[pos + 1], successor);
                }
                // 양쪽 자식이 모두 최소 개수면 병합 후 삭제
                else {
                    // 병합 전에 자식 노드를 저장
                    Node* leftChild = node->children[pos];
                    Node* rightChild = node->children[pos + 1];
                    
                    // 키를 왼쪽 자식으로 내리고 오른쪽 자식의 키들도 이동
                    leftChild->keys.push_back(node->keys[pos]);
                    for (int k : rightChild->keys) {
                        leftChild->keys.push_back(k);
                    }
                    
                    // 오른쪽 자식의 자식들도 이동
                    if (!leftChild->isLeaf) {
                        for (Node* grandChild : rightChild->children) {
                            leftChild->children.push_back(grandChild);
                            grandChild->parent = leftChild;
                        }
                    }
                    
                    // 부모에서 키와 오른쪽 자식 제거
                    node->keys.erase(node->keys.begin() + pos);
                    node->children.erase(node->children.begin() + pos + 1);
                    
                    // 오른쪽 자식의 children 비우기
                    rightChild->children.clear();
                    delete rightChild;
                    
                    // 병합된 노드에서 재귀적으로 삭제
                    deleteHelper(leftChild, key);
                    
                    // 부모가 루트이고 비어있으면
                    if (node == root && node->keys.empty()) {
                        root = node->children[0];
                        root->parent = nullptr;
                        node->children.clear();
                        delete node;
                    }
                    // 부모가 언더플로우 상태면
                    else if (node->parent && hasUnderflow(node)) {
                        fixUnderflow(node);
                    }
                }
            }
        }
        // 키가 없으면 자식에서 계속 탐색
        else if (!node->isLeaf) {
            // 자식이 최소 개수면 미리 처리
            if (hasUnderflow(node->children[pos])) {
                if (!borrowFromSibling(node, pos)) {
                    if (pos == node->getChildCount() - 1) {
                        mergeNodes(node, pos - 1);
                        pos--;
                    } else {
                        mergeNodes(node, pos);
                    }
                }
            }
            deleteHelper(node->children[pos], key);
        }
    }
    
    // 삭제 헬퍼 함수
    void deleteHelper(Node* node, int key) {
        if (!node) return;
        deleteFromNode(node, key);
    }
    
public:
    MarySearchTree(int order = 5) : m(order), root(nullptr) {
        if (m < 3) {
            throw std::invalid_argument("m-ary 트리의 차수는 최소 3이어야 합니다");
        }
        maxKeys = m - 1;
        minKeys = (m + 1) / 2 - 1;  // ceil(m/2) - 1
    }
    
    ~MarySearchTree() {
        delete root;
    }
    
    // 차수 반환
    int getOrder() const { return m; }
    
    // 삽입
    void insert(int key) {
        if (!root) {
            root = new Node();
            root->keys.push_back(key);
            return;
        }
        
        // 이미 존재하는 키는 삽입하지 않음
        if (search(key)) {
            return;
        }
        
        Node* leaf = findLeafNode(root, key);
        insertIntoLeaf(leaf, key);
    }
    
    // 검색
    bool search(int key) {
        return searchHelper(root, key);
    }
    
    // 삭제
    void remove(int key) {
        if (!root || !search(key)) {
            return;
        }
        
        deleteHelper(root, key);
        
        // 루트가 비었으면 제거
        if (root && root->keys.empty() && root->isLeaf) {
            delete root;
            root = nullptr;
        }
    }
    
    // 중위 순회 출력
    void printInorder() {
        if (root) {
            inorderHelper(root);
            std::cout << std::endl;
        } else {
            std::cout << "트리가 비어있습니다." << std::endl;
        }
    }
    
    // 트리 구조 출력 (디버깅용)
    void printTree() {
        if (!root) {
            std::cout << "트리가 비어있습니다." << std::endl;
            return;
        }
        printTreeHelper(root, 0);
    }
    
    // 트리 통계 출력
    void printStats() {
        std::cout << "\n=== " << m << "-ary 트리 통계 ===" << std::endl;
        std::cout << "차수(m): " << m << std::endl;
        std::cout << "노드당 최대 키 개수: " << maxKeys << std::endl;
        std::cout << "노드당 최소 키 개수: " << minKeys << " (루트 제외)" << std::endl;
        if (root) {
            int height = getHeight(root);
            int nodeCount = countNodes(root);
            int keyCount = countKeys(root);
            std::cout << "트리 높이: " << height << std::endl;
            std::cout << "총 노드 수: " << nodeCount << std::endl;
            std::cout << "총 키 개수: " << keyCount << std::endl;
        }
    }
    
private:
    void printTreeHelper(Node* node, int level) {
        if (!node) return;
        
        // 들여쓰기
        for (int i = 0; i < level; i++) {
            std::cout << "  ";
        }
        
        // 노드의 키들 출력
        std::cout << "[";
        for (int i = 0; i < node->getKeyCount(); i++) {
            std::cout << node->keys[i];
            if (i < node->getKeyCount() - 1) std::cout << ", ";
        }
        std::cout << "]" << (node->isLeaf ? " (leaf)" : "") << std::endl;
        
        // 자식 노드들 재귀적으로 출력
        for (int i = 0; i < node->getChildCount(); i++) {
            printTreeHelper(node->children[i], level + 1);
        }
    }
    
    int getHeight(Node* node) {
        if (!node || node->isLeaf) return 1;
        return 1 + getHeight(node->children[0]);
    }
    
    int countNodes(Node* node) {
        if (!node) return 0;
        int count = 1;
        for (Node* child : node->children) {
            count += countNodes(child);
        }
        return count;
    }
    
    int countKeys(Node* node) {
        if (!node) return 0;
        int count = node->keys.size();
        for (Node* child : node->children) {
            count += countKeys(child);
        }
        return count;
    }
};

// 테스트 코드
int main() {
    std::cout << "=== m-ary 검색 트리 테스트 ===" << std::endl;
    
    // 3-ary 트리 테스트
    std::cout << "\n1. 3-ary 트리 테스트:" << std::endl;
    MarySearchTree tree3(3);
    
    int values3[] = {10, 20, 30, 40, 50, 60, 70, 25, 35};
    for (int val : values3) {
        tree3.insert(val);
    }
    
    std::cout << "중위 순회: ";
    tree3.printInorder();
    tree3.printTree();
    tree3.printStats();
    
    // 5-ary 트리 테스트
    std::cout << "\n\n2. 5-ary 트리 테스트:" << std::endl;
    MarySearchTree tree5(5);
    
    int values5[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 25, 35, 45, 55};
    for (int val : values5) {
        tree5.insert(val);
    }
    
    std::cout << "중위 순회: ";
    tree5.printInorder();
    tree5.printTree();
    tree5.printStats();
    
    // 7-ary 트리 테스트
    std::cout << "\n\n3. 7-ary 트리 테스트:" << std::endl;
    MarySearchTree tree7(7);
    
    for (int i = 10; i <= 150; i += 10) {
        tree7.insert(i);
    }
    
    std::cout << "중위 순회: ";
    tree7.printInorder();
    tree7.printTree();
    tree7.printStats();
    
    // 검색 테스트
    std::cout << "\n\n4. 검색 테스트 (5-ary 트리):" << std::endl;
    std::cout << "45 존재: " << (tree5.search(45) ? "예" : "아니오") << std::endl;
    std::cout << "75 존재: " << (tree5.search(75) ? "예" : "아니오") << std::endl;
    
    // 삭제 테스트
    std::cout << "\n5. 삭제 테스트 (5-ary 트리):" << std::endl;
    std::cout << "\n30 삭제 후:" << std::endl;
    tree5.remove(30);
    tree5.printTree();
    tree5.printInorder();
    
    std::cout << "\n50 삭제 후:" << std::endl;
    tree5.remove(50);
    tree5.printTree();
    tree5.printInorder();
    
    // 대량 데이터 테스트
    std::cout << "\n\n6. 대량 데이터 테스트 (10-ary 트리):" << std::endl;
    MarySearchTree tree10(10);
    
    // 100개의 랜덤 키 삽입
    std::vector<int> randomKeys = {
        15, 42, 73, 28, 91, 64, 37, 12, 55, 88,
        23, 67, 34, 79, 46, 19, 82, 51, 94, 31,
        58, 25, 70, 43, 16, 85, 39, 76, 13, 61,
        97, 22, 49, 10, 68, 40, 83, 29, 56, 92,
        35, 74, 47, 20, 63, 86, 38, 71, 14, 59,
        26, 69, 44, 17, 80, 52, 95, 32, 65, 41,
        77, 24, 60, 33, 78, 45, 18, 81, 53, 96,
        30, 57, 27, 72, 48, 21, 66, 87, 36, 75,
        11, 62, 98, 89, 54, 84, 50, 93, 90, 99
    };
    
    for (int key : randomKeys) {
        tree10.insert(key);
    }
    
    tree10.printStats();
    
    // 범위 확인
    std::cout << "\n일부 키 검색:" << std::endl;
    int checkKeys[] = {10, 50, 75, 100, 105};
    for (int key : checkKeys) {
        std::cout << key << " 존재: " << (tree10.search(key) ? "예" : "아니오") << std::endl;
    }
    
    return 0;
}
