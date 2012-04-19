#include <Servo.h> 

#define SERVOS 8
#define PIN_LED 13
#define LED_TIME 1000
#define MIN_SERVO_POS 40
#define MAX_SERVO_POS 150
#define AUTO_DESCENT_ENABLED
#define AUTO_DESCENT_TIMEOUT 1500
#define AUTO_DESCENT_RATE 1 // Per 100ms

Servo servos[SERVOS];
int positions[SERVOS];
byte buffer[SERVOS];
int bytesReceived;
byte currentByte;
unsigned long lastReceived;
unsigned long lastLed;
boolean armed = false;

void setup() 
{ 
  pinMode(PIN_LED, OUTPUT); 
  Serial.begin(38400);
  bytesReceived = 0;
  lastReceived = 0;
  lastLed = 0;

  for(int i=0; i<SERVOS; i++) {
    positions[i] = MIN_SERVO_POS;
    servos[i].attach(2 + i);
    servos[i].write(MIN_SERVO_POS);
  }
} 


void loop() 
{
  if(lastLed && abs(millis() - lastReceived) > LED_TIME) {
    digitalWrite(PIN_LED, LOW);
    lastLed = 0;
  }

  if (Serial.available() > 0) {
    digitalWrite(PIN_LED, HIGH);
    lastReceived = millis();
    lastLed = lastReceived;
    currentByte = Serial.read();
    armed = true;

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
        int servoPos = map(buffer[i], 0, 250, MIN_SERVO_POS, MAX_SERVO_POS);
        servos[i].write(servoPos);
        positions[i] = servoPos;
      }
    }
  }

  if(!armed) {
    // Autodescent
#ifdef AUTO_DESCENT_ENABLED
    servos[2].write((MAX_SERVO_POS + MIN_SERVO_POS) / 2);
    positions[2] = (MAX_SERVO_POS + MIN_SERVO_POS) / 2;
    servos[3].write((MAX_SERVO_POS + MIN_SERVO_POS) / 2);
    positions[3] = (MAX_SERVO_POS + MIN_SERVO_POS) / 2;
    servos[4].write((MAX_SERVO_POS + MIN_SERVO_POS) / 2);
    positions[4] = (MAX_SERVO_POS + MIN_SERVO_POS) / 2;

    // Aux1
    servos[5].write(MAX_SERVO_POS);
    positions[5] = MAX_SERVO_POS;
#endif
    

    // Throttle
    int stepInterval = 100;

    while (positions[1] > MIN_SERVO_POS) {
      delay(stepInterval / 2);
      digitalWrite(PIN_LED, HIGH);

      positions[1] -= AUTO_DESCENT_RATE;
      positions[1] = positions[1] > MIN_SERVO_POS ? positions[1] : MIN_SERVO_POS;
      servos[1].write(positions[1]);

      delay(stepInterval / 2);
      digitalWrite(PIN_LED, LOW);
    }

    // Not Armed yet (no packets received), blink the LED
    digitalWrite(PIN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_LED, LOW);
    delay(500);
  }

#ifdef AUTO_DESCENT_ENABLED
  unsigned long now = millis();

  if(lastReceived > 0 && lastReceived < now && (now - lastReceived) > AUTO_DESCENT_TIMEOUT) {
    armed = false;
  }
#endif
} 

