import numpy as np
import matplotlib.pyplot as plt

L1 = 10
L2 = 10

theta1 = np.radians(np.linspace(0,180,900))
theta2 = np.radians(np.linspace(0,-180,300)) # thetha expected for formula = - actual servo angle

xs = []
ys = []

for t1 in theta1:
    for t2 in theta2:
        x = L1*np.cos(t1) + L2*np.cos(t1+t2)
        y = L1*np.sin(t1) + L2*np.sin(t1+t2)
        xs.append(x)
        ys.append(y)


plt.figure(figsize=(6,6))
plt.scatter(xs, ys, s=0.01)

# draw X and Y axes at 0
plt.axhline(0, color='k', linewidth=0.8)      # x-axis (y=0)
plt.axvline(0, color='k', linewidth=0.8)      # y-axis (x=0)

# grid: major + minor ticks with light dashed lines
plt.minorticks_on()
plt.grid(which='major', linestyle='-', linewidth=0.6, color='gray', alpha=0.6)
plt.grid(which='minor', linestyle='--', linewidth=0.4, color='gray', alpha=0.3)

# optional: set ticks every 2 units (adjust as needed)
import numpy as np
R = L1 + L2
plt.xticks(np.arange(-R, R+1, 2))
plt.yticks(np.arange(-R, R+1, 2))

plt.xlabel("X")
plt.ylabel("Y")
plt.title("Reachable Workspace of KinePlotter")
plt.axis("equal")
plt.show()
