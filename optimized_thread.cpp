#include "optimized_thread.h"

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

void singleThreadedSort(std::vector<int>& arr, int a, int b) {
    auto start = std::chrono::high_resolution_clock::now();
    quicksort(arr, a, b);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
}

OptimizedThreadPool::OptimizedThreadPool() :
    m_thread_count(std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 4),
    m_thread_queues(m_thread_count) {}

void OptimizedThreadPool::start() {
    for (int i = 0; i < m_thread_count; i++) { m_threads.emplace_back(&OptimizedThreadPool::threadFunc, this, i); }
}

void OptimizedThreadPool::stop() {
    for (int i = 0; i < m_thread_count; i++) {
        task_type empty_task;
        m_thread_queues[i].push(empty_task);
    }
    for (auto& t : m_threads) {
        t.join();
    }
}

void OptimizedThreadPool::push_task(FuncType func, std::vector<int>& vec, int a, int b) {
    int queue_to_push = m_qindex++ % m_thread_count;
    task_type new_task([&vec, a, b, func] {func(vec, a, b); });
    m_thread_queues[queue_to_push].push(new_task);
}

void OptimizedThreadPool::threadFunc(int qindex) {
    while (true) {
        task_type task_to_do;        
        bool res;        
        int i = 0;
        for (i; i < m_thread_count; i++) {
            if (res = m_thread_queues[(qindex + i) % m_thread_count].fast_pop(task_to_do))
                break;
        }

        if (!res) {
            m_thread_queues[qindex].pop(task_to_do);
        }
        else if (!task_to_do) {
            m_thread_queues[(qindex + i) % m_thread_count].push(task_to_do);
        }

        if (!task_to_do) {
            return;
        }
        task_to_do();
    }
}

RequestHandler2::RequestHandler2() {
    m_tpool.start();
}

RequestHandler2::~RequestHandler2() {
    m_tpool.stop();
}

void RequestHandler2::push_task(FuncType func, std::vector<int>& vec, int a, int b) {
    m_tpool.push_task(func, vec, a, b);
}
