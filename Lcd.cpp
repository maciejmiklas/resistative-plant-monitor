#include "Lcd.h"

typedef struct {
	uint16_t dd;
	uint16_t hh;
	uint16_t mm;
	uint16_t ss;
} DTime;
DTime dTime;

int16_t lastLightAdjustVal = 0;
int16_t lastLightVal = 0;
uint32_t lastUpdate = 0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
boolean blinkMark = true;

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

void lcd_init() {
	ln("Initializing LCD module");

	pinMode(LCD_LIGHT_SENS_PIN, INPUT);
	pinMode(LCD_LIGHT_ADJUST_PIN, INPUT);

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

void lcd_printTime(Time *time) {
	if ((util_millis() - lastUpdate) < LCD_CLOCK_UPDATE_MS) {
		return;
	}
	lastUpdate = util_millis();
	char buf[4];

	// dd
	if (dTime.dd != time->dd) {
		lcd.setCursor(0, 1);
		sprintf(buf, "%03d", time->dd);
		lcd.print(buf);
		dTime.dd = time->dd;
	}

	// hh
	if (dTime.hh != time->hh) {
		lcd.setCursor(8, 1);
		sprintf(buf, "%02d", time->hh);
		lcd.print(buf);
		dTime.hh = time->hh;
	}

	// mm
	if (dTime.mm != time->mm) {
		lcd.setCursor(11, 1);
		sprintf(buf, "%02d", time->mm);
		lcd.print(buf);
		dTime.mm = time->mm;
	}

	// ss
	lcd.setCursor(13, 1);
	lcd.print(blinkMark ? ":" : " ");
	blinkMark = !blinkMark;

	if (dTime.ss != time->ss) {
		lcd.setCursor(14, 1);
		sprintf(buf, "%02d", time->ss);
		lcd.print(buf);
		dTime.ss = time->ss;
	}
}

/*
 * adopt LCD back light on changing light conditions
 * Light sensor: 200 - 1000 (bright - dark)
 * LCD backlight: 200 - 50 (bright, dark)
 * [LCD backlight] = (1267 - [light sensor]) / 5.33
 */
void lcd_cycle() {
	int16_t lightAdjustVal = analogRead(LCD_LIGHT_ADJUST_PIN);
	int16_t lightVal = analogRead(LCD_LIGHT_SENS_PIN);

	if ( abs(lastLightVal - lightVal) >= LCD_LIGHT_SESN_SENSITIVITY
			|| abs(lightAdjustVal - lastLightAdjustVal)
					>= LCD_LIGHT_ADJUST_SENSITIVITY) {

		lastLightAdjustVal = lightAdjustVal;
		lastLightVal = lightVal;

		int16_t lcdLight = (800 + lightAdjustVal - lightVal) / 5;

		ln("Adopting LCD backlight. Sensor: %u, LCD: %u, Adjust(def 500): %u",
				lightVal, lcdLight, lightAdjustVal);

		if (lcdLight > LCD_BACKLIGHT_MAX) {
			lcdLight = LCD_BACKLIGHT_MAX;
		} else if (lcdLight < LCD_BACKLIGHT_MIN) {
			lcdLight = LCD_BACKLIGHT_MIN;
		}

		analogWrite(LCD_BACKLIGHT_PIN, lcdLight);
	}
}

