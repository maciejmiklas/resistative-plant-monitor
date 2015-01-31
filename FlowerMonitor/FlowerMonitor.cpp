#include "FlowerMonitor.h"

Time time;
Moisture moisture;
uint32_t wateringMs;

void setup() {
	timer_init();
	log_init();
	lcd_init();
	hygro_init(&moisture);
	alarm_init();
	wateringMs = timer_millis();
}

void loop() {
	timer_cycle();
	log_cycle();
	hygro_cycle(&moisture);

	if (moisture.status & MS_INCREASED) {
		ln("Recognized plant watering, status: %u", moisture.status);
		wateringMs = timer_millis();
	}
	if (moisture.status & MS_CHANGED) {
		lcd_printMoisture(&moisture);
	}
	timer_sample(&time, wateringMs);
	lcd_printTime(&time);
	lcd_bright();

	alarm_cycle(moisture.proc);
}
