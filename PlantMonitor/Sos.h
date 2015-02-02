#ifndef Sos_H_
#define Sos_H_

#include <Arduino.h>
#include "Log.h"

#define SOS_PIN 10
#define SOS_OFF_SHORT_DURATION 200
#define SOS_OFF_LONG_DURATION 1000
#define SOS_ON_SHORT_DURATION 200
#define SOS_ON_LONG_DURATION 1000
#define SOS_PAUSE_DONE 2000
#define SOS_PAUSE_MIDLE 500

void sos_init();
void sos_cycle();

void sos_on();
void sos_off();

#endif /* Sos_H_ */
