#ifndef Hygrometer_H_
#define Hygrometer_H_

#include <Arduino.h>
#include "Log.h"
#include "Util.h"

/* Analog input pin used to read moisture */
#define MOISTURE_READ_PIN 0

/*
 * After plant watering the moisture level can jump very high due to the water flowing across sensor.
 * Max time will be calculated as the lowest value during adoption time. Adoption timer starts
 * after we recognize significantly increased moisture level. Default is 10 minutes.
 */
#define MOISTURE_MAX_ADOPT_MS 600000

/*
 * Adoption factor used to transfer analog read to percentage.
 */
#define MOISTURE_PROC_ADOPT 8

#define MESURE_FREQ_MS 10000
#define PROC_PROBES 10
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

void hygro_setup(Moisture *moisture);

void hygro_sample(Moisture *moisture);

#endif /* Hygrometer_H_ */
