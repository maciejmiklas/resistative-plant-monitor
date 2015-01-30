#include "Log.h"

Time lt;

void log_init() {
	Serial.begin(115200);
}

void log_cycle() {
	timer_sample(&lt);
}

void startl() {
	char buf[26];
	sprintf(buf, ">>[%03u-%02u:%02u:%02u,%03u]-> ", lt.dd, lt.hh, lt.mm, lt.ss,
			lt.ml);
	Serial.print(buf);
}

void ln(const char *fmt, ...) {
	startl();
	char buf[64];
	va_list va;
	va_start(va, fmt);
	vsprintf(buf, fmt, va);
	va_end(va);
	Serial.println(buf);
}

