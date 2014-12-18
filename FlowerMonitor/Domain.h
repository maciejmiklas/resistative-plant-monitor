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

typedef struct {
	uint16_t proc;
	uint16_t maxProc;
	boolean changed;
	boolean increased;
} Moisture;

void domain_sort(uint16_t arr[], uint16_t size);

#endif /* DOMAIN_H_ */
