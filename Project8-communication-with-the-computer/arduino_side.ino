
/*Control how long a LED will light when pressing a button on the arduino side. The ON time is configured by user interface in the computer side

The arduino is programmed to turn on the built-in LED (pin 13) when a button (on pin 6) is pressed.

The LED stays on for a custom amount of time (sent over serial), then turns off. It uses serial input to receive the desired LED-on duration in milliseconds, and reads until newline or until there are no more characters.

The program uses the MsTimer2 library to schedule the turn-off, using a timer.
*/


#include <MsTimer2.h>

const int ledPin = 13;       // Built-in LED pin
const int buttonPin = 6;     // Grove button connected to D4
const int interruptPin = 3;

volatile bool timerRunning = false;
volatile int secondsPassed = 0;

bool timerActive = false;
unsigned long ledOnDuration = 1000;  // Default LED on-time in milliseconds
volatile bool buttonPressed = false;  // Flag set in interrupt

// function to turn off led and subsequently reset the active timer flag
void turnOffLED() {
  digitalWrite(ledPin, LOW);
  timerActive = false;
}

// Interrupt service routine (ISR) — runs when button is pressed
void buttonISR() {
  int buttonState = digitalRead(buttonPin);  // Read button state

  if (buttonState == HIGH) {
  // Button was pressed
  digitalWrite(ledPin, HIGH);
  Serial.println("LED is ON");

  secondsPassed = 0;
  timerRunning = true;

  MsTimer2::set(ledOnDuration, turnOffLED);  // turn off led after unspecified duration
  MsTimer2::start();
  }

}

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);    // Interrupt pin with pull-up
  pinMode(buttonPin, INPUT);  

  //digitalWrite(ledPin, LOW);   // Start with led off
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonISR, RISING);        // Triggers when button goes from LOW to HIGH
}

void loop() {
  // ----------- Serial reading block -----------
  // checks for new incoming numbers and filters potential errors like typos
  if (Serial.available() > 0) {         // any data waiting on the serial port?
    String inputString = "";            // initialize an empty string to collect digits from input
    bool validNumber = true;            // make sure it is a number and not a typo with letters

    // Read characters until newline or no more chars
    while (Serial.available() > 0) {
      char c = Serial.read();           // read each character at a time

      if (isDigit(c)) {
        inputString += c;               // if character is a digit, add it to the string
      } else if (c == '\n' || c == '\r') {    // stop reading if character is a newline or carriage break
        break;  // End of input
      } else {                          // If an invalid character is found, clear input and discard the rest of the line.
        inputString = "";
        validNumber = false;
        // Flush remaining input
        while (Serial.available() > 0) Serial.read();
        break;
      }
      delay(2); // Small delay to allow input buffer fill
    }

    // If a valid number was received, convert it to an integer and store it in ledOnDuration (+1 ms as a known workaround).
    if (validNumber && inputString.length() > 0) {
      unsigned long receivedTime = inputString.toInt();
      ledOnDuration = receivedTime + 1;  // Add 1 ms as per your bug fix
      Serial.print("I received: ");
      Serial.println(receivedTime);
    } else if (!validNumber) {
      Serial.println("Error: Invalid number received");
    }
  }

  delay(10);  // Small delay to reduce CPU usage
}