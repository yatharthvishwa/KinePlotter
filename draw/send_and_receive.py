import serial, time
from pathlib import Path

from line_draw import get_coords_path

PORT = "COM3"        
BAUD = 9600        
DELAY_BETWEEN = 0.01  # seconds
READ_WAIT = 0.5      


coords = get_coords_path()
with serial.Serial(PORT, BAUD, timeout=1) as ser:
    time.sleep(2)  # allow Arduino to reset
    for x, y in coords:
        line = f"{x:.2f},{y:.2f}\n"
        ser.write(line.encode())
        print("SENT:", line.strip())
         
        t0 = time.time()
        while time.time() - t0 < READ_WAIT:  
            resp = ser.readline().decode(errors='ignore').strip()
            if resp:
                print("ARDUINO:", resp)
        time.sleep(DELAY_BETWEEN)
        
    # keep reading until you Ctrl-C or exit
    print("Done sending; entering read loop. Ctrl-C to exit.")
    try:
        while True:
            resp = ser.readline().decode(errors='ignore').strip()
            if resp:
                print("ARDUINO:", resp)
    except KeyboardInterrupt:
        print("Exiting.")