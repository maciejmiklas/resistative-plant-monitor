#include <Timer.h>
unsigned long time;

Timer::Timer() {
	time = millis();
}

void Timer::restart(void) {
	time = millis();
}

Formated now(){
	Formated ff;
	return ff;
}

