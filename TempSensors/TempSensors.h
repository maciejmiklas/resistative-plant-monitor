// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef TempSensors_H_
#define TempSensors_H_
#include "Arduino.h"
//add your includes for the project TempSensors here

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
void lm35();
void resetLm35();
void clcd(uint8_t row);
float readV(short port);
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project TempSensors here

//Do not add code below this line
#endif /* TempSensors_H_ */