#include "NanoTest.h"

int pinVal = 0;
uint8_t pin = 9;

boolean blink = false;

void setup() {
	pinMode(pin, OUTPUT);
	pinMode(3, OUTPUT);

	pinMode(5, OUTPUT);
	analogWrite(5, 20);

	pinMode(13, OUTPUT);
}

void loop() {
	pinVal+=5;
	if (pinVal >= 255) {
		pinVal = 0;
	}
	analogWrite(pin, pinVal);
	analogWrite(3, pinVal);
	delay(10);

	if (blink == true) {
		blink = false;
		digitalWrite(13, HIGH);
	} else {
		blink = true;
		digitalWrite(13, LOW);
	}
}
