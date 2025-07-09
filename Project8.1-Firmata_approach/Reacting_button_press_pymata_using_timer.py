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


