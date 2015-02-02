#ifndef Alarm_H_
#define Alarm_H_

#include <Arduino.h>
#include "Log.h"
#include "Sos.h"

#define ALATM_THRESHOLD_PIN 3
#define ALATM_ADJUST_SENSITIVITY 10 // 0-1023

void alarm_cycle(uint8_t moisturePorc);
void alarm_init();

#endif /* Sos_H_ */
