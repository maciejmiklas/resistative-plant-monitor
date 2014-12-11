#include <Timer.h>
unsigned long time;

Timer::Timer() {
	time = millis();
}

void Timer::restart(void) {
	time = millis();
}

Timer::Time Timer::sample(void) {
	//long milis =  126000000 + 1440000 + 17000;// 1d 11h 24m 17s
	//long milis =  345600000 + 1440000 + 17000;// 4d 0h 24m 17s
	//long milis =  446400000 + 1440000 + 17000;// 5d 4h 24m 17s
	long milis = millis() - time;
	long sec = milis / 1000;
	Timer::Time ts;

	// days
	unsigned short dd = sec / 86400;
	ts.dd = dd;
	sprintf(ts.cdd, "%03d", dd);
	unsigned int cs = sec - dd * 86400;

	// hours
	unsigned short hh = cs / 3600;
	cs -= hh * 3600;
	ts.hh = hh;
	sprintf(ts.chh, "%02d", hh);

	// minutes
	unsigned short mm = cs / 60;
	cs -= mm * 60;
	ts.mm = mm;
	sprintf(ts.cmm, "%02d", mm);

	// seconds
	unsigned short ss = cs;
	ts.ss = ss;
	sprintf(ts.css, "%02d", ss);

	return ts;
}

