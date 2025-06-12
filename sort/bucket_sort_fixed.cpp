#include <iostream>
#include <vector>
#include <algorithm>

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

// Function to sort arr[] of size n using bucket sort
void bucketSort(float arr[], int n) {
    // 1) Create n empty buckets
    std::vector<float> b[n];
    
    // 2) Put array elements in different buckets
    for (int i = 0; i < n; i++) {
        int bi = n * arr[i]; // Index in bucket
        b[bi].push_back(arr[i]);
    }
    
    // 3) Sort individual buckets using insertion sort
    for (int i = 0; i < n; i++) {
        insertionSortFloat(b[i]);
    }
    
    // 4) Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < b[i].size(); j++) {
            arr[index++] = b[i][j];
        }
    }
}
