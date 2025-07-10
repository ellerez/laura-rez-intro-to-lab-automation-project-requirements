#include <Servo.h>
#include "Arduino_SensorKit.h"

Servo myServo;        // Create servo object
const int potPin = A0;      // Rotary potentiometer on A0
const int fanPin = 3;       // Mini fan connected to D5 (PWM capable)
const int servoPin = 7; // Servo on D7

void setup() {
  pinMode(fanPin, OUTPUT);
  myServo.attach(servoPin);  // Attach servo to pin D7

  while(!Serial);
  Accelerometer.begin();
  //SensorKit.begin();

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
  float y = Accelerometer.readY();  // Get Z-axis acceleration
  
  // Map Z-axis to servo angle (scaled to degrees)
  int angle = map(y * 100, -100, 100, 0, 180);
  angle = constrain(angle, 0, 180);
  myServo.write(angle);

  // Debug info in Serial Monitor
  Serial.print("PWM: ");
  Serial.print(pwmValue);
  Serial.print(" | Estimated RPM: ");
  Serial.println((int)estimatedRPM);  // send integer RPM to avoid float parsing
  Serial.print(" | Servo Angle: ");
  Serial.print(angle);
  Serial.print(" | Y-Accel: ");
  Serial.println(y);


  delay(100); // Small delay to stabilize readings
}