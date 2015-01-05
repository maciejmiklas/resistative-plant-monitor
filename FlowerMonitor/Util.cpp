#include "Timer.h"

#define TR__MS_SEC 1000
#define TR__SEC_DD 86400
#define TR__SEC_HH 3600
#define TR__SEC_MM 60

uint32_t cycleMilis;

void util_cycle() {
	cycleMilis = millis();
}

uint32_t util_millis() {
	return cycleMilis;
}

