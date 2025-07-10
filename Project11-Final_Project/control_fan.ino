const int potPin = A0;      // Rotary potentiometer on A0
const int fanPin = 3;       // Mini fan connected to D3 (PWM capable and doesn't make the noise as on D5)

void setup() {
  pinMode(fanPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);         // Read potentiometer (0 to 1023)
  int pwmValue = map(potValue, 0, 1023, 0, 255); // Map to PWM range
  analogWrite(fanPin, pwmValue);                // Set fan speed

  // Estimate RPM (for demonstration purposes only)
  float estimatedRPM = (pwmValue / 255.0) * 7000; // Assume max ~7000 RPM

  // Debug info in Serial Monitor
 Serial.print("PWM: ");
Serial.print(pwmValue);
Serial.print(" | Estimated RPM: ");
Serial.println((int)estimatedRPM);  // send integer RPM to avoid float parsing


  delay(100); // Small delay to stabilize readings
}
