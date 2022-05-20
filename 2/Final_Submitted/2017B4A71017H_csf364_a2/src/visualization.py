import matplotlib.pyplot as plt
import numpy as np

### Plotting the points first ###
fig, ax = plt.subplots()
ax.plot([0, 0], [0, 0])
f = open('..\\input\\in.txt', 'r')
s = f.readlines()
f.close()
x = []
y = []
for i in s:
    tmp = i.split()
    if len(tmp) == 1: continue
    x.append(float(tmp[0]))
    y.append(float(tmp[1]))
ax.scatter(x, y)
ax.set_title('Input Points')
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
plt.savefig('..\\output\\points.png') ### Saving the figure.
plt.tight_layout();

### Plotting the best fit lines ###
fig, ax = plt.subplots()
ax.plot([0, 0], [0, 0])
ax.scatter(x, y)
f = open('..\\output\\out.txt', 'r')
s = f.readlines()
f.close()
for i in s:
    tmp = i.split()
    if len(tmp) == 2: ### The penalty and C value are stored in the first line.
        min_penalty = float(tmp[0])
        C = float(tmp[1])
        continue
    start = float(tmp[2]) ### The starting point of the line segment.
    stop = float(tmp[3]) ### The ending point of the line segment.
    c = float(tmp[1]) ### Y-intercept of the line segment.
    if tmp[0] == 'inf': ### If slope is infinity i.e. line is parallel to Y-axis.
        y = np.linspace(start, stop, 100)
        x = 0 * y + c
    else:
        m = float(tmp[0]) ### Slope of the line segment.
        x = np.linspace(start, stop, 100)
        y = m * x + c
    ax.plot(x, y, linewidth=3)
ax.set_title('Minimum Penalty (C = ' + str(C) + ') is ' + str(min_penalty))
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
plt.savefig('..\\output\\out.png') ### Saving the figure.
plt.show()