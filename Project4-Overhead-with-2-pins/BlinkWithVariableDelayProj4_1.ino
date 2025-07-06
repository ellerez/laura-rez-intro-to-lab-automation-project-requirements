/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// Define the LED pin
const int ledPin = 13; // builtin led on in 13
const int ledPin2 = 12; // "LED" on pin 12
// Define the delay duration (ms)
const int delayDuration = 1; // Set delay to 1 ms
// Define the delay duration (us)
const int delayDurationUS = 5; // Set delay to 1 ms
// Define a variable for calculations
int calculationResult = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}


// the loop function runs over and over again forever
void loop() {

  // blink on both pins simultaneously
  digitalWrite(ledPin, HIGH);  // turn the LED on (pin 13)
  // Perform a calculation (count iterations) and store the result
  calculationResult += 1; // Increment the variable by 1
  digitalWrite(ledPin2, HIGH);  // turn the LED on (pin 12)
  delay(delayDuration);  // keep on for 1 ms
 
  // blink off both pins simultaneously
  digitalWrite(ledPin, LOW);   // turn the LED off (pin 13)
  digitalWrite(ledPin2, LOW);   // turn the LED off (pin 12)
  delay(delayDuration);         // wait for 1 ms     
}