#ifndef Hygrometer_H_
#define Hygrometer_H_

#include <Arduino.h>
#include "Log.h"
#include "Util.h"

/* Analog input pin used to read moisture */
#define MOISTURE_READ_PIN 0

/*
 * Digital out PIN used to power on moisture sensor.
 * LOW enables sensor, HIGH disables (NPN transistor).
 */
#define MOISTURE_POWER_PIN 8

/*
 * Time in milliseconds to warm up moisture sensor before taking a measurement.
 * Moisture sensor will be powered on short before taking a measurement, and right after that
 * it goes off.
 */
#define MOISTURE_WARM_UP_MS 5000

/*
 * After plant watering the moisture level can jump very high due to the water flowing across sensor.
 * Max time will be calculated as the lowest value during adoption time. Adoption timer starts
 * after we recognize significantly increased moisture level. Default is 10 minutes.
 */
#define MOISTURE_MAX_ADOPT_MS 600000

/*
 * Adoption factor used to transfer analog read to percentage.
 * sensor read:
 * 0 - 780 - "sensor dry" - "sensor in water"
 * 0 - 100%  - "sensor dry" - "sensor in water"
 */
#define MOISTURE_PROC_ADOPT 8

#define MESURE_FREQ_MS 10000
#define PROC_PROBES 10

/* minimal change in % to recognize moisture change */
#define MIN_TO_CHANGE 5

enum {
	// small change
	MS_CHANGED = 0x01,

	// large change
	MS_INCREASED = 0x02,
	MS_ALL = 0x7F
};

typedef struct {
	uint8_t proc;
	uint8_t maxProc;
	byte status;
} Moisture;

Moisture* hygro_setup();

void hygro_cycle(Moisture *moisture);

#endif /* Hygrometer_H_ */
