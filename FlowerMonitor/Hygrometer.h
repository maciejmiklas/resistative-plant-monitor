#ifndef Hygrometer_H_
#define Hygrometer_H_

#include <Arduino.h>
#include "Domain.h"
#include "Log.h"

void hygro_cycle(Moisture *moisture);
void hygro_init(Moisture *moisture);

#endif /* Hygrometer_H_ */
