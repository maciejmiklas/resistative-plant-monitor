#ifndef Timer_H_
#define Timer_H_

#include <Arduino.h>
#include <stdio.h>
#include "Domain.h"

void timer_cycle();
uint32_t timer_millis();
void timer_sample(Time *ts, uint32_t startMilis);
void timer_init();

#endif /* Timer_H_ */
