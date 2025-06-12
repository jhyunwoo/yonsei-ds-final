vector<int> countSort(vector<int>& inputArray) {
    int N = inputArray.size();
    // Finding the maximum element of array inputArray[].
    int M = 0;
    for (int i = 0; i < N; i++)
        M = max(M, inputArray[i]);
    // Initializing countArray[] with 0
    vector<int> countArray(M + 1, 0);
    // Mapping each element of inputArray[] as an index of countArray[] array
    for (int i = 0; i < N; i++)
        countArray[inputArray[i]]++;
    // Calculating prefix sum at every index of array countArray[]
    for (int i = 1; i <= M; i++)
        countArray[i] += countArray[i - 1];
    // Creating outputArray[] from countArray[] array
    vector<int> outputArray(N);
    for (int i = N - 1; i >= 0; i--) {
        outputArray[countArray[inputArray[i]] - 1] = inputArray[i];
        countArray[inputArray[i]]--;
    }
    return outputArray;
}

// The main function to that sorts arr[] of size n using Radix Sort
void radixsort(int arr[], int n) {
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);
    // Do counting sort for every digit.
    // Note that instead of passing digit
    // number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}