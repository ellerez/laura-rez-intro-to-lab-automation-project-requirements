import asyncio
import threading
import PySimpleGUI as sg
from pymata_express import PyMataExpress

# Pin configuration
BUTTON_PIN = 2
LED_PIN = 13

# Default timer delay (ms)
timer_delay = 2000

# Global board reference
board = None

# Flag to help update GUI safely from timer
gui_queue = []

# ---------------
# Turn off LED
# ---------------
def turn_off_led():
    asyncio.run(board.digital_write(LED_PIN, 0))
    gui_queue.append("LED turned OFF (timer expired)")
    gui_queue.append("Button State: Released")

# ---------------
# Button handler
# ---------------
async def button_handler(data):
    pin, value = data[1], data[2]

    if value == 0:  # Button pressed (active LOW)
        gui_queue.append("Button Pressed → LED ON")
        await board.digital_write(LED_PIN, 1)
        gui_queue.append("Button State: Pressed")
        threading.Timer(timer_delay / 1000.0, turn_off_led).start()

# ---------------
# Async setup
# ---------------
async def board_setup():
    global board
    board = PyMataExpress(com_port="COM4")
    await board.start()
    await board.set_pin_mode_digital_input_pullup(BUTTON_PIN, callback=button_handler)
    await board.set_pin_mode_digital_output(LED_PIN)

# ---------------
# GUI Layout
# ---------------
layout = [
    [sg.Text("Enter LED ON duration (ms):"), sg.Input(default_text=str(timer_delay), size=(10, 1), key='-TIME-'), sg.Button("Set Timer")],
    [sg.Text("Button State: ", size=(40, 1), key='-STATE-')],
    [sg.Text("Log:")],
    [sg.Multiline(size=(60, 10), key='-LOG-', autoscroll=True, disabled=True)],
    [sg.Button("Exit")]
]

window = sg.Window("Arduino Button Timer (FirmataExpress)", layout, finalize=True)

# ---------------
# Start asyncio board in background
# ---------------
asyncio.run(board_setup())

# ---------------
# Main GUI loop
# ---------------
while True:
    event, values = window.read(timeout=100)

    if event in (sg.WIN_CLOSED, "Exit"):
        break

    if event == "Set Timer":
        try:
            timer_delay = int(values['-TIME-'])
            gui_queue.append(f"New timer delay set: {timer_delay} ms")
        except ValueError:
            gui_queue.append("⚠️ Invalid input for timer!")

    # Handle queued messages
    while gui_queue:
        msg = gui_queue.pop(0)
        if msg.startswith("Button State:"):
            window['-STATE-'].update(msg)
        else:
            window['-LOG-'].update(msg + "\n", append=True)

# Cleanup
window.close()
asyncio.run(board.shutdown())

