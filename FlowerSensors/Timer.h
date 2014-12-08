#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"

class Timer {

public:
	Timer();

	void restart();
	//Formated now();
};

 typedef struct  {
	char dd[];
	char tt[];
	char mm[];
} Formated;


#endif /* TIMER_H_ */
