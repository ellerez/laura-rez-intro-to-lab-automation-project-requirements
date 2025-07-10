import FreeSimpleGUI as sg
import serial, threading, time, csv
from datetime import datetime

COM_PORT = 'COM4'
BAUD_RATE = 9600
CSV_FILE = 'servo_buzzer_log.csv'

# Initialize serial communication
try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.1)
    time.sleep(2)
except serial.SerialException:
    sg.popup_error(f"Cannot open {COM_PORT}")
    raise SystemExit

# Prepare CSV
with open(CSV_FILE, 'w', newline='') as f:
    csv.writer(f).writerow(['Timestamp', 'Angle', 'Buzzer'])

# GUI Layout
layout = [
    [sg.Text("Servo Angle"), sg.Text("", size=(5,1), key='-ANGLE-')],
    [sg.Canvas(key='-CANVAS-', size=(240,240))],
    [sg.Text("Buzzer State:"), sg.Text("", size=(6,1), key='-BUZZER-'),
     sg.T('  '), sg.Graph((20,20), (0,0), (20,20), key='-LED-')],
    [sg.Button("Exit")]
]
window = sg.Window("Servo & Buzzer Monitor", layout, finalize=True)

canvas = window['-CANVAS-'].TKCanvas
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
fig, ax = plt.subplots(figsize=(3,3), subplot_kw={'projection':'polar'})
plt.close(fig)
dial = ax.bar([0], [1], width=0.05, bottom=0.5)[0]
ax.set_ylim(0,1)
ax.set_yticks([])
ax.set_xticks([i*45*3.1416/180 for i in range(8)])
dial_canvas = FigureCanvasTkAgg(fig, canvas)
dial_canvas.get_tk_widget().pack()

# Serial-reading thread
def read_loop():
    buf = ''
    while True:
        data = ser.read(ser.in_waiting or 1).decode(errors='ignore')
        if not data:
            time.sleep(0.01); continue
        buf += data
        if '\n' in buf:
            line, buf = buf.split('\n',1)
            parts = line.strip().split('|')
            if len(parts)==2 and 'ANGLE:' in parts[0] and 'BUZZER:' in parts[1]:
                try:
                    angle = int(parts[0].split(':')[1].strip())
                    buz = parts[1].split(':')[1].strip()
                    ts = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                    window.write_event_value('-DATA-', (angle, buz, ts))
                except:
                    pass

threading.Thread(target=read_loop, daemon=True).start()

# Main event loop
while True:
    event, values = window.read(timeout=100)
    if event == sg.WIN_CLOSED or event == 'Exit':
        break
    if event == '-DATA-':
        angle, buz, ts = values[event]
        window['-ANGLE-'].update(str(angle))
        # Update dial
        dial.set_height(1)
        dial.set_x(angle * 3.1416 / 180)
        dial_canvas.draw()
        # Update buzzer LED
        window['-BUZZER-'].update(buz)
        color = 'green' if buz=='ON' else 'grey'
        window['-LED-'].draw_circle((10,10), 8, fill_color=color)
        # Log CSV
        with open(CSV_FILE, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([ts, angle, buz])

# Cleanup
ser.close()
window.close()
