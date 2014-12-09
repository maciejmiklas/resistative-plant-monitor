#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"

class Timer {

public:
	struct Time {
		unsigned int hh;
		unsigned short mm;
		unsigned short ss;
	};

	Timer();
	void restart();
	Time sample();
};

#endif /* TIMER_H_ */
