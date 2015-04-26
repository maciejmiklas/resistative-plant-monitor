#include "Lcd.h"

static uint16_t lastLightAdjustVal = 0;
static uint16_t lastLightSensorVal = 0;
static uint32_t lastUpdate = 0;
static LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
static boolean blinkMark = true;
static Time lastTime;

static void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

void lcd_setup() {
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
	lcd.print("0000 -> 00:00:00");

	lastUpdate = util_millis();
}

void lcd_printMoisture(Moisture *moisture) {
	lcd.setCursor(4, 0);
	char pch[4];
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
	char buf[5];

	// dd
	if (lastTime.dd != time->dd) {
		lcd.setCursor(0, 1);
		sprintf(buf, "%04d", time->dd);
		lcd.print(buf);
		lastTime.dd = time->dd;
	}

	// hh
	if (lastTime.hh != time->hh) {
		lcd.setCursor(8, 1);
		sprintf(buf, "%02d", time->hh);
		lcd.print(buf);
		lastTime.hh = time->hh;
	}

	// mm
	if (lastTime.mm != time->mm) {
		lcd.setCursor(11, 1);
		sprintf(buf, "%02d", time->mm);
		lcd.print(buf);
		lastTime.mm = time->mm;
	}

	// ss
	lcd.setCursor(13, 1);
	lcd.print(blinkMark ? ":" : " ");
	blinkMark = !blinkMark;

	if (lastTime.ss != time->ss) {
		lcd.setCursor(14, 1);
		sprintf(buf, "%02d", time->ss);
		lcd.print(buf);
		lastTime.ss = time->ss;
	}
}

void lcd_cycle() {
	uint16_t lightAdjustVal = analogRead(LCD_LIGHT_ADJUST_PIN);
	uint16_t lightSensorVal = analogRead(LCD_LIGHT_SENS_PIN);

	boolean adjust = false;
	if (util_abs16(lightAdjustVal - lastLightAdjustVal) >= LCD_LIGHT_ADJUST_SENSITIVITY) {
		lastLightAdjustVal = lightAdjustVal;
		adjust = true;
	}
	if (util_abs16(lastLightSensorVal - lightSensorVal) >= LCD_LIGHT_SESN_SENSITIVITY || adjust) {
		lastLightSensorVal = lightSensorVal;
		int16_t lcdLight = adoptLcdBacklight(lightSensorVal, lightAdjustVal);

		if (lcdLight > LCD_BACKLIGHT_MAX) {
			lcdLight = LCD_BACKLIGHT_MAX;
		} else if (lcdLight < LCD_BACKLIGHT_MIN) {
			lcdLight = LCD_BACKLIGHT_MIN;
		}
		analogWrite(LCD_BACKLIGHT_PIN, lcdLight);
	}
}

