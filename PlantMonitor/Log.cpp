#include "Log.h"

Time lt;
uint32_t runMs;
uint32_t lastStatusMs = 0;

void log_init() {
	Serial.begin(115200);
	runMs = util_millis();
}

void log_status() {
	if (util_millis() - lastStatusMs < LOG_PRINT_STATUS_MS) {
		return;
	}
	lastStatusMs = util_millis();
	uint32_t free = util_freeRam();
	ln("Status -> Free RAM: %u", free);
}

void log_cycle() {
	timer_sample(&lt, runMs);
	log_status();
}

void ln(const char *fmt, ...) {
	char buf[80];

	// print time
	sprintf(buf, ">>[%03u-%02u:%02u:%02u,%03u]-> ", lt.dd, lt.hh, lt.mm, lt.ss,
			lt.ml);
	Serial.print(buf);

	// print the message
	va_list va;
	va_start(va, fmt);
	vsprintf(buf, fmt, va);
	va_end(va);
	Serial.println(buf);
}

