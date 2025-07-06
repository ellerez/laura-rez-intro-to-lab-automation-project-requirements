// Define the button pin, Grove LED pin, and interrupt pin
const int buttonPin = 6; // Button pin
const int ledPin = 4;    // Grove LED pin
const int interruptPin = 6; // Interrupt pin (shorted with button pin)

// Variable to track the interrupt state
volatile bool interruptTriggered = false;

// Interrupt Service Routine (ISR)
void handleInterrupt() {
  interruptTriggered = true;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // Initialize the button pin as input
  pinMode(buttonPin, INPUT);
  // Initialize the LED pin as output
  pinMode(ledPin, OUTPUT);
  // Attach an interrupt to the button pin (shorted with interrupt pin)
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
  // Start Serial communication for debugging
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  // Check if the interrupt was triggered
  if (interruptTriggered) {
    interruptTriggered = false; // Reset the interrupt flag
    // Debug statement to indicate interrupt was triggered
    Serial.println("Interrupt triggered!");
  }

  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Debug statement to print the button state
  Serial.print("Button state: ");
  Serial.println(buttonState);

  // Check if the button is pressed
  if (buttonState == HIGH) {
    // Turn the LED on
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is ON");
  } else {
    // Turn the LED off
    digitalWrite(ledPin, LOW);
    Serial.println("LED is OFF");
  }
}