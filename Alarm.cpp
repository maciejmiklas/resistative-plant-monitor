#include "Alarm.h"

int16_t lastAlarmThreshold = 0;
uint8_t alarmThresholdProc = 30;

void alarm_cycle(uint8_t moisturePorc) {
	int16_t alarmThreshold = analogRead(ALARM_THRESHOLD_PIN);
	if (abs(alarmThreshold-lastAlarmThreshold) >= ALARM_ADJUST_SENSITIVITY) {
		lastAlarmThreshold = alarmThreshold;
		alarmThresholdProc = map(alarmThreshold, 0, 1025, 0, 100);
		ln("Adjusted alarm sensitivity. %u = %u%%", alarmThreshold,
				alarmThresholdProc);
	}

	// sos
	if (moisturePorc <= alarmThresholdProc) {
		sos_on();
	} else {
		sos_off();
	}

	sos_cycle();
}

void alarm_setup() {
	pinMode(ALARM_THRESHOLD_PIN, INPUT);
	sos_setup();
}
