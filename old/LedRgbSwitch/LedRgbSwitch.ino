
int pushButtonPin = 2;

int ledBluePin = 11;
int ledGreenPin = 12;
int ledRedPin = 13;
int onLed = 11;
void setup() {

  Serial.begin(9600);

  pinMode(pushButtonPin, INPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
}


void loop() {
  
  int buttonState = digitalRead(pushButtonPin);  
  if(buttonState == 0){
    onLed++;
    digitalWrite(ledBluePin,  0);
    digitalWrite(ledGreenPin,  0);
    digitalWrite(ledRedPin,  0);
  
    digitalWrite(onLed,  1);
  
     delay(300);
     Serial.println(onLed);
     if(onLed == 13){
      onLed = 10;
    }
  }  
  

  delay(10);
}
