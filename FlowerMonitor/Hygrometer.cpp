#include "Hygrometer.h"

#define NO_VALUE 222

uint8_t readProcCnt = 0;

#define MESURE_MS 100
#define PROC_PROBES 30
#define MIN_TO_CHANGE 5

uint8_t procs[PROC_PROBES];
uint32_t moistureIncreasedMs = 0;
uint32_t moistureMesureMs = 0;

uint8_t readProc() {
	uint16_t read = analogRead(MOISTURE_READ_PIN);
	uint16_t proc = (1023 - read) / 8;
	if (proc < 0) {
		proc = 0;
	} else if (proc > 100) {
		proc = 99;
	}
	return proc;
}

uint8_t calcProc() {
	readProcCnt++;
	procs[readProcCnt] = readProc();
	if (readProcCnt != PROC_PROBES - 1) {
		return NO_VALUE;
	}

	domain_sort(procs, PROC_PROBES);
	readProcCnt = 0;
	return procs[PROC_PROBES / 2 + 1];
}

void hygro_init(Moisture *moisture) {
	ln("Initializing hygrometer module");
	moisture->status = 0 | MS_CHANGED;
	moisture->maxProc = 0;
	moisture->proc = 0;
	moistureMesureMs = timer_millis();
}

void hygro_cycle(Moisture *moisture) {
	moisture->status &= ~(MS_CHANGED | MS_INCREASED);
	if (timer_millis() - moistureMesureMs < MESURE_MS) {
		return;
	}

	moistureMesureMs = timer_millis();

	uint8_t proc = calcProc();
	if (proc != NO_VALUE && abs(proc - moisture->proc) >= MIN_TO_CHANGE) {
		moisture->status |= MS_CHANGED;
		if (proc > moisture->proc + MIN_TO_CHANGE) {
			moisture->status |= MS_INCREASED;
			moisture->maxProc = proc;
			moistureIncreasedMs = timer_millis();

		} else if ((timer_millis() - moistureIncreasedMs)
				< MOISTURE_MAX_ADOPT_MS) {
			ln("Reseting max proc - time since last watering < %u",
					MOISTURE_MAX_ADOPT_MS);
			moisture->maxProc = proc;

		} else if (proc > moisture->maxProc) {
			moisture->maxProc = proc;
		}
		moisture->proc = proc;

		ln("Moisture has changed = %u%%, Max: %u%%, Status: %d", moisture->proc,
				moisture->maxProc, moisture->status);
	}
}

