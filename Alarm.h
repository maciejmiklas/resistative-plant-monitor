#ifndef Alarm_H_
#define Alarm_H_

#include <Arduino.h>
#include "Log.h"
#include "Sos.h"

/*
 * Pin for analog input used to adjust alarm trigger threshold.
 * Alarm will be triggered based on moisture measured by hygrometer - this is value in range 0-100%.
 * Reading on threshold pin is in range 0-1023 and it will be linear mapped to percentage value.
 */
#define ALARM_THRESHOLD_PIN 3

/*
 * Minimal change for analog input of alarm threshold to accept read value. Values below
 * sensitivity value will be treated as noise.
 */
#define ALARM_ADJUST_SENSITIVITY 10 // 0-1023

void alarm_cycle(uint8_t moisturePorc);
void alarm_setup();

#endif /* Alarm_H_ */
