#include "Hygrometer.h"

#define UPROC 9999

uint16_t readProcCnt = 0;

#define PROC_PROBES 30
uint16_t procs[PROC_PROBES];
uint32_t moistureIncreasedMs = 0;

uint16_t readProc() {
	int read = analogRead(MOISTURE_READ_PIN);
	uint16_t proc = (1018 - read) / 7.48;

	if (proc < 0) {
		proc = 0;
	} else if (proc > 100) {
		proc = 99;
	}
	return proc;
}

uint16_t calcProc() {
	readProcCnt++;
	procs[readProcCnt] = readProc();
	if (readProcCnt != PROC_PROBES - 1) {
		return UPROC;
	}

	domain_sort(procs, PROC_PROBES);
	readProcCnt = 0;
	return procs[PROC_PROBES / 2 + 1];
}

void hygro_init(Moisture *moisture) {
	moisture->status = 0 | MS_CHANGED;
	moisture->maxProc = 0;
	moisture->proc = 0;
}

void hygro_update(Moisture *moisture) {
	moisture->status &= ~(MS_CHANGED | MS_INCREASED);

	uint16_t proc = calcProc();
	if (proc != UPROC && proc != moisture->proc) {

		moisture->status |= MS_CHANGED;

		if (proc > moisture->proc + 5) {
			moisture->status |= MS_INCREASED;
			moisture->maxProc = proc;
			moistureIncreasedMs = util_millis();

		} else if ((util_millis() - moistureIncreasedMs) < MOISTURE_MAX_ADOPT_MS) {
			moisture->maxProc = proc;

		} else if (proc > moisture->maxProc) {
			moisture->maxProc = proc;
		}
		moisture->proc = proc;
	}
}

