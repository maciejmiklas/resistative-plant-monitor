#include "FlowerMonitor.h"

Time time;
Moisture moisture;

void setup() {
	timer_reset();
	lcd_init();
	hydro_init(&moisture);
	Serial.begin(115200);
}

void loop() {
	hydro_update(&moisture);
	if (moisture.status & MS_CHANGED) {
		timer_reset();
	}
	if (moisture.status & MS_INCREASED) {
		lcd_printMoisture(&moisture);
	}

	timer_sample(&time);
	lcd_printClock(&time);

	delay(500);
}


