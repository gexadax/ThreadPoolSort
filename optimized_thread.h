#pragma once
#include <iostream>
#include <queue>
#include <condition_variable>
#include <functional>
#include <thread>

using namespace std;

typedef function<void()> task_type; typedef void (*FuncType) (std::vector<int>&, int, int);
void singleThreadedSort(std::vector<int>& arr, int a, int b);
void quicksort(std::vector<int>& arr, int low, int high);

template<class T>
class BlockedQueue {
public:
	void push(T& item) {
		lock_guard<mutex> l(m_locker);
		m_task_queue.push(item);
		m_notifier.notify_one();
	}
	void pop(T& item) {
		unique_lock<mutex> l(m_locker);
		if (m_task_queue.empty())
			m_notifier.wait(l, [this] {return !m_task_queue.empty(); });
		item = m_task_queue.front();
		m_task_queue.pop();
	}
	bool fast_pop(T& item) {
		lock_guard<mutex> l(m_locker);
		if (m_task_queue.empty())
			return false;
		item = m_task_queue.front();
		m_task_queue.pop();
		return true;
	}
private:
	queue<T> m_task_queue;
	mutex m_locker;
	condition_variable m_notifier;
};

class OptimizedThreadPool {
public:
	OptimizedThreadPool();
	void start();
	void stop();
	void push_task(FuncType func, std::vector<int>& vec, int a, int b);
	void threadFunc(int qindex);
private:
	int m_thread_count;
	vector<thread> m_threads;
	vector<BlockedQueue<task_type>> m_thread_queues;
	unsigned m_qindex;
};

class RequestHandler2 {
public:
	RequestHandler2();
	~RequestHandler2();
void push_task(FuncType func, std::vector<int>& vec, int a, int b); private:
	OptimizedThreadPool m_tpool;
};
