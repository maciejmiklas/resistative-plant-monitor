#include "Alarm.h"

int16_t lastAlarmThreshold = 0;
uint8_t alarmThresholdProc = 30;

void alarm_cycle(uint8_t moisturePorc) {
	int16_t alarmThreshold = analogRead(ALATM_THRESHOLD_PIN);
	if (abs(alarmThreshold-lastAlarmThreshold) >= ALATM_ADJUST_SENSITIVITY) {
		lastAlarmThreshold = alarmThreshold;
		alarmThresholdProc = map(alarmThreshold, 0, 1025, 0, 100);
		ln("Adjusted alarm sensitivity. %u = %u%%", alarmThreshold,
				alarmThresholdProc);
	}

	// sos
	if (moisturePorc <= alarmThresholdProc) {
		sos_next();
	} else {
		sos_reset();
	}
}

void alarm_init() {
	pinMode(ALATM_THRESHOLD_PIN, INPUT);
	sos_init();
}
