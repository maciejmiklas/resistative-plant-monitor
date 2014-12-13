#ifndef FlowerSensors_H_
#define FlowerSensors_H_

#include "Arduino.h"
#include <LiquidCrystal.h>
#include <Timer.h>

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

// 1023 / 5 = 204.6
#define VOLT_DIV 204.6f

#define HG_IN 0

void clcd(uint8_t row);
short readProc();
void printProc(short proc, short maxProc);
void setupLcdStatic();
void updateClock();
short calcProc();
void sort(short a[], short size);

#endif /* FlowerSensors_H_ */
