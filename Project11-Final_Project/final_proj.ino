#include <Servo.h>
#include "Arduino_SensorKit.h"

Servo myServo;        // Create servo object
const int potPin = A0;      // Rotary potentiometer on A0
const int fanPin = 3;       // Mini fan connected to D5 (PWM capable)
const int servoPin = 7; // Servo on D7

void setup() {
  pinMode(fanPin, OUTPUT);
  myServo.attach(servoPin);  // Attach servo to pin D7

  //while(!Serial);
  //Accelerometer.begin();
  SensorKit.begin();

  Serial.begin(9600);
}

void loop() {

  // ---- Fan control from potentiometer ----
  int potValue = analogRead(potPin);         // Read potentiometer (0 to 1023)
  int pwmValue = map(potValue, 0, 1023, 0, 255); // Map to PWM range
  analogWrite(fanPin, pwmValue);                // Set fan speed

  // Estimate RPM (for demonstration purposes only)
  float estimatedRPM = (pwmValue / 255.0) * 7000; // Assume max ~7000 RPM

  // ---- Accelerometer reading using SensorKit ----
  float x = Accelerometer.readX();  // Get X-axis acceleration

  // Map X-axis to servo angle (scaled to degrees) -- max servo angle = 164 from previous findings
  int angle = map(x * 100, -100, 100, 0, 164);
  angle = constrain(angle, 0, 164);
  myServo.write(angle);

  // Debug info in Serial Monitor
 Serial.print("PWM: ");
 Serial.print(pwmValue);
 Serial.print(" | Estimated RPM: ");
 Serial.println((int)estimatedRPM);  // send integer RPM to avoid float parsing


  delay(100); // Small delay to stabilize readings
}
