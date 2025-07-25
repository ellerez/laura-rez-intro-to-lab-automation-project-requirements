# Project8.1 - Two way communication between the computer and the Arduino board using firmataexpress

## Goals
1. Understand the firmataexpress package and its usage in actual project
2. Understand the use of jupyter notebooks for exploration

## Instructions
1. Install the firmataexpress package in the Arduino IDE. upload the firmataexpress sketch to the Arduino board.
2. Install the [pymata4 package](https://mryslab.github.io/pymata4/install_pymata4/) in the python environment - pip install pymata4 
3. follow along with the jupyter notebook.

**IMPORTANT!** the pymata4 package sometimes causes crashes. The computers reset when booting and all your code will be lost in this case. Therefore, **commit and push often**.

## Requirements

1. Develop a Python script that communicates with an Arduino device using the pymata4 library.
2. Set up a pin for digital input and monitor its changes.
3. React to button presses on the Arduino. When the button is pressed, perform an action (e.g., print a message, turn on an LED).
5. use threading.Timer (from the threading python package) to turn off the led. This will allow the script to perform other tasks while waiting for the button press. This version of the script should be named Reacting_button_press_pymata4_using_timer.py.
6. Add a Graphical User Interface (GUI) to the timer version of the script. The GUI should display the state of the button (pressed or not pressed) and any actions performed in response to the button press. plus, it should have a textbox input to accept different time intervals for the timer.
7. light the led for 30 ms. Measure in scopy the length of the led light a 5-10 times and measure the average and standard deviation. Compare to previous arduino and non firmata python code.
- 40.925, 44.484, 48.043, 40.925, 72.954, 48.043, 55.160, 56.940, 40.925, 40.925
-- MEAN = 48.932 ms STD = 9.787 ms
8. Include comments explaining the functionality of each part of the code.
9. Write the script in a clean, organized, and efficient manner following good programming practices.


## Exercises
Paste a screenshot of the GUI here:
![alt text](image.png)

Paste a screenshot of the logic analyzer here that presents the time the LED is ON when pressing the button.
![alt text](image-1.png)
```
What is the difference between the two versions (with pymata4 and firmata and without) of the script? Which one was easier to code? Where was thread handling easier?

Answer: __Programming and executing the code and feedback through python and the gui is much slower than running the pogram in the arduino. Programming was simpler in arduino since we didn't have to include a gui. Thread handling was pretty similar in both, though had fewer lines of code in python. ________


What can you say about the accuracy of the timing between the firmata version and your arduino code version?
which one is more accurate and why?

The arduino version was much more accurate. There is probably a delay in the timing due to the firmata code interpreting the commands from python.

What are the advantages and disadvantages of using the pymata4 package?

Answer: __Advantages: simply put, a much wider range of opportunities for programming. Disadvantages: less accurate timing.________