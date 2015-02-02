#ifndef Log_H_
#define Log_H_

#include <Arduino.h>
#include <stdarg.h>
#include "Timer.h"
#include "Util.h"

void log_init();
void log_cycle();

void ln(const char *fmt, ...);

#endif /* Log_H_ */
