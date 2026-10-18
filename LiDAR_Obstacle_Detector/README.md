# LiDAR Obstacle Detector

A Python command-line project that reads **prerecorded ROS 2 LiDAR scans from MCAP files**, converts valid range measurements into a 2D point cloud, and uses **DBSCAN** to group spatially related returns into candidate obstacles. The program reports geometric properties for each cluster and visualizes the clustered scan alongside the original sensor-relative coordinate system.

This project was developed to practice robotics perception, ROS 2 data handling, numerical processing with NumPy, spatial clustering, and visual validation using actual LiDAR recordings. It is a deliberately scoped **single-scan analysis tool**, not a complete obstacle-avoidance or navigation system.

## Features

- Searches a `lidar_scans/` folder and its subdirectories for `.mcap` files and presents a numbered selection menu.
- Reads the **first `/scan` message** from the selected MCAP file using `rosbags.highlevel.AnyReader`.
- Extracts `ranges`, `range_min`, `range_max`, `angle_min`, and `angle_increment` from the ROS 2 `LaserScan` message.
- Removes nonfinite and out-of-range measurements while keeping angles aligned with their corresponding distances.
- Converts polar LiDAR readings into Cartesian `(x, y)` coordinates relative to the LiDAR sensor.
- Groups valid points with DBSCAN using user-supplied `eps` and `min_samples` parameters.
- Excludes DBSCAN noise points (`label == -1`) from reported obstacle clusters.
- Calculates each detected cluster's **centroid**, **nearest observed point to the sensor**, and **maximum pairwise point separation** (approximate cluster diameter).
- Plots points by DBSCAN label, shows noise in black, and uses equal x/y axis scaling.

## Requirements

- Python 3 with compatible versions of:
  - [`rosbags`](https://pypi.org/project/rosbags/)
  - [`numpy`](https://pypi.org/project/numpy/)
  - [`matplotlib`](https://pypi.org/project/matplotlib/)
  - [`scikit-learn`](https://pypi.org/project/scikit-learn/)
- One or more MCAP recordings containing ROS 2 `sensor_msgs/msg/LaserScan` messages on the **`/scan`** topic.
- A graphical environment capable of displaying a Matplotlib window.

The program does **not** need a live ROS 2 node or a running ROS 2 installation to read a supported recording through `rosbags`.

## Project Layout

The following is a suggested arrangement for running the program and storing screenshots:

```text
lidar-obstacle-detector/
├── obstacle_detector.py
├── requirements.txt
├── lidar_scans/
│   ├── recording_1.mcap
│   └── ...
├── images/                     # Optional README screenshots
│   ├── lidar_scan_3_pyplot.png
│   └── lidar_scan_3_RViz.png
└── README.md
```

The recording filenames are illustrative. MCAP recordings can also live in nested subdirectories under `lidar_scans/`. Rename your final script to `obstacle_detector.py` if desired.

## Setup and Execution

From the project directory, create and activate a virtual environment (recommended):

```bash
python3 -m venv .venv
source .venv/bin/activate
```

Install the project dependencies from `requirements.txt`:

```bash
python -m pip install -r requirements.txt
```

The requirements file lists `rosbags`, `numpy`, `matplotlib`, and `scikit-learn` without fixed versions. A compatible version of each package will be resolved at installation time. `pathlib` is part of the Python standard library and requires no separate installation.

Place your `.mcap` recordings under `lidar_scans/`, then run:

```bash
python obstacle_detector.py
```

The program displays a numbered list of matching MCAP **files**. Enter the corresponding number, followed by DBSCAN's parameters:

```text
Directories containing MCAP files:
1: lidar_scans/recording_1.mcap
2: lidar_scans/recording_2.mcap
Enter the directory number of your choice: 1
Enter an epsilon value (0.5 = 50cm): 0.5
Enter a minimum number of samples to define a cluster (e.g., 2, 3, 4, 5, etc.): 2
```

*The directory wording is retained from the current program; the numbered choices actually identify MCAP files.* The search is relative to the **current working directory**. The current file listing is not explicitly sorted, so its order may differ between runs.

After processing the chosen scan, the program prints the detected cluster count and measurements, then opens the scatter plot. It processes the first available `/scan` message in that recording rather than prompting for a message index.

## Processing Pipeline

### 1. Read a ROS 2 LaserScan from MCAP

`AnyReader` opens the selected recording, filters message connections to the `/scan` topic, and deserializes the first matching message. The scan provides a set of distances and the angular metadata needed to determine the direction of each reading.

### 2. Filter and transform measurements

Angles are generated from their reading indices:

```text
theta_i = angle_min + i * angle_increment
```

A shared Boolean mask removes nonfinite distances and readings outside the sensor's reported valid range. Applying **the same mask to the distance and angle arrays** preserves the alignment of each measurement with its bearing.

Valid polar measurements are then transformed into points in the LiDAR frame:

```text
x_i = range_i * cos(theta_i)
y_i = range_i * sin(theta_i)
```

Coordinates remain **sensor-relative**: `(0, 0)` is the LiDAR origin, not the mean of the observed point cloud. This matters when reporting the closest observed point to the sensor.

### 3. Cluster points with DBSCAN

DBSCAN groups points according to local spatial density. Its two interactive parameters are:

- **`eps`**: neighborhood radius, in meters when the input `LaserScan` ranges are measured in meters. For example, `0.5` is 50 cm.
- **`min_samples`**: minimum number of samples within a neighborhood (including the point itself) for a point to qualify as a core point.

The program treats each non-noise DBSCAN cluster as a **candidate obstacle**. The clustering identifies connected groups of returns, not semantic object classes: a wall may form one cluster, and nearby objects may merge into one cluster.

### 4. Report obstacle geometry

For each cluster, the program calculates:

| Measurement | Calculation | Interpretation |
| --- | --- | --- |
| **Centroid** | Mean of the cluster's x- and y-coordinates | Approximate center of the observed returns |
| **Nearest point** | Point minimizing `x² + y²` | Closest observed return to the sensor; printed as `(x, y)`, not as a scalar range |
| **Cluster diameter** | Maximum Euclidean distance over all pairs of cluster points | Longest observed point-to-point separation, printed as approximate “Obstacle Width” |

The nearest-point calculation compares **squared distances**, avoiding unnecessary square roots when only the minimum-distance index is needed. For the diameter, the implementation uses `sklearn.metrics.pairwise_distances` and takes the maximum result.

**Terminology note:** The terminal labels the last measure `Obstacle Width`, but mathematically it is the **observed cluster diameter**, not necessarily an object's physical width. For example, a long wall will have a large diameter even if it is narrow.

### 5. Visualize the clustered scan

The Matplotlib scatter plot assigns colors to the DBSCAN labels, renders noise points in black, enables a grid, and keeps axes equally scaled (`plt.axis('equal')`) to avoid visually distorting the scan geometry. Obstacle measurements are reported in the terminal rather than drawn onto the plot.

## Example Visual Comparison

The following screenshots were captured for the same project workflow: an RViz reference view of the LiDAR scan and the Python clustered visualization.

**Python / Matplotlib: clustered LiDAR returns**

![Matplotlib obstacle-detection scatter plot](images/lidar_scan_3_pyplot.png)

**ROS 2 / RViz: LiDAR scan reference**

![RViz LaserScan reference](images/lidar_scan_3_RViz.png)

The plots have different viewpoints and rendering conventions, so they are intended as a **qualitative geometry comparison**, not a pixel-level equivalence test. Copy the two supplied screenshots into your repository's `images/` directory for these relative links to render on GitHub.

## Design Considerations

- **Recorded data rather than live ROS subscriptions.** Reading MCAP files makes development and repeated testing possible without operating the rover during every run. Accessing data from actual ROS 2 recordings also provides experience with real sensor interfaces.
- **One complete scan per execution.** The program intentionally processes the first `/scan` message in a recording. It does not implement temporal tracking or scan-message selection; neither is required for this project's scope.
- **NumPy for spatial arrays and transformations.** Corresponding range and angle arrays, Boolean masking, vectorized trigonometry, and Cartesian point arrays avoid manually iterating through individual measurements.
- **DBSCAN for spatial grouping.** Density-based clustering supports an unknown number of clusters and provides a noise label. Using scikit-learn was a conscious choice to focus the project on sensor processing and interpreting clustering results rather than implementing DBSCAN from scratch.
- **Preserve the LiDAR coordinate origin.** Subtracting the point cloud mean would leave relative cluster geometry intact but make nearest-point measurements incorrect relative to the sensor.
- **Separate stages into functions.** `read_mcap()`, `split_clusters()`, `print_obstacle_data()`, `plot_data()`, and `main()` organize input, grouping, reporting, visualization, and execution.
- **Simple, inspectable output.** A terminal summary plus color-coded scatter plot is sufficient for the objective without adding object classification, navigation decisions, or a graphical application interface.
- **User-selectable DBSCAN settings.** Allowing `eps` and `min_samples` to change makes their effect on cluster grouping visible without changing the program's implementation.

## Testing and Validation

The application has been exercised with a recorded LiDAR scan, and screenshots of the Python visualization and RViz reference were captured for comparison. The file-selection workflow has also been exercised interactively. These observations support the basic ingestion, transform, clustering, and visualization workflow, but are not a formal quantitative accuracy benchmark.

For a representative final validation, use several MCAP recordings with different arrangements of visible surfaces and obstacles, and check that:

1. The plot's geometry is consistent with the corresponding RViz scan after accounting for viewpoint differences.
2. Nonfinite or out-of-range measurements do not appear in the processed point cloud.
3. Detected clusters are plausible under the selected DBSCAN settings, with noise excluded from the obstacle count.
4. Printed centroids, nearest points, and diameters correspond to the plotted clusters.
5. Adjusting `eps` or `min_samples` changes segmentation in understandable ways.

**Validation scope:** This README does not claim a measured detection accuracy, ground-truth object identification, or completion of a specified number of independent scan tests.

## Known Limitations

- **Candidate clusters, not object recognition:** A cluster represents a group of nearby LiDAR returns; it does not establish that the group is one distinct physical object. Adjacent obstacles can merge and one large surface can split.
- **Single-frame only:** The program reads only the first `/scan` message and does not track objects across time, estimate motion, or execute collision avoidance.
- **Parameter sensitivity:** `eps` and `min_samples` influence the number and geometry of clusters. A fixed neighborhood radius can behave differently for near and far surfaces due to LiDAR's angular sampling.
- **Visible surface geometry only:** The centroid and diameter describe observed returns rather than the complete object. Occluded surfaces cannot be measured from a single scan.
- **Quadratic diameter calculation:** Computing every point-to-point distance within a cluster uses roughly quadratic time and memory in the cluster size; acceptable for this small single-scan project, but not ideal for extremely large point clouds.
- **Limited input error handling:** The current script assumes the selected MCAP is readable, contains a `/scan` message, and yields valid Cartesian points. Missing recordings, invalid menu entries, malformed user-supplied DBSCAN parameters, or an empty valid scan may cause an error rather than a friendly message.
- **Local directory convention:** The program searches recursively under `lidar_scans/` relative to the directory from which the script is run; it does not search arbitrary locations or guarantee a sorted menu.

## Skills Demonstrated

**Python, ROS 2 LaserScan/MCAP data ingestion, NumPy array operations, coordinate transformations, filtering sensor measurements, DBSCAN clustering, geometric feature extraction, Matplotlib visualization, and integrating external libraries into a complete robotics-perception workflow.**
