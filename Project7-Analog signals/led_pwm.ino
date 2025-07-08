#include <MD_PWM.h>

const int rotaryPin = A0;
const int ledPin = 4;

int previousValue = -1;
int brightness = 0;

MD_PWM ledPWM(ledPin);  // Create MD_PWM object for pin 4

void setup() {
  Serial.begin(9600);
  ledPWM.begin(50);    // Initialize PWM on pin 4 with 50 (or other) Hz
  ledPWM.enable();   // Enable PWM output
}

void loop() {
  int sensorValue = analogRead(rotaryPin);

  brightness = map(sensorValue, 0, 1023, 0, 255);

  if (brightness != previousValue) {
    ledPWM.write(brightness);   // Set PWM duty cycle (0-255)
    previousValue = brightness;

    Serial.print("Brightness: ");
    Serial.println(brightness);
  }
  delay(10);
}