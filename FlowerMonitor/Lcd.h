#ifndef Lcd_H_
#define Lcd_H_

#include <LiquidCrystal.h>
#include <Arduino.h>
#include "Timer.h"

#define LIGHT_SENS_PIN 1
#define LIGHT_SESN_INIT 200;

void lcd_init();
void lcd_printClock(Time *time);
void lcd_printProc(uint16_t proc, uint16_t maxProc);
void lcd_printMoisture(Moisture *moisture);
void lcd_bright();

#endif /* Lcd_H_ */
