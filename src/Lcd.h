#ifndef Lcd_H_
#define Lcd_H_

#include <LiquidCrystal.h>
#include <Arduino.h>
#include "Timer.h"
#include "Log.h"
#include "Hygrometer.h"

/* Input pin for photoresistor. */
#define LCD_LIGHT_SENS_PIN 1

/* Input pin for variable resistor used to adjust insensitivity of LCD back light. */
#define LCD_LIGHT_ADJUST_PIN 2

/*
 * Minimal change on adjust input to recognize it as valid read. Reads below this value will be
 * treated as noise.
 */
#define LCD_LIGHT_ADJUST_SENSITIVITY 10

/* Minimum analog input change of photorezistor to adopt LCD back light. */
#define LCD_LIGHT_SESN_SENSITIVITY 40

/* PWM out for LCD back light. */
#define LCD_BACKLIGHT_PIN 9

/* Initial value for LCD back light PWM */
#define LCD_BACKLIGHT_INIT 200

/* Minimal brightness of PWM for LCD back light. */
#define LCD_BACKLIGHT_MIN 140

/* Maximal brightens of PWM for LCD back light. */
#define LCD_BACKLIGHT_MAX 255

/* Interval for update of LCD display */
#define LCD_CLOCK_UPDATE_MS 500

void lcd_init();
void lcd_cycle();

void lcd_printTime(Time *time);
void lcd_printProc(uint16_t proc, uint16_t maxProc);
void lcd_printMoisture(Moisture *moisture);

#endif /* Lcd_H_ */
