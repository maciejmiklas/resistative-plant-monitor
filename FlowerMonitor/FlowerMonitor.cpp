// Do not remove the include below
#include "FlowerMonitor.h"

#include "Timer.h"
#include "Lcd.h"

Time time;

void setup() {
	timer_reset();
	lcd_init();
	Serial.begin(115200);
}

void loop() {
	timer_sample(&time);
	lcd_printClock(&time);
	delay(1000);
}
