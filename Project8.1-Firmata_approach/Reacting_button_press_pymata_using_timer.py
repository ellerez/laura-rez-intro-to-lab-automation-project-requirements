import time
import sys
import threading
import FreeSimpleGUI as sg
from pymata4 import pymata4

# ----------- Constants ----------- #
BUTTON_PIN = 6         # Grove Button on D6
LED_PIN = 13           # Built-in LED
DEFAULT_DURATION_MS = 5000
COM_PORT = 'COM4'

# ----------- Globals ----------- #
led_timer = None
led_on_duration = DEFAULT_DURATION_MS

# ----------- Helper Function ----------- #
def current_time():
    return time.strftime("%Y-%m-%d %H:%M:%S")

# ----------- Callback ----------- #
def button_callback(data):
    global led_timer
    pin = data[1]
    value = data[2]

    if value == 1:
        window.write_event_value("-BUTTON-", "Pressed")

        board.digital_write(LED_PIN, 1)  # Turn LED on
        timestamp = current_time()
        window.write_event_value("-LED_ON-", f"LED turned ON [{timestamp}]")

        # Cancel existing timer if active
        if led_timer and led_timer.is_alive():
            led_timer.cancel()

        # Start a new timer to turn off LED
        led_timer = threading.Timer(led_on_duration / 1000, turn_off_led)
        led_timer.start()
    else:
        window.write_event_value("-BUTTON-", "Released")

# ----------- Turn off LED ----------- #
def turn_off_led():
    board.digital_write(LED_PIN, 0)
    timestamp = current_time()
    window.write_event_value("-LED_OFF-", f"LED turned OFF [{timestamp}]")

# ----------- Setup Arduino ----------- #
board = pymata4.Pymata4(com_port=COM_PORT)

board.set_pin_mode_digital_input(BUTTON_PIN, callback=button_callback)
board.set_pin_mode_digital_output(LED_PIN)
board.digital_write(LED_PIN, 0)

# ----------- GUI Setup ----------- #
layout = [
    [sg.Text("LED On-Time (ms):"), sg.Input(default_text=str(DEFAULT_DURATION_MS), key="-DURATION-", size=(10, 1)), sg.Button("Set")],
    [sg.Text("Button State:"), sg.Text("Released", key="-STATE-", size=(10, 1))],
    [sg.Text("LED State:"), sg.Text("Off", key="-LEDSTATE-", size=(10, 1))],
    [sg.Multiline(key="-LOG-", size=(50, 10), disabled=True)],
    [sg.Button("Exit")]
]

window = sg.Window("Arduino Button + LED Controller", layout)

# ----------- Event Loop ----------- #
try:
    while True:
        event, values = window.read(timeout=100)

        if event in (sg.WIN_CLOSED, "Exit"):
            break

        elif event == "Set":
            try:
                led_on_duration = int(values["-DURATION-"])
                window["-LOG-"].print(f"✅ LED duration set to {led_on_duration} ms")
            except ValueError:
                window["-LOG-"].print("❌ Invalid number! Please enter an integer.")

        elif event == "-BUTTON-":
            state = values[event]
            window["-STATE-"].update(state)
            window["-LOG-"].print(f"🔘 Button {state}")

        elif event == "-LED_ON-":
            window["-LEDSTATE-"].update("On")
            window["-LOG-"].print(f"💡 {values[event]}")

        elif event == "-LED_OFF-":
            window["-LEDSTATE-"].update("Off")
            window["-LOG-"].print(f"💡 {values[event]}")

except Exception as e:
    print(f"Error: {e}")

finally:
    board.shutdown()
    window.close()



# import time
# from threading import Timer
# from pymata4 import pymata4
# import FreeSimpleGUI as sg

# # Setup
# BUTTON_PIN = 6       # Grove button is connected to D6
# LED_PIN = 13         # Built-in LED
# LED_ON_DURATION = 2000  # time in ms

# # Callback data indices
# CB_PIN = 1
# CB_VALUE = 2

# # Global variables
# board = pymata4.Pymata4(com_port="COM4")
# led_timer = None

# def turn_off_led():
#     board.digital_write(LED_PIN, 0)
#     print("LED turned OFF")

# def button_callback(data):
#     global led_timer
#     pin = data[CB_PIN]
#     value = data[CB_VALUE]

#     # Button is pressed (pull-up config means LOW = pressed)
#     if value == 0:
#         print("Button PRESSED")
#         board.digital_write(LED_PIN, 1)
#         print("LED turned ON")

#         # Cancel existing timer if running
#         if led_timer is not None:
#             led_timer.cancel()

#         # Start timer to turn off the LED
#         led_timer = Timer(LED_ON_DURATION/1000, turn_off_led)
#         led_timer.start()

# def setup():
#     board.set_pin_mode_digital_input_pullup(BUTTON_PIN, callback=button_callback)
#     board.set_pin_mode_digital_output(LED_PIN)

# try:
#     setup()
#     print("Ready. Press the button to turn on LED.")
#     while True:
#         time.sleep(1)

# except KeyboardInterrupt:
#     print("Shutting down...")
#     board.shutdown()



# import time
# import sys
# from pymata4 import pymata4

# print('start')
# # Grove Kit Setup
# BUTTON_PIN = 6       # Button connected to D6
# LED_PIN = 13         # Built-in LED
# POLL_TIME = 1        # Polling interval in seconds

# # Callback data indices
# CB_PIN_MODE = 0
# CB_PIN = 1
# CB_VALUE = 2
# CB_TIME = 3

# def the_callback(data):
#     """
#     A callback function to respond to button changes.
#     It prints button state and toggles LED accordingly.
#     """
#     value = data[CB_VALUE]
#     pin = data[CB_PIN]
#     timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(data[CB_TIME]))
    
#     if value == 1:
#         print(f"[{timestamp}] Button PRESSED on pin {pin}")
#         board.digital_write(LED_PIN, 1)
#     else:
#         print(f"[{timestamp}] Button RELEASED on pin {pin}")
#         board.digital_write(LED_PIN, 0)

# def monitor_button(my_board, pin):
#     # Set button as digital input with pull-up resistor
#     my_board.set_pin_mode_digital_input_pullup(pin, callback=the_callback)
    
#     # Set LED pin as output and ensure it's off
#     my_board.set_pin_mode_digital_output(LED_PIN)
#     my_board.digital_write(LED_PIN, 0)

#     while True:
#         try:
#             # Optional: poll every few seconds and log button state
#             value, ts = my_board.digital_read(pin)
#             dt = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(ts))
#             print(f"[{dt}] Polling: Button value is {'PRESSED' if value == 1 else 'RELEASED'}")

#             time.sleep(POLL_TIME)
#         except KeyboardInterrupt:
#             print("Exiting...")
#             my_board.shutdown()
#             sys.exit(0)

# # Connect to board on COM4
# board = pymata4.Pymata4(com_port="COM4")

# try:
#     monitor_button(board, BUTTON_PIN)
# except KeyboardInterrupt:
#     board.shutdown()
#     sys.exit(0)


