#ifndef Log_H_
#define Log_H_

#include <Arduino.h>
#include <stdarg.h>
#include "Domain.h"
#include "Timer.h"

void log_init();
void log_cycle();
void log_ln(const char *fmt, ...);

#endif /* Log_H_ */
