from PIL import Image, ImageFilter
import numpy as np
import matplotlib.pyplot as plt
import time
import serial   # enable when sending to Arduino

# -------------------------------
# SETTINGS
# -------------------------------

X_MIN, X_MAX = 2, 16
Y_MIN, Y_MAX = 0, 10

IMAGE_PATH = r"D:\UNOQ\Inverse kinematics 2DOF\sololevel.png"
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
if img_w == 0: img_w = 1
if img_h == 0: img_h = 1

xs_norm = (xs - xs.min()) / img_w
ys_norm = (ys - ys.min()) / img_h


# -------------------------------
# 3. Scale to your rectangle
# -------------------------------

xs_scaled = xs_norm * (X_MAX - X_MIN) + X_MIN
ys_scaled = ys_norm * (Y_MAX - Y_MIN) + Y_MIN

scaled = list(zip(xs_scaled, ys_scaled))

# downsample BEFORE nearest neighbor – FAST!
DOWNSAMPLE_FACTOR = 15
scaled = scaled[::DOWNSAMPLE_FACTOR]



# -------------------------------
# 4. PATH FINDING (Nearest Neighbor)
# -------------------------------

pts = np.array(scaled, dtype=float)
n = len(pts)

visited = np.zeros(n, dtype=bool)
path = []

idx = 0  # start at first point
for _ in range(n):
    visited[idx] = True
    path.append(tuple(pts[idx]))

    # distances to all points
    d = np.linalg.norm(pts - pts[idx], axis=1)
    d[visited] = 1e12  # ignore visited nodes

    idx = np.argmin(d)

coords_path = path  # final ordered drawing path




# -------------------------------
# 5. Plot final path
# -------------------------------

px = [p[0] for p in coords_path]
py = [p[1] for p in coords_path]

plt.figure(figsize=(6,6))
plt.plot(px, py, linewidth=0.5)
plt.scatter(px, py, s=1)
plt.title("Final Path (Nearest Neighbor Drawing)")
plt.axis('equal')
plt.show()



#send to Arduino

# coords_path is already built earlier in the script
def get_coords_path():
    """Return the ordered path as a list of (x,y) float tuples."""
    return coords_path

def send_to_arduino(coords_path, port="COM3", baud=115200, delay=0.01):
    """Send coords to Arduino over serial and return after done."""
    import serial, time
    with serial.Serial(port, baud, timeout=1) as ser:
        time.sleep(2)  # allow Arduino to reset
        for (x, y) in coords_path:
            message = f"{x:.2f},{y:.2f}\\n"
            ser.write(message.encode())
            # Optionally read immediate response (non-blocking)
            time.sleep(delay)

if __name__ == "__main__":
    
    px = [p[0] for p in coords_path]
    py = [p[1] for p in coords_path]
    plt.figure(figsize=(6,6))
    plt.plot(px, py, linewidth=0.5)
    plt.scatter(px, py, s=1)
    plt.title("Final Path (Nearest Neighbor Drawing)")
    plt.axis('equal')
    plt.show()

    
