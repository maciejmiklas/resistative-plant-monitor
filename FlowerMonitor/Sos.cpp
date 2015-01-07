#include "Sos.h"

uint8_t state;
uint16_t duration = 0;

void sos_reset() {
	state = 1;
	digitalWrite(SOS_PIN, LOW);
	duration = util_millis();
}

void sos_next() {
	uint8_t pinVal;

	switch (state) {
	// LONG
	case 1:
	case 3:
	case 5:
		if (util_millis() - duration > 300) {
			state++;
			duration = util_millis();
			sos_next();
		}
		pinVal = HIGH;
		break;

		// SHORT
	case 7:
	case 9:
	case 11:
		pinVal = HIGH;
		break;

		// OFF
	case 2:
	case 4:
	case 6:
	case 8:
	case 10:
	case 12:
		if (util_millis() - duration > 50) {
			state++;
			duration = util_millis();
			sos_next();
		}
		pinVal = LOW;
		break;
	}

	digitalWrite(SOS_PIN, pinVal);
}
