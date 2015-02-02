#ifndef Lcd_H_
#define Lcd_H_

#include <LiquidCrystal.h>
#include <Arduino.h>
#include "Timer.h"
#include "Log.h"
#include "Hygrometer.h"

// Input pin for photoresistor.
#define LCD_LIGHT_SENS_PIN 1

// Input pin for variable resistor used to adjust insensitivity of LCD back light.
#define LCD_LIGHT_ADJUST_PIN 2

#define LCD_LIGHT_ADJUST_SENSITIVITY 10
#define LCD_LIGHT_SESN_SENSITIVITY 40
#define LCD_BACKLIGHT_PIN 9
#define LCD_BACKLIGHT_INIT 200
#define LCD_BACKLIGHT_MIN 140
#define LCD_BACKLIGHT_MAX 255
#define LCD_CLOCK_UPDATE_MS 500

void lcd_init();
void lcd_cycle();

void lcd_printTime(Time *time);
void lcd_printProc(uint16_t proc, uint16_t maxProc);
void lcd_printMoisture(Moisture *moisture);

#endif /* Lcd_H_ */
