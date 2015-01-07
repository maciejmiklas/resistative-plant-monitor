#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <Arduino.h>

#define LCD_LIGHT_SENS_PIN 1
#define LCD_LIGHT_SESN_SENSITIVITY 200
#define LCD_BACKLIGHT_PIN 9
#define LCD_BACKLIGHT_INIT 200
#define LCD_CLOCK_UPDATE_MS 500

#define SOS_PIN 13

#define MOISTURE_READ_PIN 0

/*
 * After plant watering the moisture level can jump very high due to the water flowing across sensor.
 * Max time will be calculated as the lowest value during adoption time. Adoption timer starts
 * after we recognize significantly increased moisture level. Default is 5 minutes.
 */
#define MOISTURE_MAX_ADOPT_MS 300000

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

enum {
	MS_CHANGED = 0x01,
	MS_INCREASED = 0x02,
	MS_ALL = 0x7F
};

typedef struct {
	uint16_t proc;
	uint16_t maxProc;
	byte status;
} Moisture;

void domain_sort(uint16_t arr[], uint16_t size);

#endif /* DOMAIN_H_ */
