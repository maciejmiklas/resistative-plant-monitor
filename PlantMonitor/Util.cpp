#include <Util.h>

uint32_t cycleMilis;

void util_sort(uint8_t arr[], uint8_t size) {
	uint8_t i, temp, j;
	for (i = 1; i < size; i++) {
		temp = arr[i];
		j = i - 1;
		while ((temp < arr[j]) && (j >= 0)) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

void util_init() {
	util_cycle();
}

void util_cycle() {
	cycleMilis = millis();
}

uint32_t util_millis() {
	return cycleMilis;
}

uint16_t util_freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (uint16_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
