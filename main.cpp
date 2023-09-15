// main.cpp
#include <iostream>
#include "optimized_thread.h"
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

mutex coutLocker;

void taskFunc2(int, int delay) {
    this_thread::sleep_for(chrono::microseconds(delay));
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
    cout << "Quicksort completed for range [" << low << ", " << high << "]" << endl;
}

int main(int argc, char** argv) {
    vector<int> arr(100);

    // Initialize the array with random values (as in your original code).
    srand(static_cast<unsigned>(time(NULL)));
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = rand() % 10; // Adjust the range as needed.
    }

    RequestHandler2 rh;

    // Push the quicksort task for the entire array into the thread pool.
    rh.push_task(static_cast<quicksort_task_type>([&arr]() {quicksort(arr, 0, arr.size() - 1); }), arr, 0, arr.size() - 1);

    // Wait for the sorting to complete.
    rh.waitForCompletion();

    // Print the sorted array.
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
