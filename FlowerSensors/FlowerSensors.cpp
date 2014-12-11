#include "FlowerSensors.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Timer tmr;
unsigned short maxProc = 0;
unsigned short lastProc = 0;
short setProcCnt = 0;
boolean blinkMark = true;

void setup() {
	lcd.begin(16, 2);
	lcd.noAutoscroll();
}

void loop() {
	unsigned short proc = rproc();
	if (proc > lastProc + 10) {
		tmr.restart();
		maxProc = proc;
	}

	// make sure to record each rise of read from sensor
	if (proc > maxProc) {
		maxProc = proc;
	}

	printProc(proc, maxProc);

	// read #lastProc every 10 seconds to wait enough time for change on the sensors
	setProcCnt++;
	if (setProcCnt == 10) {
		setProcCnt = 0;
		lastProc = proc;
	}

	delay(1000);
}

void printProc(unsigned short proc, unsigned short maxProc) {
	clcd(0);
	lcd.print("NOW:");
	char pch[4];
	sprintf(pch, "%02d", proc);
	lcd.print(pch);
	lcd.print("%");

	lcd.print("  MAX:");
	sprintf(pch, "%02d", maxProc);
	lcd.print(pch);
	lcd.print("%");

	clcd(1);
	Timer::Time time = tmr.sample();
	lcd.print(time.cdd);
	lcd.print(" --~ ");
	lcd.print(time.chh);
	lcd.print(":");
	lcd.print(time.cmm);

	lcd.print(blinkMark ? ":" : " ");
	blinkMark = !blinkMark;

	lcd.print(time.css);
}

unsigned short rproc() {
	int read = analogRead(HG_IN);
	unsigned short proc = (1018 - read) / 7.48;

	if (proc < 0) {
		proc = 0;
	} else if (proc > 100) {
		proc = 99;
	}
	return proc;
}

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

