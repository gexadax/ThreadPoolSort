#include <iostream>
#include "optimized_thread.h"
#include <chrono>

using namespace std;

mutex coutLocker;

void taskfunc(int id, int delay) {
	this_thread::sleep_for(chrono::seconds(delay));
	lock_guard<mutex> l(coutLocker);
	cout << "task " << id << " made by thread_id " << this_thread::get_id() << endl;
}

void taskFunc2(int, int delay) {
	this_thread::sleep_for(chrono::microseconds(delay));
}

int main(int argc, char** argv) {

	RequestHandler2 rh;

	for (int i = 1; i <= 100000; i++) {
		srand((int)time(NULL));
		rh.push_task(taskFunc2, i, 1 + rand() % 3);
	}

	return 0;
}