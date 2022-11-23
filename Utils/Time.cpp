#include "Time.h"
#include <chrono>
using namespace std;

TimeNow Time::now;

__int64 TimeNow::UnixNano() {
	chrono::steady_clock::time_point timestamp = chrono::steady_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(timestamp.time_since_epoch()).count();
}