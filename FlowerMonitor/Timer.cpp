#include "Timer.h"

#define TR__MS_SEC 1000
#define TR__SEC_DD 86400
#define TR__SEC_HH 3600
#define TR__SEC_MM 60

uint32_t timeMS;

//long milis =  126000000 + 1440000 + 17000;// 1d 11h 24m 17s
//long milis =  345600000 + 1440000 + 17000;// 4d 0h 24m 17s
//long milis =  446400000 + 1440000 + 17000;// 5d 4h 24m 17s
void timer_sample(Time *ts) {
	uint32_t milis = util_millis() - timeMS;
	uint32_t sec = milis / TR__MS_SEC;

	// days
	ts->dd = sec / TR__SEC_DD;
	sprintf(ts->cdd, "%03d", ts->dd);
	uint32_t cs = sec - ts->dd * TR__SEC_DD;

	// hours
	ts->hh = cs / TR__SEC_HH;
	cs -= ts->hh * TR__SEC_HH;
	sprintf(ts->chh, "%02d", ts->hh);

	// minutes
	ts->mm = cs / TR__SEC_MM;
	cs -= ts->mm * TR__SEC_MM;
	sprintf(ts->cmm, "%02d", ts->mm);

	// seconds
	uint16_t ss = cs;
	ts->ss = ss;
	sprintf(ts->css, "%02d", ss);
}

void timer_reset() {
	timeMS = util_millis();
}
