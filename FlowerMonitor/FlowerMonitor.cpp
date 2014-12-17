// Do not remove the include below
#include "FlowerMonitor.h"

#include "Timer.h"
#include "Lcd.h"

void setup() {
	timer_reset();
	lcd_init();
	Serial.begin(115200);
}

void loop() {
	Time time;
	timer_sample(&time);
	delay(1000);
}
