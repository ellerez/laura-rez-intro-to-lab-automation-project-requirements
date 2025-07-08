#include <MsTimer2.h>

const int ledPin = 13;       // Built-in LED pin
const int buttonPin = 6;     // Grove button connected to D4

bool timerActive = false;
unsigned long ledOnDuration = 1000;  // Default LED on time in milliseconds
bool lastButtonState = HIGH;          // Button state tracking (pull-up means HIGH when not pressed)

// Function called when timer expires to turn off the LED
void turn_off() {
  digitalWrite(ledPin, LOW);
  timerActive = false;
}

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(buttonPin, INPUT_PULLUP);  // Grove button is active low, enable internal pull-up
}

void loop() {
  // ----------- Serial reading block -----------
  if (Serial.available() > 0) {
    String inputString = "";
    bool validNumber = true;

    // Read characters until newline or no more chars
    while (Serial.available() > 0) {
      char c = Serial.read();

      if (isDigit(c)) {
        inputString += c;
      } else if (c == '\n' || c == '\r') {
        break;  // End of input
      } else {
        // Invalid character found, clear input and set error flag
        inputString = "";
        validNumber = false;
        // Flush remaining input
        while (Serial.available() > 0) Serial.read();
        break;
      }
      delay(2); // Small delay to allow input buffer fill
    }

    if (validNumber && inputString.length() > 0) {
      unsigned long receivedTime = inputString.toInt();
      ledOnDuration = receivedTime + 1;  // Add 1 ms as per your bug fix
      Serial.print("I received: ");
      Serial.println(receivedTime);
    } else if (!validNumber) {
      Serial.println("Error: Invalid number received");
    }
  }

  // ----------- Button polling block -----------
  bool currentButtonState = digitalRead(buttonPin);

  // Detect button press: HIGH->LOW transition (pressed)
  if (lastButtonState == HIGH && currentButtonState == LOW && !timerActive) {
    digitalWrite(ledPin, HIGH);  // Turn LED on
    timerActive = true;

    MsTimer2::stop();
    MsTimer2::set(ledOnDuration, turn_off);  // Set timer with received duration
    MsTimer2::start();
  }
  lastButtonState = currentButtonState;

  // Optional: small delay for debouncing and CPU relief
  delay(10);
}