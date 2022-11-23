#pragma once

class TimeNow {
public:
	static __int64 UnixNano();
};

class Time {
public:
	static TimeNow now;
};