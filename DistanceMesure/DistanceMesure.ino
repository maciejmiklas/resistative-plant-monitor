#include <LiquidCrystal.h>

#define HC_ECHO_PIN 7
#define HC_TRIG_PIN 8 
#define MIN_DIST 2 

int lastDistance = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  lcd.print("Distance in cm:");
  
 //Serial.begin (9600);
 pinMode(HC_TRIG_PIN, OUTPUT);
 pinMode(HC_ECHO_PIN, INPUT);
 
 
}

void loop() {
    int dist = distance();
  if(dist < 0){
    return;
  }
  if(abs(dist-lastDistance) > MIN_DIST){
    lastDistance = dist;  
    
    lcd.setCursor(0, 1);    
    lcd.print(dist);    
  }
  
  delay(10);
}

int distance(){
  // init
  digitalWrite(HC_TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  
  // send 8x 40 khz pulse
  digitalWrite(HC_TRIG_PIN, HIGH);
  delayMicroseconds(10);   
  digitalWrite(HC_TRIG_PIN, LOW);
  
  // wait for response (wave reflected by opticle)
  int duration = pulseIn(HC_ECHO_PIN, HIGH);
 
  int distance = duration/58.2;
  return distance;
}
