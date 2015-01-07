#include "Lcd.h"

typedef struct {
	uint16_t dd;
	uint16_t hh;
	uint16_t mm;
	uint16_t ss;
} DTime;
DTime dTime;
int lightSensorVal = 0;

uint32_t lastUpdate = 0;

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
	pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
	analogWrite(LCD_BACKLIGHT_PIN, LCD_BACKLIGHT_INIT);

	// row 0
	clcd(0);
	lcd.print("NOW:00%  MAX:00%");

	// row 1
	clcd(1);
	lcd.print("000 --~ 00:00:00");

	lastUpdate = util_millis();
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
	if ((util_millis() - lastUpdate) < LCD_CLOCK_UPDATE_MS) {
		return;
	}
	lastUpdate = util_millis();

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

/*
 * Light sensor: 200 - 1000 (bright - dark)
 * LCD backlight: 200 - 50 (bright, dark)
 * [LCD backlight] = (1267 - [light sensor]) / 5.33
 */
void lcd_bright() {
	int lightVal = analogRead(LCD_LIGHT_SENS_PIN);
	if ( abs(lightSensorVal - lightVal) >= LCD_LIGHT_SESN_SENSITIVITY) {
		lightSensorVal = lightVal;
		int lcdLight = (1267 - lightVal) / 5.33;
		analogWrite(LCD_BACKLIGHT_PIN, lcdLight);
	}
}

