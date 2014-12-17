#ifndef Timer_H_
#define Timer_H_

#include <Arduino.h>
#include <stdio.h>

typedef struct {
	uint16_t dd;
	char cdd[4];

	uint16_t hh;
	char chh[3];

	uint16_t mm;
	char cmm[3];

	uint16_t ss;
	char css[3];
} Time;

void timer_sample(Time *ts);

void timer_reset();

#endif /* Timer_H_ */
