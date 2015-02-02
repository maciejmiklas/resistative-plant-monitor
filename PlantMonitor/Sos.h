#ifndef Sos_H_
#define Sos_H_

#include <Arduino.h>
#include "Log.h"

/* Pin out for SOS LED. */
#define SOS_PIN 10

/* LED on time for 3x long flash. */
#define SOS_ON_LONG_DURATION 1000

/* LED off time for 3x long flash. */
#define SOS_OFF_LONG_DURATION 1000

/* LED on time for 3x short flash. */
#define SOS_ON_SHORT_DURATION 200

/* LED off time for 3x short flash. */
#define SOS_OFF_SHORT_DURATION 200

/** Pause time after single SOS message (3x long, 3x short). */
#define SOS_PAUSE_DONE 2000

/* Pause time between 3x long and 3x short flash. */
#define SOS_PAUSE_MIDLE 500

void sos_init();
void sos_cycle();

void sos_on();
void sos_off();

#endif /* Sos_H_ */
