#ifndef MoistureMeter_H_
#define MoistureMeter_H_

#include <Arduino.h>
#include "Log.h"
#include "Util.h"

/*
 * General flow:
 * 1) Initialize - setup PINs and go to 2)
 * 2) Warm up - switch on power for moisture sensor (#MOISTURE_POWER_PIN) and wait for #MOISTURE_WARM_UP_MS
 * 3) Start measure - take #PROC_PROBES probes from PIN #MOISTURE_READ_PIN, pause for
 *    #MESURE_PROBE_WAIT_MS after taking single probe.
 * 4) Calculate median form taken probes and return value, so that the software can react to changing moisture.
 * 5) Go to sleep for #MESURE_FREQ_MS
 * 6) Wake up and go to 2)
 *
 * NOTE: all wait or delay functions are non blocking. We relay on fact that #mmet_cycle(Moisture) will be called
 * every few milliseconds and we calculate time required to move from one step to another.
 * The flow is based on simplified state machine pattern.
 */

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
 * Adoption factor used to transfer analog read to percentage:
 * IN: (0 - 1023) -> ("sensor dry" - "sensor in water")
 * OUT: (0 - 100%)  -> ("sensor dry" - "sensor in water")
 *
 * Calculated percentage can be outside 0-100 range - it will be corrected later on, but not above 256
 */
#define probeToPercent(read) (read/8)

/* Start probing after this time */
#define MESURE_FREQ_MS 300000

/* write time between probes */
#define MESURE_PROBE_WAIT_MS 1000
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

Moisture* mmet_setup();

void mmet_cycle(Moisture *moisture);

#endif /* MoistureMeter_H_ */
