// Do not remove the include below
#include "TwoLCDs.h"


//The setup function is called once at startup of the sketch
void setup()
{
	  // initialize digital pin 13 as an output.
	  pinMode(13, OUTPUT);
}

// The loop function is called in an endless loop
void loop()
{
	  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	  delay(100);              // wait for a second
	  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	  delay(1000);              // wait for a second
}
