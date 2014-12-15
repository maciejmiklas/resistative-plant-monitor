#include "FlowerSensors.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#define PROC_PROBES 30
#define UPROC 9999
uint16_t procs[PROC_PROBES];
uint16_t maxProc = 0;
uint16_t lastProc = 0;

boolean blinkMark = true;
uint16_t readProcCnt = 0;
Time time;
long timeMS;

// 1023 / 5 = 204.6
#define VOLT_DIV 204.6f

#define HG_IN 0

struct DTime {
	uint16_t dd;
	uint16_t hh;
	uint16_t mm;
	uint16_t ss;
};
DTime dTime;

void setup() {
	//Serial.begin(9600);
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	setupLcdStatic();
}

void loop() {
	uint16_t proc = calcProc();
	if (proc != UPROC && proc != lastProc) {
		if (proc > lastProc + 5) {
			timeMS = millis();
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

uint16_t calcProc() {
	readProcCnt++;
	procs[readProcCnt] = readProc();
	if (readProcCnt != PROC_PROBES - 1) {
		return UPROC;
	}

	sort(procs, PROC_PROBES);
	readProcCnt = 0;
	return procs[PROC_PROBES / 2];
}

void sort(uint16_t arr[], uint16_t size) {
	uint16_t i, temp, j;
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
	sample(&time);

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

void printProc(uint16_t proc, uint16_t maxProc) {
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

uint16_t readProc() {
	int read = analogRead(HG_IN);
	uint16_t proc = (1018 - read) / 7.48;

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

//long milis =  126000000 + 1440000 + 17000;// 1d 11h 24m 17s
//long milis =  345600000 + 1440000 + 17000;// 4d 0h 24m 17s
//long milis =  446400000 + 1440000 + 17000;// 5d 4h 24m 17s
void sample(Time *ts) {
	long milis = millis() - timeMS;
	long sec = milis / 1000;

	// days
	uint16_t dd = sec / 86400;
	ts->dd = dd;
	sprintf(ts->cdd, "%03d", dd);
	uint16_t cs = sec - dd * 86400;

	//Serial.print("cs1:");
	//Serial.print(cs);

	// hours
	uint16_t hh = cs / 3600;
	cs -= hh * 3600;
	ts->hh = hh;
	sprintf(ts->chh, "%02d", hh);

	//Serial.print(",cs2:");
	//Serial.print(cs);

	// minutes
	uint16_t mm = cs / 60;
	cs -= mm * 60;
	ts->mm = mm;
	sprintf(ts->cmm, "%02d", mm);

	//Serial.print(",cs3:");
	//Serial.print(cs);

	// seconds
	uint16_t ss = cs;
	ts->ss = ss;
	sprintf(ts->css, "%02d", ss);

	//Serial.print(",milis:");
	//Serial.print(milis);

	//Serial.print(",sec:");
	//Serial.print(sec);

//	Serial.print(",dd:");
//	Serial.print(dd);
//
//	Serial.print(",hh:");
//	Serial.print(hh);
//
//	Serial.print(",mm:");
//	Serial.print(mm);
//
//	Serial.print(",ss:");
//	Serial.println(ss);

}

