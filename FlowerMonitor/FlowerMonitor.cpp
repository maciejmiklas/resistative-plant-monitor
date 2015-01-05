#include "FlowerMonitor.h"

Time time;
Moisture moisture;

void setup() {
	timer_reset();
	lcd_init();
	hygro_init(&moisture);
	Serial.begin(115200);
}

void loop() {
	util_cycle();

	hygro_update(&moisture);
	if (moisture.status & MS_INCREASED) {
		timer_reset();
	}
	if (moisture.status & MS_CHANGED) {
		lcd_printMoisture(&moisture);
	}
	timer_sample(&time);
	lcd_printClock(&time);
	lcd_bright();

	delay(100);
}


