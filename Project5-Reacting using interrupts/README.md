# Project 5 - Reacting to input part I: Interrupts

1. Make led light on button press
2. Learn how to debug code by establish communication with the computer using the Serial library
3. Understand the difference between polling and interrupt and benefits of using interrupts over polling
4. Learn how to use interrupts in Arduino

## Light led on button press (arduino code)
 - button pin is 6, grove led pin is 4
 - read value from button pin. If button is pressed (how can you tell? -- Using digitalRead, if the button "buttonPin" is pressed, it is given a 'High' value, or 1; as opposed to 0 when not pressed) turn grove led on. If button is not pressed, turn it off
 - test that this works.

## Debug statements
- use the Serial library to add debug statements to your code. View using Serial Monitor in Arduino IDE

## Simulate additional long process
- We wish to simulate a long process that happens in the loop. In your lab, this could be a calculation, data acquisition, etc. In order to simulate this, add to your loop the following code:

```
for (int i = 0; i< 10000; i++){
    Serial.println("calculating...");
}
```
- test if lighting led still works. Why or why not?
answer here: ___Lighting does not work - I can see in the serial monitor that it is stuck on 'calculation...' since we gave it a loop to print 'calculation...' 10 thousand times. It takes too much time and there isn't the opportunity to execute the action_______

## Use interrupt to light led
- Add code to create an interrupt pin (why can't it be the button pin?). Use a variable for this.
-- This arduino board only has two interrupt pins, 2 and 3; the button pin is not programmed as an interrupt pin and cannot be used with attachInterrupt
- Short the interrupt pin with the button pin
- test. Does it work?
-- it does work, with pin 3 (interrupt=1, on).

## Exercises
 - commit and upload your code in this project folder.

