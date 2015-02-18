#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <Arduino.h>

void util_setup();
void util_cycle();

void util_sort(uint8_t arr[], uint8_t size);
uint32_t util_millis();
uint16_t util_freeRam();

#endif /* DOMAIN_H_ */
