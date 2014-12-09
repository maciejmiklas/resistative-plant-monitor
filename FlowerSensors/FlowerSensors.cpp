#include "FlowerSensors.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Timer tmr;
double lproc = 0;

void setup() {
	lcd.begin(16, 2);
	lcd.noAutoscroll();
}

void loop() {
	double proc = hproc();
	if (proc > lproc) {
		tmr.restart();
	}
	clcd(0);
	lcd.print(proc);
	lcd.print(" %");

	clcd(1);
	Timer::Time time = tmr.sample();
	lcd.print(time.hh);
	lcd.print(":");
	lcd.print(time.mm);
	lcd.print(":");
	lcd.print(time.ss);

	delay(1000);
	lproc = proc;
}

double hproc() {
	int read = analogRead(HG_IN);
	//double proc = 120 - (3* read)/25;
	double proc = (1018 - read) / 7.48;

	if (proc < 0) {
		proc = 0;
	} else if (proc > 100) {
		proc = 100;
	}
	return proc;
}

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

