#include "FlowerMonitor.h"

Time time;
Moisture moisture;

uint32_t wateringMs;
void setup1() {
	timer_init();
	log_init();
	lcd_init();
	hygro_init(&moisture);
	sos_init();
	wateringMs = timer_millis();
}

void loop1() {
	timer_cycle();
	log_cycle();

	hygro_update(&moisture);
	if (moisture.status & MS_INCREASED) {
		ln("Recognized plant watering");
		wateringMs = timer_millis();
	}
	if (moisture.status & MS_CHANGED) {
		lcd_printMoisture(&moisture);
	}
	timer_sample(&time, wateringMs);
	lcd_printTime(&time, wateringMs);
	lcd_bright();

	delay(100);
}

void setup() {
	timer_init();
	log_init();
	sos_init();
}

void loop() {
	timer_cycle();
	log_cycle();
	sos_next();
}
