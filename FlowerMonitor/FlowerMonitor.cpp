#include "FlowerMonitor.h"

Time time;
Moisture moisture;

void setup() {
	timer_reset();
	lcd_init();
	hydro_init(&moisture);
	//Serial.begin(115200);
}

void loop() {
	hydro_update(&moisture);
	if (moisture.increased) {
		timer_reset();
	}
	if (moisture.changed) {
		lcd_printMoisture(&moisture);
	}

	timer_sample(&time);
	lcd_printClock(&time);

	delay(500);
}
