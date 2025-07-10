#include <Servo.h>
#include "Arduino_SensorKit.h"

Servo myServo;

const int potPin = A0;       // Rotary potentiometer
const int fanPin = 3;        // Mini fan on D3 (PWM capable)
const int servoPin = 7;      // Servo on D7
const int lightSensorPin = A6; // Light sensor is hard-wired to A6

void setup() {
  pinMode(fanPin, OUTPUT);
  myServo.attach(servoPin);
  Oled.begin();
  Oled.setFlipMode(true);
  Serial.begin(9600);
}

void loop() {
  // ----- Fan control via potentiometer -----
  int potValue = analogRead(potPin);              // 0 to 1023
  int pwmValue = map(potValue, 0, 1023, 0, 255);   // Map to PWM range
  analogWrite(fanPin, pwmValue);

  // ----- Estimate RPM -----
  float estimatedRPM = (pwmValue / 255.0) * 7000;

  // ----- Read light level and control servo -----
  int lightValue = analogRead(lightSensorPin);         // 0 to ~1023
  int angle = map(lightValue, 0, 1023, 0, 180);         // Map to servo angle
  angle = constrain(angle, 0, 180);
  myServo.write(angle);

  // ----- Print Servo angle in OLED display -----
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0, 3);
  Oled.print("            ");  // print spaces to clear old number
  Oled.setCursor(0, 3);
  Oled.print("Value: ");
  Oled.print(angle);

  // Oled.setCursor(0, 3);
  // Oled.print("Value: ");
  // Oled.print(angle);

  // ----- Serial output -----
  Serial.print("PWM: ");
  Serial.print(pwmValue);
  Serial.print(" | Estimated RPM: ");
  Serial.print((int)estimatedRPM);
  Serial.print(" | Light Level: ");
  Serial.print(lightValue);
  Serial.print(" | Servo Angle: ");
  Serial.println(angle);

  delay(100);
}