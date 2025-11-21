from PIL import Image, ImageFilter
import numpy as np
import matplotlib.pyplot as plt

# -------------------------------
# SETTINGS
# -------------------------------

# rectangle limits for final scaled points
X_MIN, X_MAX = 2, 16
Y_MIN, Y_MAX = -4, 10

IMAGE_PATH = r"D:\UNOQ\Inverse kinematics 2DOF\sololevel.jpeg"
EDGE_THRESHOLD = 50


# -------------------------------
# 1. Load & Detect Edges
# -------------------------------

img = Image.open(IMAGE_PATH).convert("L")
edges = img.filter(ImageFilter.FIND_EDGES)
edges = np.array(edges)

# raw edge coordinates (y, x)
points = np.argwhere(edges > EDGE_THRESHOLD)
coords = [(int(x), int(y)) for (y, x) in points]


# -------------------------------
# 2. Normalize coords to 0..1
# -------------------------------

xs = np.array([p[0] for p in coords], dtype=float)
ys = np.array([p[1] for p in coords], dtype=float)

img_w = xs.max() - xs.min()
img_h = ys.max() - ys.min()

# Avoid division by zero
if img_w == 0: img_w = 1
if img_h == 0: img_h = 1

# shift all to start at (0,0)
xs_norm = (xs - xs.min()) / img_w
ys_norm = (ys - ys.min()) / img_h


# -------------------------------
# 3. Scale to your rectangle
# -------------------------------

xs_scaled = xs_norm * (X_MAX - X_MIN) + X_MIN
ys_scaled = ys_norm * (Y_MAX - Y_MIN) + Y_MIN

scaled_coords = list(zip(xs_scaled, ys_scaled))
scaled = [(float(x), float(y)) for x, y in scaled_coords]



# -------------------------------
# 4. Plot the scaled path
# -------------------------------

plt.figure(figsize=(6,6))
plt.scatter(xs_scaled, ys_scaled, s=1)

plt.title("Scaled Coordinates in Rectangle (2,-4) to (16,10)")
plt.axis('equal')
plt.show()


# -------------------------------
# 5. Print some values for checking
# -------------------------------

print("Total points:", len(scaled))
print("Example points:", print(scaled))


import serial
import time

# open serial
ser = serial.Serial("COM3", 115200)   # <= change COM port
time.sleep(2)  # wait for Arduino reset

coords = scaled  # use the scaled coordinates

for (x, y) in coords:
    message = f"{x},{y}\n"
    ser.write(message.encode())
    print(message)
    time.sleep(0.01)

