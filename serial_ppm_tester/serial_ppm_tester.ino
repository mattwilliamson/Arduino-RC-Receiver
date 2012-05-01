volatile uint16_t rcValue[9] = {0,0,0,0,0,0,0,0,0}; // interval [1000;2000]
volatile bool ledOn = false;

static void rxInt() {
  ledOn = !ledOn;
  uint16_t now,diff;
  static uint16_t last = 0;
  static uint8_t chan = 0;

  now = micros();
  diff = now - last;
  last = now;
//  if(diff>3000) { 
//    chan = 0;
//  }
//  else {
//    if( 900 < diff && diff < 2200 && chan < 8 ) {
	  rcValue[chan] = diff;
//	}
    chan++;
    chan %= 9;
//  }
  if(ledOn)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);  
  pinMode(2, INPUT);
  pinMode(3, INPUT); 
  attachInterrupt(0, rxInt, RISING);
}

void loop() {
  Serial.print("Channels: ");
  for(int i=0; i<9; i++) {
    Serial.print(rcValue[i]);
    Serial.print(",");
  }
  Serial.println("");
  delay(500);
}
