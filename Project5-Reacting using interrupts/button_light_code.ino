// Define the button pin and Grove LED pin
const int buttonPin = 6; // Button pin
const int ledPin = 4;    // Grove LED pin

// the setup function runs once when you press reset or power the board
void setup() {
  // Initialize the button pin as input
  pinMode(buttonPin, INPUT);
  // Initialize the LED pin as output
  pinMode(ledPin, OUTPUT);
  // Start the Serial communication for debugging
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Debug statement to print the button state
  Serial.print("Button state: ");
  Serial.println(buttonState);

  // Check if the button is pressed
  if (buttonState == HIGH) {
    // Turn the LED on
    digitalWrite(ledPin, HIGH);
    // Debug statement to indicate LED is ON
    Serial.println("LED is ON");
  } else {
    // Turn the LED off
    digitalWrite(ledPin, LOW);
    // Debug statement to indicate LED is OFF
    Serial.println("LED is OFF");
  }

  //for (int i = 0; i< 10000; i++){
    //Serial.println("calculating...");
//}
}