from rosbags.highlevel import AnyReader
import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt
from sklearn.cluster import DBSCAN
from sklearn.metrics import pairwise_distances

# Get a single, complete LiDAR scan reading from ROS2 bag
def read_mcap():
    # List directory with MCAP/ROS bag files
    dir_list = sorted(Path("lidar_scans/").rglob("*.mcap"))
    print("Directories containing MCAP files:")
    dir_dict = {}
    for i, dir in enumerate(dir_list):
        j = i+1
        dir_dict[j] = dir
        print(f"{j}: {dir}")

    # Select file
    dir_in = input("Enter the directory number of your choice: ")
    file_path = dir_dict[int(dir_in)]

    # Read a single /scan message from the selected MCAP file to generate
    # numpy array calculation metrics
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

# Plot data as scatter plot
def plot_data(unique, colors, labels, cartesian):
    for k, col in zip(unique, colors):
        if k == -1:
            col = [0, 0, 0, 1] # Black for noise
        class_member_mask = (labels == k)
        plt.scatter(cartesian[class_member_mask, 0], cartesian[class_member_mask, 1], c=col, marker='o')

    plt.title("Obstacle Detection")
    plt.grid(True)
    plt.axis('equal')
    plt.show()

# Split data into clusters
def split_clusters(unique, labels, cartesian):
    clusters = []
    for cluster in unique:
        if cluster == -1:
            continue
        cluster_mask = (labels == cluster)
        cluster_arr = cartesian[cluster_mask]
        clusters.append(cluster_arr)
    return clusters

def print_obstacle_data(clusters):
    print(f"Detected Obstacles: {len(clusters)}")
    
    for i in range(len(clusters)):
        print(f"Obstacle {i+1}: ")
        # Calculate and print centroid coordinate values
        center_x = np.mean(clusters[i][:,0])
        center_y = np.mean(clusters[i][:,1])
        print(f"Centroid: ({center_x:.3f}, {center_y:.3f})")

        # Calculate squared distances for each point in the obstacle cluster
        distances = np.sum(clusters[i]**2, axis=1)

        # Find the index with the shortest squared distance
        nearest_index = np.argmin(distances)

        # Retrieve and print closest point
        nearest = clusters[i][nearest_index]
        print(f"Nearest Point: ({nearest[0]:.3f}, {nearest[1]:.3f})")

        # Calculate width of obstacle
        pairwise_dist = pairwise_distances(clusters[i])
        cluster_width = np.max(pairwise_dist)
        print(f"Obstacle Width: ~{cluster_width:.3f} m")
        print("")

def main():
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

    # Get inputs for DBSCAN algorithm
    epsilon = input("Enter an epsilon value (0.5 = 50cm): ")
    samples = input("Enter a minimum number of samples to define a cluster (e.g., 2, 3, 4, 5, etc.): ")

    # Perform DBSCAN on Cartesian array
    dbscan = DBSCAN(eps=float(epsilon), min_samples=int(samples))
    dbscan.fit(cartesian)

    # Create arrays of cluster labels and core points for later calculations
    labels = dbscan.labels_

    # Get number of unique labels
    unique = set(labels)
    colors = [plt.cm.Spectral(each) for each in np.linspace(0, 1, len(unique))]

    clusters = split_clusters(unique, labels, cartesian)
    print_obstacle_data(clusters)

    plot_data(unique, colors, labels, cartesian)

if __name__ == "__main__":
    main()