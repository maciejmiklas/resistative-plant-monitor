#ifndef FlowerSensors_H_
#define FlowerSensors_H_

#include "Arduino.h"
#include <LiquidCrystal.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project FlowerSensors here

void clcd(uint8_t row);
uint16_t readProc();
void printProc(uint16_t proc, uint16_t maxProc);
void setupLcdStatic();
void updateClock();
uint16_t calcProc();
void sort(uint16_t a[], uint16_t size);

struct Time {
	uint16_t dd;
	char cdd[4];

	uint16_t hh;
	char chh[3];

	uint16_t mm;
	char cmm[3];

	uint16_t ss;
	char css[3];
};
void sample(Time *ts);

#endif /* FlowerSensors_H_ */
