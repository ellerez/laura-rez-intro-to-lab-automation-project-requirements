#include <MsTimer2.h>

const int buttonPin = 6;         // Grove button pin
const int ledPin = 4;            // Grove LED pin
const int interruptPin = 3;      // Interrupt pin (jumpered from pin 6)

volatile bool timerRunning = false;
volatile int secondsPassed = 0;

void turnOffLED() {

  digitalWrite(ledPin, LOW);
}

void handleInterrupt() {
  int buttonState = digitalRead(buttonPin);  // Read button state

  if (buttonState == HIGH) {
    // Button was pressed
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is ON");

    secondsPassed = 0;
    timerRunning = true;

    MsTimer2::set(5000, turnOffLED);  // turn off led after 5s
    MsTimer2::start();
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
  delay(1000);
  // Simulated workload
  //for (int i = 0; i < 10000; i++) {
    //Serial.println("calculating...");
  //}
}

