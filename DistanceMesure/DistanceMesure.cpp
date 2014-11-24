// Do not remove the include below
#include "DistanceMesure.h"

#include <LiquidCrystal.h>

#define HC_ECHO_PIN 7
#define HC_TRIG_PIN 8
#define MIN_DIST 2

// the amount of characters used in each lcd row to output live data
#define LCD_CHRS 8

int lastDistance = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	lcd.setCursor(11, 0);
	lcd.print("d(cm)");

	lcd.setCursor(11, 1);
	lcd.print("t(ms)");

	//Serial.begin (9600);
	pinMode(HC_TRIG_PIN, OUTPUT);
	pinMode(HC_ECHO_PIN, INPUT);
}

void clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("          ");
}

int distance() {
	// init
	digitalWrite(HC_TRIG_PIN, LOW);
	delayMicroseconds(5);

	// send 8x 40 khz pulse
	digitalWrite(HC_TRIG_PIN, HIGH);
	delayMicroseconds(10);

	// wait for response (wave reflected by opticle)
	int duration = pulseIn(HC_ECHO_PIN, HIGH);

	// print duration in second line on lcd
	double durms = (double) duration / (double) 1000;
	clcd(1);
	lcd.setCursor(0, 1);
	lcd.print(durms);

	int distance = duration / 58.5;
	return distance;
}

void loop() {
	int dist = distance();
	if (dist < 0) {
		return;
	}
	if (abs(dist-lastDistance) > MIN_DIST) {
		lastDistance = dist;
		clcd(0);
		lcd.setCursor(0, 0);
		lcd.print(dist);
	}

	delay(500);
}
