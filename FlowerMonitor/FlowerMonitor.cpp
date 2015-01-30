#include "FlowerMonitor.h"

Time time;
Moisture moisture;

void setup1() {
	log_init();
	timer_reset();
	lcd_init();
	hygro_init(&moisture);
	sos_reset();
}

void loop1() {
	util_cycle();
	log_cycle();

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

void setup() {
	util_cycle();
	timer_reset();
	log_init();
}

void loop() {
	util_cycle();
	log_cycle();

	ln("Hi there !");
	delay(763);
}
