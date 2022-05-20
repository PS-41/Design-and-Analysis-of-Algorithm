import matplotlib.pyplot as plt
import matplotlib.patches as patches

f = open("..\\output\\result.txt", "r")
s = f.readlines()
measure = int(float(s[0]))
contour = int(float(s[1]))
f.close()

##### Visualising rectangles with contour #####
fig, ax = plt.subplots()
ax.plot([0, 0], [0, 0])
f = open("..\\output\\rectangles.txt", "r")
s = f.readlines()
for i in s:
    x = i.split()
    a = float(x[0])
    b = float(x[2])
    c = float(x[1]) - float(x[0])
    d = float(x[3]) - float(x[2])
    rect = patches.Rectangle((a, b), c, d, linewidth=1, edgecolor='r', facecolor='None')
    ax.add_patch(rect)
f.close()
f = open("..\\output\\contour_edges.txt", "r")
s = f.readlines()
for i in s:
    x = i.split()
    a = float(x[0])
    b = float(x[1])
    c = float(x[2])
    d = float(x[3])
    rect = patches.Rectangle((a, b), c, d, linewidth=2, edgecolor='b', facecolor="none")
    ax.add_patch(rect)
ax.set_title("Rectangles with Contour")
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
# ax.legend(['contour','rectangles'],bbox_to_anchor=(0.85, 0.92),title='Legend')
plt.savefig('..\\output\\rectangles_with_contour.png')
plt.tight_layout()
f.close()
###################################################

##### Visualising the contour of rectangles #####
fig, ax = plt.subplots()
ax.plot([0, 0], [0, 0])
f = open("..\\output\\contour_edges.txt", "r")
s = f.readlines()
for i in s:
    x = i.split()
    a = float(x[0])
    b = float(x[1])
    c = float(x[2])
    d = float(x[3])
    rect = patches.Rectangle((a, b), c, d, linewidth=2, edgecolor='b', facecolor="none")
    ax.add_patch(rect)
ax.set_title("Contour (Perimeter = " + str(contour) + " Units)")
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
plt.savefig('..\\output\\contour.png')
plt.tight_layout()
f.close()
####################################################

##### Visualising the measure of rectangles #####
fig, ax = plt.subplots()
ax.plot([0, 0], [0, 0])
f = open("..\\output\\rectangles.txt", "r")
s = f.readlines()
for i in s:
    x = i.split()
    a = float(x[0])
    b = float(x[2])
    c = float(x[1]) - float(x[0])
    d = float(x[3]) - float(x[2])
    rect = patches.Rectangle((a, b), c, d, linewidth=1, edgecolor='black', facecolor='black')
    ax.add_patch(rect)
f.close()
ax.set_title("Measure (area) = " + str(measure) + " Square Units")
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
plt.savefig('..\\output\\measure.png')
plt.tight_layout()
###################################################

##### Visualising input rectangles #####
fig, ax = plt.subplots()
ax.plot([0, 0], [0, 0])
f = open("..\\output\\rectangles.txt", "r")
s = f.readlines()
for i in s:
    x = i.split()
    a = float(x[0])
    b = float(x[2])
    c = float(x[1]) - float(x[0])
    d = float(x[3]) - float(x[2])
    rect = patches.Rectangle((a, b), c, d, linewidth=1, edgecolor='r', facecolor='None')
    ax.add_patch(rect)
ax.set_title("Rectangles")
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
plt.savefig('..\\output\\rectangles.png')
plt.show()
f.close()
#########################################