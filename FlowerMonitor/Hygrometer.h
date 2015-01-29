#ifndef Hygrometer_H_
#define Hygrometer_H_

#include <Arduino.h>
#include "Domain.h"
#include "Util.h"
#include "Log.h"

void hygro_update(Moisture *moisture);
void hygro_init(Moisture *moisture);

#endif /* Hygrometer_H_ */
