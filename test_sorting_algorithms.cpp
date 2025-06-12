#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstring>

// ==================== 정렬 알고리즘 구현 ====================

// Insertion Sort
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for(int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Heap Sort
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;
    if (r < n && arr[r] > arr[largest])
        largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Merge Sort
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right)
        return;
    
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Quick Sort
int find_pivot_index(std::vector<int>& a, int left, int right) {
    int center = (left + right) / 2;
    
    if (a[center] < a[left])
        std::swap(a[center], a[left]);
    if (a[right] < a[left])
        std::swap(a[right], a[left]);
    if (a[right] < a[center])
        std::swap(a[right], a[center]);
    
    return center;
}

void quickSort(std::vector<int>& a, int left, int right) {
    if (right > left) {
        int pivotIndex = find_pivot_index(a, left, right);
        int pivot = a[pivotIndex];
        
        std::swap(a[pivotIndex], a[right]);
        
        int i = left - 1;
        int j = right;
        
        while (true) {
            while (a[++i] < pivot);
            while (j > left && a[--j] > pivot);
            if (i >= j) break;
            std::swap(a[i], a[j]);
        }
        
        std::swap(a[i], a[right]);
        
        quickSort(a, left, i - 1);
        quickSort(a, i + 1, right);
    }
}

// Shell Sort
void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// Counting Sort
std::vector<int> countingSort(std::vector<int>& inputArray) {
    int N = inputArray.size();
    if (N == 0) return inputArray;
    
    int M = *std::max_element(inputArray.begin(), inputArray.end());
    std::vector<int> countArray(M + 1, 0);
    
    for (int i = 0; i < N; i++)
        countArray[inputArray[i]]++;
    
    for (int i = 1; i <= M; i++)
        countArray[i] += countArray[i - 1];
    
    std::vector<int> outputArray(N);
    for (int i = N - 1; i >= 0; i--) {
        outputArray[countArray[inputArray[i]] - 1] = inputArray[i];
        countArray[inputArray[i]]--;
    }
    return outputArray;
}

// Bucket Sort (float 버전)
void insertionSortFloat(std::vector<float>& arr) {
    int n = arr.size();
    for(int i = 1; i < n; ++i) {
        float key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void bucketSort(std::vector<float>& arr) {
    int n = arr.size();
    if (n <= 0) return;
    
    std::vector<std::vector<float>> b(n);
    
    for (int i = 0; i < n; i++) {
        int bi = n * arr[i];
        if (bi >= n) bi = n - 1;
        b[bi].push_back(arr[i]);
    }
    
    for (int i = 0; i < n; i++) {
        insertionSortFloat(b[i]);
    }
    
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < b[i].size(); j++) {
            arr[index++] = b[i][j];
        }
    }
}

// Radix Sort
int getMax(std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

void countSortRadix(std::vector<int>& arr, int exp) {
    int n = arr.size();
    std::vector<int> output(n);
    int count[10] = {0};
    
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
    
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    
    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixSort(std::vector<int>& arr) {
    int m = getMax(arr);
    
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSortRadix(arr, exp);
}

// ==================== 테스트 유틸리티 함수 ====================

// 랜덤 배열 생성
std::vector<int> generateRandomArray(int size, int min = 0, int max = 1000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    
    std::vector<int> arr(size);
    for (int& val : arr) {
        val = dis(gen);
    }
    return arr;
}

// 정렬된 배열 생성
std::vector<int> generateSortedArray(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    return arr;
}

// 역순 정렬된 배열 생성
std::vector<int> generateReverseSortedArray(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = size - i - 1;
    }
    return arr;
}

// 배열이 정렬되었는지 확인
bool isSorted(const std::vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i-1]) return false;
    }
    return true;
}

// 단일 매개변수 정렬 함수용 테스트 함수 (insertion sort, heap sort, shell sort, radix sort)
void testSortAlgorithm(const std::string& name, void (*sortFunc)(std::vector<int>&), 
                       const std::vector<int>& original) {
    std::vector<int> arr = original;
    
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << std::setw(15) << name << ": ";
    if (isSorted(arr)) {
        std::cout << "✓ Sorted correctly in " << std::setw(8) << duration.count() << " μs";
    } else {
        std::cout << "✗ Sort failed!";
    }
    std::cout << std::endl;
}

// 세 개의 매개변수를 가진 정렬 함수용 테스트 함수 (merge sort, quick sort)
void testSortAlgorithm(const std::string& name, void (*sortFunc)(std::vector<int>&, int, int), 
                       const std::vector<int>& original) {
    std::vector<int> arr = original;
    
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << std::setw(15) << name << ": ";
    if (isSorted(arr)) {
        std::cout << "✓ Sorted correctly in " << std::setw(8) << duration.count() << " μs";
    } else {
        std::cout << "✗ Sort failed!";
    }
    std::cout << std::endl;
}

// counting sort용 특별 테스트 함수
void testSortAlgorithm(const std::string& name, std::vector<int> (*sortFunc)(std::vector<int>&), 
                       const std::vector<int>& original) {
    std::vector<int> arr = original;
    
    auto start = std::chrono::high_resolution_clock::now();
    arr = sortFunc(arr);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << std::setw(15) << name << ": ";
    if (isSorted(arr)) {
        std::cout << "✓ Sorted correctly in " << std::setw(8) << duration.count() << " μs";
    } else {
        std::cout << "✗ Sort failed!";
    }
    std::cout << std::endl;
}

// ==================== 메인 테스트 프로그램 ====================

int main() {
    std::cout << "=== 정렬 알고리즘 종합 테스트 ===" << std::endl;
    std::cout << std::endl;
    
    // 테스트 데이터 크기
    std::vector<int> testSizes = {100, 1000, 5000};
    
    for (int size : testSizes) {
        std::cout << "테스트 크기: " << size << " 요소" << std::endl;
        std::cout << "---------------------------------" << std::endl;
        
        // 랜덤 배열 테스트
        std::cout << "랜덤 배열 테스트:" << std::endl;
        std::vector<int> randomArr = generateRandomArray(size);
        
        testSortAlgorithm("Insertion Sort", insertionSort, randomArr);
        testSortAlgorithm("Heap Sort", heapSort, randomArr);
        testSortAlgorithm("Merge Sort", mergeSort, randomArr);
        testSortAlgorithm("Quick Sort", quickSort, randomArr);
        testSortAlgorithm("Shell Sort", shellSort, randomArr);
        testSortAlgorithm("Counting Sort", countingSort, randomArr);
        testSortAlgorithm("Radix Sort", radixSort, randomArr);
        
        std::cout << std::endl;
        
        // 정렬된 배열 테스트
        std::cout << "이미 정렬된 배열 테스트:" << std::endl;
        std::vector<int> sortedArr = generateSortedArray(size);
        
        testSortAlgorithm("Insertion Sort", insertionSort, sortedArr);
        testSortAlgorithm("Heap Sort", heapSort, sortedArr);
        testSortAlgorithm("Merge Sort", mergeSort, sortedArr);
        testSortAlgorithm("Quick Sort", quickSort, sortedArr);
        testSortAlgorithm("Shell Sort", shellSort, sortedArr);
        
        std::cout << std::endl;
        
        // 역순 정렬된 배열 테스트
        std::cout << "역순 정렬된 배열 테스트:" << std::endl;
        std::vector<int> reverseArr = generateReverseSortedArray(size);
        
        testSortAlgorithm("Insertion Sort", insertionSort, reverseArr);
        testSortAlgorithm("Heap Sort", heapSort, reverseArr);
        testSortAlgorithm("Merge Sort", mergeSort, reverseArr);
        testSortAlgorithm("Quick Sort", quickSort, reverseArr);
        testSortAlgorithm("Shell Sort", shellSort, reverseArr);
        
        std::cout << std::endl << "=================================" << std::endl << std::endl;
    }
    
    // Bucket Sort 별도 테스트 (float 배열 필요)
    std::cout << "Bucket Sort 테스트 (float 배열):" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 0.99f);
    
    std::vector<float> floatArr(1000);
    for (float& val : floatArr) {
        val = dis(gen);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    bucketSort(floatArr);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    bool sorted = true;
    for (int i = 1; i < floatArr.size(); i++) {
        if (floatArr[i] < floatArr[i-1]) {
            sorted = false;
            break;
        }
    }
    
    std::cout << "Bucket Sort: ";
    if (sorted) {
        std::cout << "✓ Sorted correctly in " << duration.count() << " μs" << std::endl;
    } else {
        std::cout << "✗ Sort failed!" << std::endl;
    }
    
    // 정렬 알고리즘 비교 요약
    std::cout << std::endl << "=== 정렬 알고리즘 특성 요약 ===" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "알고리즘        | 시간복잡도(평균) | 안정성 | 추가공간" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Insertion Sort  | O(n²)           | 안정   | O(1)" << std::endl;
    std::cout << "Heap Sort       | O(n log n)      | 불안정 | O(1)" << std::endl;
    std::cout << "Merge Sort      | O(n log n)      | 안정   | O(n)" << std::endl;
    std::cout << "Quick Sort      | O(n log n)      | 불안정 | O(log n)" << std::endl;
    std::cout << "Shell Sort      | O(n^1.5)        | 불안정 | O(1)" << std::endl;
    std::cout << "Counting Sort   | O(n + k)        | 안정   | O(k)" << std::endl;
    std::cout << "Radix Sort      | O(d(n + k))     | 안정   | O(n + k)" << std::endl;
    std::cout << "Bucket Sort     | O(n + k)        | 안정   | O(n)" << std::endl;
    
    return 0;
}
