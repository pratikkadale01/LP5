#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <omp.h>

using namespace std;

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    #pragma omp parallel shared(arr)
    {
        for (int i = 0; i < n - 1; ++i) {
            #pragma omp for
            for (int j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    #pragma omp critical
                    {
                        swap(arr[j], arr[j + 1]);
                    }
                }
            }
        }
    }
}


// Sequential Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

// Utility function to generate random numbers
vector<int> generateRandomNumbers(int size) {
    vector<int> nums(size);
    for (int i = 0; i < size; ++i) {
        nums[i] = rand() % 1000;
    }
    return nums;
}

int main() {
    srand(time(0));

    int size = 10000;
    vector<int> arr = generateRandomNumbers(size);

    // Sequential Bubble Sort
    vector<int> seqArr(arr);
    auto start = chrono::steady_clock::now();
    bubbleSort(seqArr);
    auto end = chrono::steady_clock::now();
    chrono::duration<double> seqTime = end - start;
    cout << "Sequential Bubble Sort: " << seqTime.count() << " seconds" << endl;

    // Parallel Bubble Sort
    vector<int> parArr(arr);
    start = chrono::steady_clock::now();
    parallelBubbleSort(parArr);
    end = chrono::steady_clock::now();
    chrono::duration<double> parTime = end - start;
    cout << "Parallel Bubble Sort: " << parTime.count() << " seconds" << endl;

    // Sequential Merge Sort
    seqArr = arr;
    start = chrono::steady_clock::now();
    sequentialMergeSort(seqArr, 0, size - 1);
    end = chrono::steady_clock::now();
    seqTime = end - start;
    cout << "Sequential Merge Sort: " << seqTime.count() << " seconds" << endl;

    // Parallel Merge Sort
    parArr = arr;
    start = chrono::steady_clock::now();
    parallelMergeSort(parArr, 0, size - 1);
    end = chrono::steady_clock::now();
    parTime = end - start;
    cout << "Parallel Merge Sort: " << parTime.count() << " seconds" << endl;

    return 0;
}
