#include "Log.h"

Time lt;
uint32_t runMilis;
void log_init() {
	Serial.begin(115200);
	runMilis = util_millis();
}
void log_cycle() {
	timer_sample(&lt, runMilis);
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

