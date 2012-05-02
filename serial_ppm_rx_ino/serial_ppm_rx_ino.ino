#include <SoftwareSerial.h>

#define CHANNELS 8

// Set these a little high since they are registering low on AQ
#if defined(__AVR_ATtiny85__)
  // Set up pulse lengths with delays appropriate for ATtiny85 8MHz
  #define MIN_PULSE_TIME 1030   // 1000us, compensated for delay
  #define MAX_PULSE_TIME 1990   // 2000us, compensated for delay
  #define HALF_PULSE_TIME (MIN_PULSE_TIME + MAX_PULSE_TIME) / 2
  #define SYNC_PULSE_TIME 3050  // 3000us, compensated for delay
#else
  // ATmega328 16MHz
  #define MIN_PULSE_TIME 1030   // 1000us, compensated for delay
  #define MAX_PULSE_TIME 1990   // 2000us, compensated for delay
  #define HALF_PULSE_TIME (MIN_PULSE_TIME + MAX_PULSE_TIME) / 2
  #define SYNC_PULSE_TIME 3050  // 3000us, compensated for delay
#endif

#define SERIAL_BAUD 38400

#define PIN_LED 4 // ATtiny85 pin 3
#define PIN_PPM 3 // ATtiny pin 2

SoftwareSerial rxSerial(0, 1); // ATtiny85 pins 6 & 7

#define SERIAL rxSerial

#define RECEIVER_TIMEOUT 1500 // 1.5s
#define MIN_RECEIVER_VALUE 0
#define MAX_RECEIVER_VALUE 250

// For Aeroquad, this should disarm the quad when reception is lost
// TODO: Need to match AQ's channels
// YAXIS,ZAXIS,THROTTLE,XAXIS,AUX1,AUX2,0,0
static unsigned int defaultPulseWidths[CHANNELS] = {
  MIN_PULSE_TIME,      // Throttle
  HALF_PULSE_TIME,     // Roll
  HALF_PULSE_TIME,     // Pitch
  MIN_PULSE_TIME,      // Yaw
  
  MAX_PULSE_TIME,     // AUX1 (MODE in Aeroquad)
  MAX_PULSE_TIME,     // AUX2 (ALTITUDE in Aeroquad)
  MAX_PULSE_TIME,     // AUX3
  MAX_PULSE_TIME      // AUX4
};



unsigned int pulseWidths[CHANNELS];
byte buffer[CHANNELS];
int bytesReceived;
byte currentByte;
unsigned long lastReceived = 0;
boolean armed = false;

// Sync pulse first
int currentChannel = CHANNELS - 1;
unsigned long currentPulse = SYNC_PULSE_TIME;
unsigned long lastPulseMicros = 0;

unsigned long lastLedBlink = 0;
boolean ledOn = false;


void setDefaultPulseWidths() {
  for (int i=0; i<CHANNELS; i++) {
    pulseWidths[i] = defaultPulseWidths[i];
  }
}

void handleSerial() {
  // Handle Serial Data
  if (SERIAL.available()) {
    lastReceived = millis();
    currentByte = SERIAL.read();

    if (currentByte == 254) {
      // Either packet is done, or we got corrupt data. Reset the packet
      bytesReceived = 0;
    } else {
      buffer[bytesReceived] = currentByte;
      bytesReceived++;
    }

    if (bytesReceived == CHANNELS) {
      bytesReceived = 0;
      armed = true;

      // Convert char (0-250) to pulse width (1000-2000)
      for (int i=0; i<CHANNELS; i++) {
        pulseWidths[i] = map(buffer[i], MIN_RECEIVER_VALUE, MAX_RECEIVER_VALUE, 
                                        MIN_PULSE_TIME, MAX_PULSE_TIME);
      }
    }
  }
}

void checkArmed() {
  if(!armed) {
    // Not Armed yet (no packets received) or lost reception, blink the LED
    // and set the dault positions
    setDefaultPulseWidths();
    
    if(lastLedBlink == 0 || (millis() - lastLedBlink) >= 500) {
      lastLedBlink = millis();
      ledOn = !ledOn;
      
      if(ledOn)
        digitalWrite(PIN_LED, HIGH);
      else
        digitalWrite(PIN_LED, LOW);
    }
  } else {
    digitalWrite(PIN_LED, HIGH);
  }
}

void checkLostReception() {
  // Check if we lost reception
  if(armed && lastReceived > 0 && millis() - lastReceived > RECEIVER_TIMEOUT || millis() - lastReceived + 10 > RECEIVER_TIMEOUT) {
    armed = false;
    
    for(int i=0; i<5; i++) {
      digitalWrite(PIN_LED, HIGH);
      delay(100);
      digitalWrite(PIN_LED, LOW);
      delay(100);
    }
  }
}

void sendPulses() {
  if((micros() - lastPulseMicros) >= currentPulse) {
    digitalWrite(PIN_PPM, LOW);
    
    currentChannel++;
    
    if (currentChannel == CHANNELS) {
      // After last channel
      currentPulse = SYNC_PULSE_TIME;
      currentChannel = -1; // Will be 0 on next interrupt
    } else {
      currentPulse = pulseWidths[currentChannel];
    }
    
    digitalWrite(PIN_PPM, HIGH);
    lastPulseMicros = micros();
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PPM, OUTPUT);

  SERIAL.begin(38400);
  
  setDefaultPulseWidths();
  sendPulses();
}

void loop() {
  handleSerial();
  checkArmed();
  checkLostReception();
  sendPulses();
}

