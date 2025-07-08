#include <Servo.h>

Servo myServo;  // create servo object

void setup() {
  Serial.begin(9600);

  myServo.attach(7);  // attach servo to pin 7
}

void loop() {

  myServo.write(90);  // Hold servo at 90°
  delay(1000);
}

