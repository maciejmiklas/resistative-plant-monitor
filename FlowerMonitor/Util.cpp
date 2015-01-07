#include "Util.h"

uint32_t cycleMilis;

void util_cycle() {
	cycleMilis = millis();
}

uint32_t util_millis() {
	return cycleMilis;
}

