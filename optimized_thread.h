// optimized_thread.h
#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>

using namespace std;

typedef std::function<void()> task_type;
typedef std::function<void()> quicksort_task_type;


template<class T>
class BockedQueue {
public:
    void push(T& item) {
        lock_guard<mutex> l(m_locker);
        m_task_queue.push(item);
        m_event_holder.notify_one();
    }

    void pop(T& item) {
        unique_lock<mutex> l(m_locker);
        if (m_task_queue.empty())
            m_event_holder.wait(l, [this] { return !m_task_queue.empty(); });
        item = m_task_queue.front();
        m_task_queue.pop();
    }

    bool fast_pop(T& item) {
        unique_lock<mutex> l(m_locker);
        if (m_task_queue.empty())
            return false;
        item = m_task_queue.front();
        m_task_queue.pop();
        return true;
    }

private:
    queue<T> m_task_queue;
    mutex m_locker;
    condition_variable m_event_holder;
};

class OptimizedThreadPool {
public:
    OptimizedThreadPool();
    void start();
    void stop();
    void push_task(task_type f);
    void push_task(quicksort_task_type f, vector<int>& arr, int low, int high);
    void waitForCompletion();
    void threadFunc(int qindex);

private:
    int m_thread_count;

    vector<thread> m_threads;
    vector<BockedQueue<task_type>> m_thread_queues;

    unsigned m_qindex;
};

class RequestHandler2 {
public:
    RequestHandler2();
    ~RequestHandler2();
    void push_task(task_type f);
    void push_task(quicksort_task_type f, vector<int>& arr, int low, int high);
    void waitForCompletion();

private:
    OptimizedThreadPool m_tpool;
};
