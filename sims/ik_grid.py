import numpy as np
import matplotlib.pyplot as plt

L1 = 10
L2 = 10

theta1 = np.radians(np.linspace(0,180,300))
theta2 = np.radians(np.linspace(0,180,300))

xs = []
ys = []

for t1 in theta1:
    for t2 in theta2:
        x = L1*np.cos(t1) + L2*np.cos(t1+t2)
        y = L1*np.sin(t1) + L2*np.sin(t1+t2)
        xs.append(x)
        ys.append(y)

plt.figure(figsize=(6,6))
plt.scatter(xs, ys, s=1)
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Reachable Workspace of 2-DOF Arm")
plt.axis("equal")
plt.show()
