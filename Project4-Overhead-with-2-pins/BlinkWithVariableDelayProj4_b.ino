// Define the LED pins
const int ledPin = 12; // Change this to 12 for Grove LED
const int ledPin2 = 13; // LED on pin 13 *** pins must be in same block!
// Define the delay duration (ms)
const int delayDuration = 1; // Set delay to 1 ms

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  // Turn both pins HIGH simultaneously using port manipulation
  PORTD |= (1 << ledPin) | (1 << ledPin2); // Set pins 4 and 12 HIGH
  delay(delayDuration);                    // keep on for 1 ms

  // Turn both pins LOW simultaneously using port manipulation
  PORTD &= ~((1 << ledPin) | (1 << ledPin2)); // Set pins 4 and 12 LOW
  delay(delayDuration);                       // wait for 1 ms
}