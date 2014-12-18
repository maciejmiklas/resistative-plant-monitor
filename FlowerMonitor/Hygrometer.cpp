#include "Hygrometer.h"

#define UPROC 9999

uint16_t readProcCnt = 0;
uint16_t maxProc = 0;
uint16_t lastProc = 0;

#define PROC_PROBES 30
uint16_t procs[PROC_PROBES];

uint16_t readProc() {
	int read = analogRead(HYDRO_READ_PIN);
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
	return procs[PROC_PROBES / 2];
}

void hydro_init(Moisture *moisture) {
	moisture->changed = true;
	moisture->increased = false;
	moisture->maxProc = 0;
	moisture->proc = 0;
}

void hydro_update(Moisture *moisture) {
	moisture->changed = false;
	moisture->increased = false;

	uint16_t proc = calcProc();
	if (proc != UPROC && proc != lastProc) {
		moisture->changed = true;
		if (proc > lastProc + 5) {
			moisture->increased = true;
		}
		if (proc > lastProc) {
			maxProc = proc;
		}
		lastProc = proc;

		moisture->maxProc = maxProc;
		moisture->proc = proc;
	}
}

