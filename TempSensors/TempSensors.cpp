// Do not remove the include below
#include "TempSensors.h"
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define TMP36_IN 0
#define V_PR 10

// vlotage compensation when using not stabilized power supply
#define V_CMP 0.015f

// 1023 / 5 = 204.6
#define VOLT_DIV 204.6f

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 7

short lm35pr;
float lm35Vs;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
	lcd.begin(16, 2);
	lcd.noAutoscroll();
	resetLm35();

	sensors.begin();
}

void loop() {
	lm35();

	sensors.requestTemperatures();
	clcd(1);
	lcd.print(sensors.getTempCByIndex(0));
	lcd.print(" c");
}

void lm35() {
	delay(500);
	float volts = readV(TMP36_IN);
	if (lm35pr < V_PR) {
		lm35Vs += volts;
		lm35pr++;
		return;
	}
	float tmp36Volt = lm35Vs / lm35pr;
	float tmp36Temp = (tmp36Volt - 0.5) * 100.0;
	clcd(0);
	lcd.print(tmp36Temp);
	lcd.print("c->");
	lcd.print(tmp36Volt, 5);
	lcd.print("v");
	resetLm35();
}

void resetLm35() {
	lm35pr = 0;
	lm35Vs = 0;
}

float readV(short port) {
	return (float) analogRead(port) / VOLT_DIV + V_CMP;
}

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}
