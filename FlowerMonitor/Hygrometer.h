#ifndef Hygrometer_H_
#define Hygrometer_H_

#include <Arduino.h>
#include "Domain.h"
#include "Util.h"

#define MOISTURE_READ_PIN 0

/*
 * After plant watering the moisture level can jump very high due to the water flowing across sensor.
 * Max time will be calculated as the lowest value during adoption time. Adoption timer starts
 * after we recognize significantly increased moisture level. Default is 5 minutes.
 */
#define MOISTURE_MAX_ADOPT_MS 300000

void hygro_update(Moisture *moisture);
void hygro_init(Moisture *moisture);

#endif /* Hygrometer_H_ */
