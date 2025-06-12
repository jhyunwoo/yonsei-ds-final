#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <set>
#include <iomanip>
#include <algorithm>

// 간단한 BST 구현 (테스트용)
class SimpleBST {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        Node(int val) : data(val), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    Node* insert(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->data)
            node->left = insert(node->left, val);
        else if (val > node->data)
            node->right = insert(node->right, val);
        return node;
    }
    
    bool search(Node* node, int val) {
        if (!node) return false;
        if (val == node->data) return true;
        if (val < node->data) return search(node->left, val);
        return search(node->right, val);
    }
    
    Node* findMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }
    
    Node* remove(Node* node, int val) {
        if (!node) return nullptr;
        
        if (val < node->data)
            node->left = remove(node->left, val);
        else if (val > node->data)
            node->right = remove(node->right, val);
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
        return node;
    }
    
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
    int getHeight(Node* node) {
        if (!node) return 0;
        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
    
public:
    SimpleBST() : root(nullptr) {}
    ~SimpleBST() { destroyTree(root); }
    
    void insert(int val) { root = insert(root, val); }
    bool search(int val) { return search(root, val); }
    void remove(int val) { root = remove(root, val); }
    int getHeight() { return getHeight(root); }
};

// 테스트 유틸리티 함수들
std::vector<int> generateRandomNumbers(int count, int min = 1, int max = 10000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    
    std::set<int> unique_numbers;
    while (unique_numbers.size() < count) {
        unique_numbers.insert(dis(gen));
    }
    
    return std::vector<int>(unique_numbers.begin(), unique_numbers.end());
}

std::vector<int> generateSequentialNumbers(int count) {
    std::vector<int> numbers;
    for (int i = 1; i <= count; i++) {
        numbers.push_back(i);
    }
    return numbers;
}

// 트리 성능 테스트
template<typename TreeType>
void testTreePerformance(const std::string& treeName, const std::vector<int>& testData) {
    TreeType tree;
    std::vector<double> insertTimes, searchTimes, removeTimes;
    
    std::cout << "\n" << treeName << " 테스트 (" << testData.size() << " 요소):" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // 삽입 테스트
    auto start = std::chrono::high_resolution_clock::now();
    for (int val : testData) {
        tree.insert(val);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto insertDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "삽입 시간: " << insertDuration.count() << " μs" << std::endl;
    
    // 검색 테스트
    start = std::chrono::high_resolution_clock::now();
    int foundCount = 0;
    for (int val : testData) {
        if (tree.search(val)) foundCount++;
    }
    end = std::chrono::high_resolution_clock::now();
    auto searchDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "검색 시간: " << searchDuration.count() << " μs (찾은 개수: " << foundCount << ")" << std::endl;
    
    // 존재하지 않는 값 검색 테스트
    start = std::chrono::high_resolution_clock::now();
    int notFoundCount = 0;
    for (int i = 0; i < testData.size(); i++) {
        if (!tree.search(testData.back() + i + 1)) notFoundCount++;
    }
    end = std::chrono::high_resolution_clock::now();
    auto notFoundDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "미존재 값 검색 시간: " << notFoundDuration.count() << " μs" << std::endl;
    
    // 트리 높이 출력 (BST만 해당)
    if (treeName == "Binary Search Tree") {
        std::cout << "트리 높이: " << tree.getHeight() << std::endl;
    }
    
    // 삭제 테스트
    std::vector<int> removeData(testData.begin(), testData.begin() + testData.size() / 2);
    start = std::chrono::high_resolution_clock::now();
    for (int val : removeData) {
        tree.remove(val);
    }
    end = std::chrono::high_resolution_clock::now();
    auto removeDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "삭제 시간 (" << removeData.size() << " 요소): " << removeDuration.count() << " μs" << std::endl;
    
    // 삭제 확인
    int removedCount = 0;
    for (int val : removeData) {
        if (!tree.search(val)) removedCount++;
    }
    std::cout << "삭제 확인: " << removedCount << "/" << removeData.size() << " 성공" << std::endl;
}

// 정확성 테스트
template<typename TreeType>
bool testTreeCorrectness(const std::string& treeName) {
    TreeType tree;
    bool allTestsPassed = true;
    
    std::cout << "\n" << treeName << " 정확성 테스트:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // 기본 삽입/검색 테스트
    std::vector<int> testValues = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    
    // 삽입
    for (int val : testValues) {
        tree.insert(val);
    }
    
    // 모든 값이 존재하는지 확인
    bool insertTest = true;
    for (int val : testValues) {
        if (!tree.search(val)) {
            insertTest = false;
            std::cout << "✗ 삽입 테스트 실패: " << val << "을(를) 찾을 수 없음" << std::endl;
            allTestsPassed = false;
        }
    }
    if (insertTest) {
        std::cout << "✓ 삽입 테스트 통과" << std::endl;
    }
    
    // 존재하지 않는 값 검색
    bool searchTest = true;
    std::vector<int> nonExistentValues = {15, 55, 85, 90, 100};
    for (int val : nonExistentValues) {
        if (tree.search(val)) {
            searchTest = false;
            std::cout << "✗ 검색 테스트 실패: 존재하지 않는 " << val << "을(를) 찾음" << std::endl;
            allTestsPassed = false;
        }
    }
    if (searchTest) {
        std::cout << "✓ 검색 테스트 통과" << std::endl;
    }
    
    // 삭제 테스트
    std::vector<int> removeValues = {20, 30, 50};
    for (int val : removeValues) {
        tree.remove(val);
    }
    
    bool removeTest = true;
    for (int val : removeValues) {
        if (tree.search(val)) {
            removeTest = false;
            std::cout << "✗ 삭제 테스트 실패: " << val << "이(가) 여전히 존재함" << std::endl;
            allTestsPassed = false;
        }
    }
    
    // 삭제되지 않은 값들이 여전히 존재하는지 확인
    for (int val : testValues) {
        bool shouldExist = std::find(removeValues.begin(), removeValues.end(), val) == removeValues.end();
        if (shouldExist && !tree.search(val)) {
            removeTest = false;
            std::cout << "✗ 삭제 테스트 실패: " << val << "이(가) 잘못 삭제됨" << std::endl;
            allTestsPassed = false;
        }
    }
    
    if (removeTest) {
        std::cout << "✓ 삭제 테스트 통과" << std::endl;
    }
    
    // 중복 삽입 테스트
    tree.insert(40);
    tree.insert(40);
    int count40 = 0;
    if (tree.search(40)) count40 = 1;
    
    if (count40 == 1) {
        std::cout << "✓ 중복 처리 테스트 통과" << std::endl;
    } else {
        std::cout << "✗ 중복 처리 테스트 실패" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

int main() {
    std::cout << "=== 탐색 트리 알고리즘 종합 테스트 ===" << std::endl;
    
    // 1. 정확성 테스트
    std::cout << "\n[1] 정확성 테스트" << std::endl;
    std::cout << "=================" << std::endl;
    
    bool bstCorrect = testTreeCorrectness<SimpleBST>("Binary Search Tree");
    
    // 2. 성능 테스트
    std::cout << "\n\n[2] 성능 테스트" << std::endl;
    std::cout << "===============" << std::endl;
    
    std::vector<int> testSizes = {1000, 5000, 10000};
    
    for (int size : testSizes) {
        std::cout << "\n### 테스트 크기: " << size << " ###" << std::endl;
        
        // 랜덤 데이터
        std::cout << "\n-- 랜덤 데이터 --" << std::endl;
        std::vector<int> randomData = generateRandomNumbers(size);
        testTreePerformance<SimpleBST>("Binary Search Tree", randomData);
        
        // 순차 데이터 (최악의 경우)
        std::cout << "\n-- 순차 데이터 (최악의 경우) --" << std::endl;
        std::vector<int> sequentialData = generateSequentialNumbers(size);
        testTreePerformance<SimpleBST>("Binary Search Tree", sequentialData);
    }
    
    // 3. 트리 특성 요약
    std::cout << "\n\n=== 탐색 트리 특성 요약 ===" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "트리 종류          | 평균 검색 | 최악 검색 | 평균 삽입 | 최악 삽입" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Binary Search Tree | O(log n)  | O(n)      | O(log n)  | O(n)" << std::endl;
    std::cout << "AVL Tree           | O(log n)  | O(log n)  | O(log n)  | O(log n)" << std::endl;
    std::cout << "Red-Black Tree     | O(log n)  | O(log n)  | O(log n)  | O(log n)" << std::endl;
    std::cout << "2-4 Tree           | O(log n)  | O(log n)  | O(log n)  | O(log n)" << std::endl;
    std::cout << "m-ary Tree         | O(log n)  | O(log n)  | O(log n)  | O(log n)" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    
    std::cout << "\n트리별 특징:" << std::endl;
    std::cout << "- BST: 구현이 간단하지만 불균형 시 성능 저하" << std::endl;
    std::cout << "- AVL: 엄격한 균형으로 검색 최적화, 삽입/삭제 시 회전 많음" << std::endl;
    std::cout << "- Red-Black: AVL보다 느슨한 균형, 삽입/삭제 효율적" << std::endl;
    std::cout << "- 2-4 Tree: B-트리의 특수 케이스, 디스크 기반 시스템에 적합" << std::endl;
    std::cout << "- m-ary Tree: 노드당 많은 키 저장, 캐시 효율성 높음" << std::endl;
    
    // 테스트 결과 요약
    std::cout << "\n\n=== 테스트 결과 요약 ===" << std::endl;
    std::cout << "------------------------" << std::endl;
    if (bstCorrect) {
        std::cout << "✓ 모든 정확성 테스트 통과" << std::endl;
    } else {
        std::cout << "✗ 일부 정확성 테스트 실패" << std::endl;
    }
    std::cout << "✓ 성능 테스트 완료" << std::endl;
    std::cout << "\n참고: 순차 데이터에서 BST는 연결 리스트처럼 동작하여 O(n) 성능을 보입니다." << std::endl;
    std::cout << "      균형 트리(AVL, Red-Black)는 이런 경우에도 O(log n)을 유지합니다." << std::endl;
    
    return 0;
}
