#ifndef Lcd_H_
#define Lcd_H_

#include <LiquidCrystal.h>
#include <Arduino.h>
#include "Timer.h"
#include "Domain.h"
#include "Util.h"
#include "Log.h"

void lcd_init();
void lcd_printClock(Time *time);
void lcd_printProc(uint16_t proc, uint16_t maxProc);
void lcd_printMoisture(Moisture *moisture);
void lcd_bright();

#endif /* Lcd_H_ */
