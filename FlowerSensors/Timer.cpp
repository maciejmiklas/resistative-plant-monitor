#include <Timer.h>
unsigned long time;

Timer::Timer() {
	time = millis();
}

void Timer::restart(void) {
	time = millis();
}

Timer::Time Timer::sample(void) {
	long milis = 39600000 + 1440000 + 17000;// 11h 24m  17s
	long sec = milis / 1000;
	unsigned int hh = sec / 3600;
	long hs = hh * 3600;
	unsigned short mm = (sec - hs) / 60;
	unsigned short ss = sec - hs - mm * 60;
	Timer::Time ts;
	ts.hh = hh;
	ts.mm = mm;
	ts.ss = ss;
	return ts;
}

