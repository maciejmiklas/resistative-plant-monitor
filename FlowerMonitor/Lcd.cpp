#include "Lcd.h"
#include <LiquidCrystal.h>

typedef struct {
	uint16_t dd;
	uint16_t hh;
	uint16_t mm;
	uint16_t ss;
} DTime;
DTime dTime;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

boolean blinkMark = true;

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

void lcd_init() {
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

void lcd_printClock(Time *time) {

	// dd
	if (dTime.dd != time->dd) {
		lcd.setCursor(0, 1);
		lcd.print(time->cdd);
		dTime.dd = time->dd;
	}

	// hh
	if (dTime.hh != time->hh) {
		lcd.setCursor(8, 1);
		lcd.print(time->chh);
		dTime.hh = time->hh;
	}

	// mm
	if (dTime.mm != time->mm) {
		lcd.setCursor(11, 1);
		lcd.print(time->cmm);
		dTime.mm = time->mm;
	}

	// ss
	lcd.setCursor(13, 1);
	lcd.print(blinkMark ? ":" : " ");
	blinkMark = !blinkMark;

	if (dTime.ss != time->ss) {
		lcd.setCursor(14, 1);
		lcd.print(time->css);
		dTime.ss = time->ss;
	}
}
