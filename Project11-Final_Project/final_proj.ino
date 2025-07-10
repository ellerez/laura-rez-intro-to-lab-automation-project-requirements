/*Final project: control fan speed with rotary potentiometer, control servo angle with light sensor (I also prepared the control with the accelerometer, but chose to proceed with the light sensor as the rotation is smooter), 
   and control buzzer and fan speed based on servo angle (buzzer on and fan speed 0 rpm when angle < 30 degrees).
   The angle of the rotary motor is displayed in the OLED display.
   The estimated RPM of the fan is calculated based on the PWM value (assuming the maximum rpm for the fan of 7000).
*/


#include <Servo.h>
#include "Arduino_SensorKit.h"

Servo myServo;

const int potPin = A0;       // Rotary potentiometer
const int fanPin = 3;        // Mini fan on D3 (PWM capable)
const int servoPin = 7;      // Servo on D7
const int lightSensorPin = A6; // Light sensor is hard-wired to A6
const int buzzerPin = 5;      // Buzzer on D5


bool buzzerOn = false;        // Track buzzer state
int fanSpeed = 0;             // Current fan speed PWM value

void setup() {
  // Initialize pins, OLED display, and serial communication
  pinMode(fanPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  myServo.attach(servoPin);

  Oled.begin();
  Oled.setFlipMode(true);

  digitalWrite(buzzerPin, LOW);  // buzzer off at start

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

  // Control buzzer and fan based on angle threshold
  if (angle < 30) {
    
    tone(buzzerPin, 1000);  // turn passive buzzer ON at 1kHz
    buzzerOn = true;
    // digitalWrite(buzzerPin, HIGH);  // turn buzzer ON
    analogWrite(fanPin, 0);          // stop fan
    
  } else {
    
    noTone(buzzerPin);      // turn passive buzzer OFF
    buzzerOn = false;
    // digitalWrite(buzzerPin, LOW);   // turn buzzer OFF
    analogWrite(fanPin, pwmValue);  // resume fan speed from rotary
    
    }

  // ----- Serial output -----
  Serial.print("PWM: ");
  Serial.print(pwmValue);
  Serial.print(" | Estimated RPM: ");
  Serial.print((int)estimatedRPM);
  Serial.print(" | Light Level: ");
  Serial.print(lightValue);
  Serial.print("ANGLE: ");
  Serial.print(angle);
  Serial.print(" | BUZZER: ");
  Serial.println(buzzerOn ? "ON" : "OFF");

  delay(100);
}