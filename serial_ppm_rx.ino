#include <Servo.h> 

#define SERVOS 8
#define PIN_LED 13
#define LED_TIME 1000

Servo servos[SERVOS];
int positions[SERVOS];
byte buffer[SERVOS];
int bytesReceived;
byte currentByte;
int lastReceived;
 
void setup() 
{ 
  pinMode(PIN_LED, OUTPUT); 
  Serial.begin(38400);
  bytesReceived = 0;
  lastReceived = 0;
  
  for(int i=0; i<SERVOS; i++) {
    positions[i] = 0;
    servos[i].attach(1 + i);
  }
} 
 
 
void loop() 
{
  if(lastReceived && abs(millis() - lastReceived) > LED_TIME) {
    digitalWrite(PIN_LED, LOW);
    lastReceived = 0;
  }
  
  if (Serial.available() > 0) {
    digitalWrite(PIN_LED, HIGH);
    lastReceived = millis();
    currentByte = Serial.read();
    
    if (currentByte == 254) {
      // Either packet is done, or we got corrupt data. Reset the packet
      bytesReceived = 0;
    }
    
    buffer[bytesReceived] = currentByte;
    bytesReceived++;
    
    if(bytesReceived == SERVOS) {
      bytesReceived = 0;
      
      // Send Servo Commands
      for(int i=0; i<SERVOS; i++) {
        int servoPos = map(buffer[i], 0, 250, 0, 179);
        servos[i].write(servoPos);
      }
    }
  }
} 
