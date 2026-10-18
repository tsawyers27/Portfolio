from rosbags.highlevel import AnyReader
import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt
from sklearn.cluster import DBSCAN

np.set_printoptions(threshold=np.inf)

# Get a single, complete LiDAR scan reading from ROS2 bag
def read_mcap():
    file_path = Path(input("Enter a LiDAR scan file name: "))

    with AnyReader([file_path]) as reader:
        connections = [x for x in reader.connections if x.topic == '/scan']

        for connection, timestamp, rawdata in reader.messages(connections=connections):
            msg = reader.deserialize(rawdata, connection.msgtype)
            data = np.frombuffer(msg.ranges, dtype=np.float32)
            range_max = msg.range_max
            range_min = msg.range_min
            angle_min = msg.angle_min
            angle_inc = msg.angle_increment
            break
    return data, range_max, range_min, angle_min, angle_inc

# Create data array and get angle data from LiDAR reading
ranges, range_max, range_min, angle_min, angle_inc = read_mcap()

# Make NumPy array writeable
ranges = ranges.copy()

# Create corresponding Numpy array of angles for Cartesian coordinate calculations
indices = np.arange(len(ranges))
angles = angle_min + indices * angle_inc

# Match array dtype
angles = angles.astype(np.float32)

# Remove invalid values using boolean mask
mask = (np.isfinite(ranges) & (ranges >= range_min) & (ranges < range_max))
ranges = ranges[mask]
angles = angles[mask]

# Create array of Cartesian coordinates using matrix operations
x = ranges * np.cos(angles)
y = ranges * np.sin(angles)
cartesian = np.column_stack((x, y))

# Initial clustering test
dbscan = DBSCAN(eps=0.5, min_samples=2)
dbscan.fit(cartesian)
labels = dbscan.labels_

# Initial test plot
unique = set(labels)
colors = [plt.cm.Spectral(each) for each in np.linspace(0, 1, len(unique))]

for k, col in zip(unique, colors):
    if k == -1:
        col = [0, 0, 0, 1] # Black for noise
    class_member_mask = (labels == k)
    plt.plot(cartesian[class_member_mask, 0], cartesian[class_member_mask, 1], 'o',
             markerfacecolor=tuple(col), markeredgecolor='k', markersize=6)

plt.title("Clustering")
plt.grid(True)
plt.axis('equal')
plt.show()