#include "FlowerSensors.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Timer tmr;
short maxProc = 0;
boolean blinkMark = true;
short lastProc = 0;
#define PROC_PROBES 30
short procs[PROC_PROBES];
short readProcCnt = 0;

struct DTime {
	int dd;
	short hh;
	short mm;
	short ss;
};
DTime dTime;

void setup() {
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	setupLcdStatic();
}

void loop() {
	short proc = calcProc();
	if (proc != -1 && proc != lastProc) {
		if (proc > lastProc + 5) {
			tmr.restart();
		}
		if (proc > lastProc) {
			maxProc = proc;
		}
		lastProc = proc;
		printProc(proc, maxProc);
	}

	updateClock();

	delay(500);
}

short calcProc() {
	readProcCnt++;
	procs[readProcCnt] = readProc();
	if (readProcCnt != PROC_PROBES - 1) {
		return -1;
	}

	sort(procs, PROC_PROBES);
	readProcCnt = 0;
	return procs[PROC_PROBES / 2];
}

void sort(short arr[], short size) {
	short i, temp, j;
	for (i = 1; i < size; i++) {
		temp = arr[i];
		j = i - 1;
		while ((temp < arr[j]) && (j >= 0)) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

void setupLcdStatic() {
	// row 0
	clcd(0);
	lcd.print("NOW:");
	lcd.setCursor(6, 0);
	lcd.print("%  MAX:");
	lcd.setCursor(15, 0);
	lcd.print("%");

	// row 1
	clcd(1);
	lcd.print("000 --~ 00:00:00");
}

void updateClock() {
	Timer::Time time = tmr.sample();

	// dd
	if (dTime.dd != time.dd) {
		lcd.setCursor(0, 1);
		lcd.print(time.cdd);
		dTime.dd = time.dd;
	}

	// hh
	if (dTime.hh != time.hh) {
		lcd.setCursor(8, 1);
		lcd.print(time.chh);
		dTime.hh = time.hh;
	}

	// mm
	if (dTime.mm != time.mm) {
		lcd.setCursor(11, 1);
		lcd.print(time.cmm);
		dTime.mm = time.mm;
	}

	// ss
	lcd.setCursor(13, 1);
	lcd.print(blinkMark ? ":" : " ");
	blinkMark = !blinkMark;

	if (dTime.ss != time.ss) {
		lcd.setCursor(14, 1);
		lcd.print(time.css);
		dTime.ss = time.ss;
	}
}

void printProc(short proc, short maxProc) {
	clcd(0);
	lcd.print("NOW:");
	char pch[3];
	sprintf(pch, "%02d", proc);
	lcd.print(pch);
	lcd.print("%");

	lcd.print("  MAX:");
	sprintf(pch, "%02d", maxProc);
	lcd.print(pch);
	lcd.print("%");
}

short readProc() {
	int read = analogRead(HG_IN);
	short proc = (1018 - read) / 7.48;

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

