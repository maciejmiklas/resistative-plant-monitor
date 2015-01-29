#include "Sos.h"

uint8_t state;
uint16_t switchMs = 0;
uint8_t lastPinVal = -1;

void sos_reset() {
	state = 1;
	digitalWrite(SOS_PIN, LOW);
	switchMs = util_millis();
}

void doSwitch(uint16_t duration) {
	if (util_millis() - switchMs >= duration) {
		state++;
		switchMs = util_millis();
	}
}
// States:
// 1 - LED ON for long time
// 2 - LED OFF
// 3 - LED ON for long time
// 4 - LED OFF
// 5 - LED ON for long time
// 6 - LED OFF
// 7 - LED ON for short time
// 8 - LED OFF
// 9 - LED ON for short time
// 10 - LED OFF
// 11 - LED ON for short time
// 12 - LED OFF
void sos_next() {
	uint8_t pinVal = HIGH;

	// led off
	if ((state & 1) == 0) {
		doSwitch (SOS_OFF_DURATION);
		pinVal = LOW;
	}
	// led on for long time
	else if (state <= 5) {
		doSwitch (SOS_ON_LONG_DURATION);
		pinVal = HIGH;
	} else if (state <= 11) {
		doSwitch (SOS_ON_SHORT_DURATION);
		pinVal = HIGH;
	} else {
		state = 1;
	}

	if (lastPinVal != pinVal) {
		digitalWrite(SOS_PIN, pinVal);
	}
	lastPinVal = pinVal;
}

