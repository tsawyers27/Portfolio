from rosbags.highlevel import AnyReader
import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt

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

# Create array of Cartesian coordinates
x = ranges * np.cos(angles)
y = ranges * np.sin(angles)

plt.figure(figsize=(8, 6))
plt.scatter(x, y , c='blue', alpha=0.7)
plt.xlabel('X Coord')
plt.ylabel('Y Coord')
plt.title('Point Cloud')
plt.grid(True)
plt.show()