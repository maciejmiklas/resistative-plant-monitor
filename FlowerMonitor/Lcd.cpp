#include "Lcd.h"

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
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	// row 0
	clcd(0);
	lcd.print("NOW:00%  MAX:00%");

	// row 1
	clcd(1);
	lcd.print("000 --~ 00:00:00");
}

void lcd_printMoisture(Moisture *moisture) {
	lcd.setCursor(4, 0);
	char pch[3];
	sprintf(pch, "%02d", moisture->proc);
	lcd.print(pch);

	lcd.setCursor(13, 0);
	sprintf(pch, "%02d", moisture->maxProc);
	lcd.print(pch);
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

void lcd_bright() {
	//int phVal = analogRead(PHOTO_SENS_PIN);
}

