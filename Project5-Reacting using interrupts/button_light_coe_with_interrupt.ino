const int buttonPin = 6;   // Grove button pin
const int ledPin = 4;      // Grove LED pin
const int interruptPin = 3; // Interrupt pin (jumpered from pin 6); when pin 3 is activated, it is 1 (high)

volatile bool interruptTriggered = false;

void handleInterrupt() {
  interruptTriggered = true;
}

void setup() {
  pinMode(buttonPin, INPUT);         // Grove button is on D6
  pinMode(interruptPin, INPUT);      // Set D2 (jumpered from D6) as input
  pinMode(ledPin, OUTPUT);           // LED on D4
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if (interruptTriggered) {
    interruptTriggered = false;
    Serial.println("Interrupt triggered!");
  }

  int buttonState = digitalRead(buttonPin);
  Serial.print("Button state: ");
  Serial.println(buttonState);

  for (int i = 0; i < 10000; i++) {
    Serial.println("calculating...");
    if (interruptTriggered) break;
  }

  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED is OFF");
  }
}
