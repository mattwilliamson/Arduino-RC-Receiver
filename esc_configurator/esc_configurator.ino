// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo.write(179);
  Serial.begin(115200); 
  Serial.println("Servo controller. l/m/h OR L/M/H"); 
  Serial.println("Connect servo to pin 9"); 
} 
 
void loop() 
{ 
} 


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    switch(inChar) {
      case 'l':
      case 'L':
        myservo.write(0);
        Serial.println("Servo set to low"); 
        break;
     
      case 'm':
      case 'M':
        myservo.write(89);
        Serial.println("Servo set to medium"); 
        break;   
        
      case 'h':
      case 'H':
        myservo.write(179);
        Serial.println("Servo set to HIGH"); 
        break;
    }
  }
}
