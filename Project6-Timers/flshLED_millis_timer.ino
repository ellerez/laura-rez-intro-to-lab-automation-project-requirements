const int buttonPin = 6;         // Grove button pin
const int ledPin = 4;            // Grove LED pin
const int interruptPin = 3;      // Interrupt pin (jumpered from pin 6)
const long interval = 5000;      // 5 seconds

unsigned long ledOnTime = 0;     // Time when the LED was turned on
bool ledState = false;           // Track if LED is currently ON
bool ledTimerActive = false;     // Track if the timer is running

void handleInterrupt() {
  int buttonState = digitalRead(buttonPin);  // Read button state

  if (buttonState == HIGH) {
    // Button was pressed
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is ON");

    ledOnTime = millis();      // Start 5-second timer
    ledState = true;
    ledTimerActive = true;
  }
}

void setup() {
  pinMode(buttonPin, INPUT);              // Grove button input
  pinMode(interruptPin, INPUT_PULLUP);    // Interrupt pin with pull-up
  pinMode(ledPin, OUTPUT);                // Grove LED output

  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, RISING);
  Serial.begin(9600);
}

void loop() {
  // Check if the 5-second timer is active
  if (ledTimerActive && (millis() - ledOnTime >= interval)) {
    digitalWrite(ledPin, LOW);
    Serial.println("LED is OFF");

    ledState = false;
    ledTimerActive = false;
  }

  // Simulated workload
  //for (int i = 0; i < 10000; i++) {
    //Serial.println("calculating...");
  //}
}