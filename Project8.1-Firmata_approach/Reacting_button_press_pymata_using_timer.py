import time
import sys
from pymata4 import pymata4

print('start')
# Grove Kit Setup
BUTTON_PIN = 6       # Button connected to D6
LED_PIN = 13         # Built-in LED
POLL_TIME = 1        # Polling interval in seconds

# Callback data indices
CB_PIN_MODE = 0
CB_PIN = 1
CB_VALUE = 2
CB_TIME = 3

def the_callback(data):
    """
    A callback function to respond to button changes.
    It prints button state and toggles LED accordingly.
    """
    value = data[CB_VALUE]
    pin = data[CB_PIN]
    timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(data[CB_TIME]))
    
    if value == 0:
        print(f"[{timestamp}] Button PRESSED on pin {pin}")
        board.digital_write(LED_PIN, 1)
    else:
        print(f"[{timestamp}] Button RELEASED on pin {pin}")
        board.digital_write(LED_PIN, 0)

def monitor_button(my_board, pin):
    # Set button as digital input with pull-up resistor
    my_board.set_pin_mode_digital_input_pullup(pin, callback=the_callback)
    
    # Set LED pin as output and ensure it's off
    my_board.set_pin_mode_digital_output(LED_PIN)
    my_board.digital_write(LED_PIN, 0)

    while True:
        try:
            # Optional: poll every few seconds and log button state
            value, ts = my_board.digital_read(pin)
            dt = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(ts))
            print(f"[{dt}] Polling: Button value is {'PRESSED' if value == 0 else 'RELEASED'}")
            time.sleep(POLL_TIME)
        except KeyboardInterrupt:
            print("Exiting...")
            my_board.shutdown()
            sys.exit(0)

# Connect to board on COM4
board = pymata4.Pymata4(com_port="COM4")

try:
    monitor_button(board, BUTTON_PIN)
except KeyboardInterrupt:
    board.shutdown()
    sys.exit(0)


