#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"

class Timer {

public:
	struct Time {
		int dd;
		char cdd[4];

		short hh;
		char chh[3];

		short mm;
		char cmm[3];

		short ss;
		char css[3];
	};

	Timer();
	void restart();
	Time sample();
};

#endif /* TIMER_H_ */
