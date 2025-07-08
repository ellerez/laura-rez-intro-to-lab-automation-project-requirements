#include <Servo.h>

Servo myServo;        // Create servo object
const int potPin = A0;  // Rotary sensor on A0
const int servoPin = 7; // Servo on D7

void setup() {
  myServo.attach(servoPin);  // Attach servo to pin D7
  Serial.begin(9600);      
}

void loop() {
  int potValue = analogRead(potPin);               // Read rotary sensor (0–1023)
  int angle = map(potValue, 0, 1023, 0, 164);      // Map to servo angle (0–180; altered to 164, the maximum angle possible to relate to 1023 after which the motor struggles to match peak signal from potentiometer)
  
  myServo.write(angle);                            // Rotate servo
  delay(15);  // Small delay to allow servo to move
  
  // Optional debug output
  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print(" → Angle: ");
  Serial.println(angle);
}