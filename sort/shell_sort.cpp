#include <iostream>
#include <vector>

int shellSort(std::vector<int>& arr) {
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
    return 0;
}

int main(){
    std::vector<int> arr = {12, 11, 13, 5, 6};
    shellSort(arr);
}