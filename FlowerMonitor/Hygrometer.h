#ifndef Hygrometer_H_
#define Hygrometer_H_

#include <Arduino.h>
#include "Domain.h"

#define HYDRO_READ_PIN 0

void hydro_update(Moisture *moisture);
void hydro_init(Moisture *moisture);

#endif /* Hygrometer_H_ */
