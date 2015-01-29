#include "Log.h"

Time time;

void log_init() {
	Serial.begin(115200);
}

void log_cycle() {
	timer_sample(&time);
}

void startl() {
	char buf[48];
	// TODO add milis and micros
	sprintf(buf, ">> [%03d-%02d:%02d:%02d,%03d] ->", time.dd, time.hh, time.mm,
			time.ss, time.ml);
	Serial.print(buf);
}

void log_ln(const char *fmt, ...) {
	startl();
	char buf[64];
	va_list va;
	va_start(va, fmt);
	vsprintf(buf, fmt, va);
	va_end(va);
	Serial.print(buf);
}

