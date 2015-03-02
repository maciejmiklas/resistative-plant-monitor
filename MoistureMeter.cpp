#include "MoistureMeter.h"

static uint32_t moistureIncreasedMs = 0;
static uint32_t timerMs = 0;
static uint8_t procs[PROC_PROBES];
static uint8_t probeIdx = 0;
#define NO_VALUE 222

/**
 * Hygrometer executes measurement in few steps:
 * 1) switch on power to moisture sensor
 * 2) wait for moisture sensor to "warm up"
 * 3) probe moisture level
 * 4) go idle for some time
 * 5) wake up and go to 1)
 * All those steps are implemented with usage of simple state machine.
 */
static void state_startWarmup(Moisture *moisture);
static void state_execWarmup(Moisture *moisture);

static void state_startMeasure(Moisture *moisture);
static void state_execMeasure(Moisture *moisture);

static void state_startIdle(Moisture *moisture);
static void state_execIdle(Moisture *moisture);

void (*state)(Moisture *moisture) = &state_startWarmup;

static uint8_t probeProc() {
	uint16_t read = analogRead(MOISTURE_READ_PIN);
	uint8_t proc = probeToPercent(read);
	if (proc < 0) {
		proc = 0;
	} else if (proc > 100) {
		proc = 99;
	}
	return proc;
}

static void startTimer() {
	timerMs = util_millis();
}

static boolean checkTimer(uint32_t time) {
	return util_millis() - timerMs > time;
}

static uint8_t readProc() {
	if (probeIdx > 0 && !checkTimer(MESURE_PROBE_WAIT_MS)) {
		return NO_VALUE;
	}
	startTimer();

	procs[probeIdx++] = probeProc();
	if (probeIdx < PROC_PROBES) {
		return NO_VALUE;
	}
	util_sort_u8(procs, PROC_PROBES);
	return procs[PROC_PROBES / 2 + 1];
}

static void state_startWarmup(Moisture *moisture) {
	startTimer();
	state = &state_execWarmup;
	digitalWrite(MOISTURE_POWER_PIN, LOW);
}

static void state_execWarmup(Moisture *moisture) {
	if (checkTimer(MOISTURE_WARM_UP_MS)) {
		state = &state_startMeasure;
	}
}

static void state_startMeasure(Moisture *moisture) {
	state = &state_execMeasure;
	probeIdx = 0;
}

static void state_execMeasure(Moisture *moisture) {
	uint8_t proc = readProc();
	if (proc == NO_VALUE) {
		return;
	}
	// power off sensor right after probing
	digitalWrite(MOISTURE_POWER_PIN, HIGH);

	if (util_abs8(proc - moisture->proc) >= MIN_TO_CHANGE) {
		moisture->status |= MS_CHANGED;
		if (proc > moisture->proc + MIN_TO_CHANGE) {
			moisture->status |= MS_INCREASED;
			moisture->maxProc = proc;
			moistureIncreasedMs = util_millis();

		} else if ((util_millis() - moistureIncreasedMs) < MOISTURE_MAX_ADOPT_MS) {
			ln("Reseting max proc based on adoption time after watering");
			moisture->maxProc = proc;

		} else if (proc > moisture->maxProc) {
			moisture->maxProc = proc;
		}
		moisture->proc = proc;

		ln("Moisture has changed = %u%%, Max: %u%%, Status: %d", moisture->proc,
				moisture->maxProc, moisture->status);
	}

	state = &state_startIdle;
}

static void state_startIdle(Moisture *moisture) {
	startTimer();
	state = &state_execIdle;
}

static void state_execIdle(Moisture *moisture) {
	if (checkTimer(MESURE_FREQ_MS)) {
		state = &state_startWarmup;
	}
}

Moisture* mmet_setup() {
	ln("Initializing hygrometer module");

	Moisture *moisture = (Moisture *) malloc(sizeof(Moisture));
	moisture->status = 0 | MS_CHANGED;
	moisture->maxProc = 0;
	moisture->proc = 0;

	timerMs = util_millis();

	pinMode(MOISTURE_POWER_PIN, OUTPUT);

	return moisture;
}

void mmet_cycle(Moisture *moisture) {
	moisture->status &= ~(MS_CHANGED | MS_INCREASED);
	(*state)(moisture);
}

