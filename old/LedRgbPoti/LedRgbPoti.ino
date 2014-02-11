
int SWITCH_PIN = 2;
int BLUE_PIN = 9;
int GREEN_PIN = 10;
int RED_PIN = 11;

int onLed = 11;
int lastBight = 0;

void setup() {
  Serial.begin(9600);

  pinMode(SWITCH_PIN, INPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  
  analogWrite(BLUE_PIN, 50);
  analogWrite(GREEN_PIN, 50);
  analogWrite(RED_PIN, 50);
}


void loop() {
  
  int buttonState = digitalRead(SWITCH_PIN);  
  if(buttonState == 0){
    onLed++;
    if(onLed == 12) {
      onLed = 9;
    }    
    delay(300);
  }  
  
  int sensorValue = analogRead(A0); 
  int brightness = sensorValue/4; 
  if(lastBight != brightness){  
    lastBight = brightness;
    analogWrite(onLed, brightness);
    String lnc = " -> ";
    String ln = onLed + lnc + brightness;
    Serial.println(ln);
  }
  delay(10);
  
}
