#ifndef Timer_H_
#define Timer_H_

#include <Arduino.h>
#include <stdio.h>
#include "Domain.h"
#include "Util.h"

void timer_sample(Time *ts);

void timer_reset();

#endif /* Timer_H_ */
