#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Sequential implementation of min, max, sum, and average operations
void sequentialOperations(const vector<int>& data) {
    int min_val = *min_element(data.begin(), data.end());
    int max_val = *max_element(data.begin(), data.end());
    int sum = accumulate(data.begin(), data.end(), 0);
    double avg = static_cast<double>(sum) / data.size();

    cout << "Sequential Operations:\n";
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;
}

// Parallel implementation of min, max, sum, and average operations
void parallelOperations(const vector<int>& data) {
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    int sum = 0;

#pragma omp parallel for reduction(min : min_val) reduction(max : max_val) reduction(+ : sum)
    for (int i = 0; i < data.size(); i++) {
        min_val = min(min_val, data[i]);
        max_val = max(max_val, data[i]);
        sum += data[i];
    }

    double avg = static_cast<double>(sum) / data.size();

    cout << "Parallel Operations:\n";
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;
}

int main() {
    const int dataSize = 10000000;
    vector<int> data(dataSize);
    iota(data.begin(), data.end(), 1);

    // Sequential operations
    auto startTime = high_resolution_clock::now();
    sequentialOperations(data);
    auto endTime = high_resolution_clock::now();
    auto sequentialDuration = duration_cast<milliseconds>(endTime - startTime);

    cout << "Sequential Execution Time: " << sequentialDuration.count() << " ms\n\n";

    // Parallel operations
    omp_set_num_threads(4);
    startTime = high_resolution_clock::now();
    parallelOperations(data);
    endTime = high_resolution_clock::now();
    auto parallelDuration = duration_cast<milliseconds>(endTime - startTime);

    cout << "\nParallel Execution Time: " << parallelDuration.count() << " ms\n";

    return 0;
}
