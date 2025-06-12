int find_pivot_index(std::vector<int>& a, int left, int right) {
    int center = (left + right) / 2;
    int tmp;
    // median-of-three logic
    if (a[center] < a[left]) {
        tmp = a[center]; a[center] = a[left]; a[left] = tmp;
    }
    if (a[right] < a[left]) {
        tmp = a[right]; a[right] = a[left]; a[left] = tmp;
    }
    if (a[right] < a[center]) {
        tmp = a[right]; a[right] = a[center]; a[center] = tmp;
    }
    return center; // return index of pivot (center)
    }

void quicksort(vector<int>& a, int left, int right) {
    if (right > left) {
        // Choose pivot using median-of-three
        int pivotIndex = find_pivot_index(a, left, right);
        int pivot = a[pivotIndex];
        // Move pivot to the end
        swap(a[pivotIndex], a[right]);
        int i = left - 1;
        int j = right;
        while (true) {
            while (a[++i] < pivot); // move i forward
            while (j > left && a[--j] > pivot); // move j backward
            if (i >= j) break;
            swap(a[i], a[j]);
        }
        // Restore pivot to correct position
        swap(a[i], a[right]);
        // Recur on both partitions
        quicksort(a, left, i - 1);
        quicksort(a, i + 1, right);
    }
}