#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <Arduino.h>

// LCD
#define LCD_LIGHT_SENS_PIN 1
#define LCD_LIGHT_SESN_SENSITIVITY 10
#define LCD_BACKLIGHT_PIN 9
#define LCD_BACKLIGHT_INIT 200
#define LCD_BACKLIGHT_MIN 140
#define LCD_BACKLIGHT_MAX 255
#define LCD_CLOCK_UPDATE_MS 500

// SOS
#define SOS_PIN 13
#define SOS_OFF_DURATION 300
#define SOS_ON_SHORT_DURATION 400
#define SOS_ON_LONG_DURATION 800

// HYGRO
#define MOISTURE_READ_PIN 0

/*
 * After plant watering the moisture level can jump very high due to the water flowing across sensor.
 * Max time will be calculated as the lowest value during adoption time. Adoption timer starts
 * after we recognize significantly increased moisture level. Default is 5 minutes.
 */
#define MOISTURE_MAX_ADOPT_MS 300000

typedef struct {
	uint16_t dd;
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
	uint8_t ml;
} Time;

enum {
	MS_CHANGED = 0x01, MS_INCREASED = 0x02, MS_ALL = 0x7F
};

typedef struct {
	uint8_t proc;
	uint8_t maxProc;
	byte status;
} Moisture;

void domain_sort(uint16_t arr[], uint16_t size);

#endif /* DOMAIN_H_ */
