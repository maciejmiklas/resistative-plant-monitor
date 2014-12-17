#include "Timer.h"

//#define TR__MS_SEC 1000
#define TR__MS_SEC 10
#define TR__SEC_DD 86400
#define TR__SEC_HH 3600
#define TR__SEC_MM 60

uint32_t timeMS;

//long milis =  126000000 + 1440000 + 17000;// 1d 11h 24m 17s
//long milis =  345600000 + 1440000 + 17000;// 4d 0h 24m 17s
//long milis =  446400000 + 1440000 + 17000;// 5d 4h 24m 17s
void timer_sample(Time *ts) {
	uint32_t milis = millis() - timeMS;
	uint32_t sec = milis / TR__MS_SEC;

	// days
	uint16_t dd = sec / TR__SEC_DD;
	ts->dd = dd;
	sprintf(ts->cdd, "%03d", dd);
	uint16_t cs = sec - dd * TR__SEC_DD;

	Serial.print(">>> cs1:");
	Serial.print(cs);

	// hours
	uint16_t hh = cs / TR__SEC_HH;
	cs -= hh * TR__SEC_HH;
	ts->hh = hh;
	sprintf(ts->chh, "%02d", hh);

	Serial.print(",cs2:");
	Serial.print(cs);

	// minutes
	uint16_t mm = cs / TR__SEC_MM;
	cs -= mm * TR__SEC_MM;
	ts->mm = mm;
	sprintf(ts->cmm, "%02d", mm);

	Serial.print(",cs3:");
	Serial.print(cs);

	// seconds
	uint16_t ss = cs;
	ts->ss = ss;
	sprintf(ts->css, "%02d", ss);

	Serial.print(",milis:");
	Serial.print(milis);

	Serial.print(",sec:");
	Serial.print(sec);

	Serial.print(",dd:");
	Serial.print(dd);

	Serial.print(",hh:");
	Serial.print(hh);

	Serial.print(",mm:");
	Serial.print(mm);

	Serial.print(",ss:");
	Serial.print(ss);
	Serial.println(" <<<");
}

void timer_reset() {
	timeMS = millis();
}
