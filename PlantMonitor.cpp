#include "PlantMonitor.h"

Time time;
Moisture* moisture;
uint32_t wateringMs;

void setup() {
	util_setup();
	log_setup();
	lcd_setup();
	moisture = hygro_setup();
	alarm_setup();
	wateringMs = util_millis();
}

// "xxx_loop()" methods are meant to be called on every loop. Those are non blocking methods.
void loop() {
	// util has to be called as first, because it creates snapshot of milliseconds (util_millis())
	// that is being used by most functions.
	util_cycle();
	log_cycle();

	hygro_cycle(moisture);
	if (moisture->status & MS_INCREASED) {
		ln("Recognized plant watering, status: %u", moisture->status);
		wateringMs = util_millis();
	}
	if (moisture->status & MS_CHANGED) {
		lcd_printMoisture(moisture);
	}

	// update time passed by since last watering
	timer_sample(&time, wateringMs);
	lcd_printTime(&time);

	// adopt LCD back light on changing light conditions
	lcd_cycle();

	// eventually blink sos
	alarm_cycle(moisture->proc);

	delay(100);
}
