#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::system_clock;
using std::chrono::milliseconds;

class Timer {
public:
	Timer() {}
	~Timer() {}

	void start() {
		start_time = high_resolution_clock::now();
	}

	void stop() {
		end_time = high_resolution_clock::now();
	}

	unsigned long long getElapsedMilliSeconds() {
		return std::chrono::duration_cast<milliseconds>(end_time - start_time).count();
	}
private:
	Timer(const Timer&);
	Timer& operator=(const Timer&);

	system_clock::time_point start_time;
	system_clock::time_point end_time;
};

#endif
