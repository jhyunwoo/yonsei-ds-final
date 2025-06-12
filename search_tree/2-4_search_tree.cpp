#include <iostream>
#include <vector>
#include <algorithm>

class TwoFourTree {
private:
    static const int MAX_KEYS = 3;  // 2-4 트리는 최대 3개의 키를 가짐
    
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
        bool isFull() const { return keys.size() == MAX_KEYS; }
        bool isMinimal() const { return keys.size() == 1; }
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
            if (parent->isFull()) {
                splitNode(parent);
            }
        }
    }
    
    // 리프 노드에 키 삽입
    void insertIntoLeaf(Node* node, int key) {
        int pos = findInsertPosition(node, key);
        node->keys.insert(node->keys.begin() + pos, key);
        
        if (node->isFull()) {
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
            if (leftSibling->keys.size() > 1) {
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
            if (rightSibling->keys.size() > 1) {
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
            sibling->children.clear();  // 자식들을 이미 이동했으므로 clear
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
            child->children.clear();  // 자식들을 이미 이동했으므로 clear
            delete child;
        }
        
        // 부모가 루트이고 비어있으면
        if (node == root && node->keys.empty()) {
            root = node->children[0];
            root->parent = nullptr;
            node->children.clear();  // 자식 리스트 비우기
            delete node;
        }
        // 부모가 최소 키 개수보다 적으면
        else if (node->parent && node->keys.size() == 0) {  // isMinimal이 아니라 empty 체크
            fixUnderflow(node);
        }
    }
    
    // 언더플로우 수정
    void fixUnderflow(Node* node) {
        if (node == root) {
            if (node->keys.empty() && !node->children.empty()) {
                root = node->children[0];
                root->parent = nullptr;
                node->children.clear();  // 자식 리스트 비우기
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
                if (node != root && node->isMinimal()) {
                    fixUnderflow(node);
                }
            }
            // 내부 노드에서 삭제
            else {
                // 왼쪽 서브트리의 최대값으로 대체
                if (node->children[pos]->keys.size() > 1) {
                    int predecessor = findMax(node->children[pos]);
                    node->keys[pos] = predecessor;
                    deleteHelper(node->children[pos], predecessor);
                }
                // 오른쪽 서브트리의 최소값으로 대체
                else if (node->children[pos + 1]->keys.size() > 1) {
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
                    
                    // 오른쪽 자식의 children 비우기 (이미 이동했으므로)
                    rightChild->children.clear();
                    delete rightChild;
                    
                    // 병합된 노드에서 재귀적으로 삭제
                    deleteHelper(leftChild, key);
                    
                    // 부모가 루트이고 비어있으면
                    if (node == root && node->keys.empty()) {
                        root = node->children[0];
                        root->parent = nullptr;
                        node->children.clear();  // 자식 리스트 비우기
                        delete node;
                    }
                    // 부모가 최소 키 개수보다 적으면
                    else if (node->parent && node->isMinimal()) {
                        fixUnderflow(node);
                    }
                }
            }
        }
        // 키가 없으면 자식에서 계속 탐색
        else if (!node->isLeaf) {
            // 자식이 최소 개수면 미리 처리
            if (node->children[pos]->isMinimal()) {
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
    TwoFourTree() : root(nullptr) {}
    
    ~TwoFourTree() {
        delete root;
    }
    
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
        std::cout << "\n=== 2-4 트리 통계 ===" << std::endl;
        std::cout << "노드당 최대 키 개수: " << MAX_KEYS << std::endl;
        std::cout << "노드당 최소 키 개수: 1 (루트 제외)" << std::endl;
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
    TwoFourTree tree;
    
    std::cout << "=== 2-4 트리 테스트 ===" << std::endl;
    
    // 삽입 테스트
    std::cout << "\n1. 삽입 테스트:" << std::endl;
    int values[] = {10, 20, 30, 40, 50, 60, 70, 25, 35};
    
    for (int val : values) {
        std::cout << "\n" << val << " 삽입 후:" << std::endl;
        tree.insert(val);
        tree.printTree();
    }
    
    // 중위 순회 출력
    std::cout << "\n2. 중위 순회 (정렬된 순서): ";
    tree.printInorder();
    
    // 검색 테스트
    std::cout << "\n3. 검색 테스트:" << std::endl;
    std::cout << "25 존재: " << (tree.search(25) ? "예" : "아니오") << std::endl;
    std::cout << "15 존재: " << (tree.search(15) ? "예" : "아니오") << std::endl;
    
    // 트리 통계
    tree.printStats();
    
    // 삭제 테스트
    std::cout << "\n\n4. 삭제 테스트:" << std::endl;
    
    std::cout << "\n30 삭제 후:" << std::endl;
    tree.remove(30);
    tree.printTree();
    tree.printInorder();
    
    std::cout << "\n20 삭제 후:" << std::endl;
    tree.remove(20);
    tree.printTree();
    tree.printInorder();
    
    std::cout << "\n60 삭제 후:" << std::endl;
    tree.remove(60);
    tree.printTree();
    tree.printInorder();
    
    // 최종 통계
    tree.printStats();
    
    return 0;
} 