const int buttonPin = 6;   // Grove button pin
const int ledPin = 4;      // Grove LED pin
const int interruptPin = 3; // Interrupt pin (jumpered from pin 6); when pin 3 is activated, it is 1 (high)

int buttonState = 0

volatile bool interruptTriggered = false;

void setup() {
  pinMode(buttonPin, INPUT);         // Grove button is on D6
  pinMode(interruptPin, INPUT_PULLUP);      // Set D2 (jumpered from D6) as input
  pinMode(ledPin, OUTPUT);           // LED on D4
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
  Serial.begin(9600);
}

void handleInterrupt() {
  interruptTriggered = true;

  int buttonState = digitalRead(buttonPin);
  Serial.print("Button state: ");
  Serial.println(buttonState);

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED is OFF");
  }
}

void loop() {
  if (interruptTriggered) {
    interruptTriggered = false;
    Serial.println("Interrupt triggered!");
  }

  for (int i = 0; i < 10000; i++) {
    Serial.println("calculating...");
  }
}

