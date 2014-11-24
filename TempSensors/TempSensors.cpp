// Do not remove the include below
#include "TempSensors.h"
#include <LiquidCrystal.h>

#define TMP36_IN 0

// 1023 / 5 = 204.6
#define VOLT_DIV 204.6

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.noAutoscroll();
}

void loop() {
	float tmp36Volt = readV(TMP36_IN);
	float tmp36Temp = (tmp36Volt - 0.5) * 100.0;
	clcd(0);
	lcd.print(tmp36Temp);
	lcd.print("(c) ");
	lcd.print(tmp36Volt);
	lcd.print("(V)");

	delay(500);
}

float readV(short port) {
	return analogRead(port) / VOLT_DIV;
}

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}
