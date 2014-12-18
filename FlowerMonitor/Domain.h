#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <Arduino.h>

typedef struct {
	uint16_t dd;
	char cdd[4];

	uint16_t hh;
	char chh[3];

	uint16_t mm;
	char cmm[3];

	uint16_t ss;
	char css[3];
} Time;

enum {
	MS_CHANGED = 0x01,
	MS_INCREASED = 0x02,
	MS_ALL = 0x7F
};

typedef struct {
	uint16_t proc;
	uint16_t maxProc;
	byte status;
} Moisture;

void domain_sort(uint16_t arr[], uint16_t size);

#endif /* DOMAIN_H_ */
