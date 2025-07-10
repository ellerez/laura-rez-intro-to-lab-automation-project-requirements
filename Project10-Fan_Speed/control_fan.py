import FreeSimpleGUI as sg
import serial
import threading
import csv
import time
from datetime import datetime

# -------------------
# Configuration
# -------------------
COM_PORT = 'COM4'
BAUD_RATE = 9600
CSV_FILENAME = "fan_speed_log.csv"

# -------------------
# Setup Serial
# -------------------
try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Allow Arduino to reset
except serial.SerialException:
    sg.popup_error(f"Could not open serial port {COM_PORT}")
    raise SystemExit

# -------------------
# Create CSV file with headers if not exists
# -------------------
with open(CSV_FILENAME, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(["Timestamp", "PWM", "Estimated RPM"])

# -------------------
# GUI Layout
# -------------------
layout = [
    [sg.Text("Fan Speed Monitor", font=('Any', 16), justification='center')],
    [sg.Text("PWM Value:"), sg.Text("", key='-PWM-')],
    [sg.Text("Estimated RPM:"), sg.Text("", key='-RPM-')],
    [sg.Multiline("", size=(50, 10), key='-LOG-', autoscroll=True, disabled=True)],
    [sg.Button("Exit")]
]

window = sg.Window("Fan Speed Logger", layout, finalize=True)

# -------------------
# Serial Reading Thread
# -------------------
def read_serial():
    while True:
        if ser.in_waiting:
            line = ser.readline().decode(errors='ignore').strip()
            if line.startswith("PWM:"):
                # Example line: PWM: 128 | Estimated RPM: 3500
                parts = line.replace("PWM:", "").split("|")
                try:
                    pwm = int(parts[0].strip())
                    rpm = int(parts[1].replace("Estimated RPM:", "").strip())
                except (IndexError, ValueError):
                    continue

                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                window.write_event_value('-DATA-', (timestamp, pwm, rpm))

threading.Thread(target=read_serial, daemon=True).start()

# -------------------
# Main GUI Loop
# -------------------
while True:
    event, values = window.read()

    if event == sg.WIN_CLOSED or event == "Exit":
        break

    elif event == '-DATA-':
        timestamp, pwm, rpm = values[event]
        window['-PWM-'].update(pwm)
        window['-RPM-'].update(rpm)

        log_line = f"[{timestamp}] PWM: {pwm} | RPM: {rpm}\n"
        window['-LOG-'].update(log_line, append=True)

        # Append to CSV
        with open(CSV_FILENAME, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([timestamp, pwm, rpm])

window.close()
ser.close()
