import FreeSimpleGUI as sg
import serial
import threading
import time

# -------------------------
# SERIAL CONNECTION SETTINGS
# -------------------------
PORT = 'COM4'
BAUDRATE = 9600

# Try to connect to Arduino
try:
    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to reset
except serial.SerialException:
    sg.popup_error(f"Could not open port {PORT}")
    exit()

# -------------------------
# GUI LAYOUT
# -------------------------
layout = [
    [sg.Text("Enter LED on-time (ms):")],
    [sg.Input(key='-INPUT-')],
    [sg.Button("Send"), sg.Button("Exit")],
    [sg.Text("Response from Arduino:")],
    [sg.Multiline(key='-OUTPUT-', size=(50, 10), autoscroll=True, disabled=True)],
]

window = sg.Window("Arduino LED Controller", layout)

# -------------------------
# Thread to continuously read from serial
# -------------------------
def read_serial():
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode(errors='ignore').strip()
            
            .write_event_value('-SERIAL-', line)

threading.Thread(target=read_serial, daemon=True).start()

# -------------------------
# MAIN EVENT LOOP
# -------------------------

while True:
    event, values = window.read()

    if event == sg.WINDOW_CLOSED or event == "Exit":
        break

    elif event == "Send":
        user_input = values['-INPUT-'].strip()
        if user_input.isdigit():
            ser.write((user_input + '\n').encode())
        else:
            window['-OUTPUT-'].update("⚠️ Invalid input. Enter a number.\n", append=True)

    elif event == '-SERIAL-':
        msg = values[event]
        if msg == '1':
            window['-OUTPUT-'].update("🔘 Button pressed and LED is ON\n", append=True)
        elif msg == '0':
            window['-OUTPUT-'].update("💡 LED is OFF\n", append=True)
        elif msg == '2':
            window['-OUTPUT-'].update("🔄 Button released\n", append=True)
        elif msg.startswith("I received:"):
            window['-OUTPUT-'].update(f"✅ {msg}\n", append=True)
        elif "Error" in msg:
            window['-OUTPUT-'].update(f"❌ {msg}\n", append=True)
        else:
            window['-OUTPUT-'].update(f"📨 {msg}\n", append=True)

window.close()
ser.close()
