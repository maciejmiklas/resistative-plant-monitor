#ifndef Lcd_H_
#define Lcd_H_

#include <Arduino.h>
#include "Timer.h"

void lcd_init();
void lcd_printClock(Time *time);

#endif /* Lcd_H_ */
