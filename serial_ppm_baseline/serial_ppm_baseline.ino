#include <TimerOne.h> 

#define CHANNELS 8

#define PIN_PPM 9

void setup() {
  pinMode(PIN_PPM, OUTPUT);
  digitalWrite(PIN_PPM, LOW);
}

void loop() {
  // sync pulse
  digitalWrite(PIN_PPM, HIGH);
  delayMicroseconds(3000);
  digitalWrite(PIN_PPM, LOW);
  
  for(int i=0; i<CHANNELS; i++) {
    digitalWrite(PIN_PPM, HIGH);
    delayMicroseconds(2500);
    digitalWrite(PIN_PPM, LOW);
  }
  
} 

