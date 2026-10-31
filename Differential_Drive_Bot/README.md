# Differential-Drive Robot Simulator

A C++ differential-drive kinematics simulator with scheduled wheel-velocity commands, 2D trajectory visualization using Matplot++, and a custom [Webots](https://cyberbotics.com/) controller for the e-puck mobile robot.

The project has two execution paths:

1. **Standalone simulator:** enter a timestep, simulation duration, initial robot state, and a sequence of timestamped wheel-velocity commands. The application advances an ideal kinematic model and plots its XY trajectory.
2. **Webots integration:** run a preconfigured sequence of wheel commands on a simulated e-puck while advancing the **same C++ kinematic model** to calculate a predicted pose.

The emphasis is on differential-drive kinematics, discrete-time simulation, state management, and C++ integration with a robotics simulator—not navigation, obstacle avoidance, or SLAM.

## Features

- Reusable C++ `Robot` class with position, heading, physical parameters, and left/right wheel velocities
- Translational and rotational velocity calculations for a differential-drive platform
- Forward Euler integration of 2D pose `(x, y, theta)`
- Fixed-timestep simulation with **scheduled** motor commands; wheel velocities persist until changed
- Console prompts for standalone simulation configuration and command entry
- XY trajectory visualization through Matplot++ and Gnuplot
- Separate Webots C++ controller that drives an e-puck using linear-to-angular wheel-speed conversion
- Predicted final pose output from both execution paths

## Project layout

```text
Differential_Drive_Bot/
├── CMakeLists.txt
├── main.cpp                 # Standalone CLI, scheduling, and plotting
├── robot.h                  # Pose2D and Robot class declarations
├── robot.cpp                # Robot kinematics and pose integration
├── controllers/
│   └── diff_drive_controller/
│       ├── diff_drive_controller.cpp
│       └── Makefile          # Webots controller build configuration
├── worlds/
│   └── diff_drive_world.wbt  # Floor and e-puck scene
└── build/                    # Local CMake output (generated)
```

The Webots project may also contain automatically created `libraries/`, `plugins/`, or `protos/` directories.

## Mathematical model

Let:

- `v_r`, `v_l` = right and left **wheel linear velocities** (m/s)
- `L` = wheel-to-wheel separation (track width, m)
- `v` = robot forward velocity (m/s)
- `omega` = robot yaw rate (rad/s)
- `theta` = robot heading (radians)
- `dt` = simulation timestep (s)


The robot's forward velocity and yaw rate are:

$$
v = \frac{v_r + v_l}{2}, \qquad
\omega = \frac{v_r - v_l}{L}
$$

The current pose is advanced using forward Euler integration:

$$
\begin{aligned}
x_{t+dt} &= x_t + v \cos(\theta_t)\,dt \\
y_{t+dt} &= y_t + v \sin(\theta_t)\,dt \\
\theta_{t+dt} &= \theta_t + \omega\,dt
\end{aligned}
$$


The `Robot::transform_pose()` method calculates the next pose, updates the object's stored pose, and returns the new `Pose2D` value.

This is an **ideal kinematic model**. Curved motion is approximated by successive short Euler steps, so numerical accuracy depends on `dt`.

## Requirements

### Standalone application

- C++17-capable compiler (for example, GCC)
- CMake 3.14 or newer
- Git/network access for CMake's initial Matplot++ download
- [Matplot++](https://github.com/alandefreitas/matplotplusplus) **v1.2.1**, fetched automatically by CMake
- [Gnuplot](http://www.gnuplot.info/) for the plotting backend, with a graphical terminal for interactive windows

On Ubuntu-based Linux distributions, including Linux Mint, a typical prerequisite installation is:

```bash
sudo apt update
sudo apt install build-essential cmake git gnuplot-qt
```

The standalone project's CMake file uses `FetchContent` and links the executable with Matplot++ (`matplot`). It also disables Matplot++'s pedantic-warnings build option to accommodate the tested GCC environment.

### Webots application

- [Webots](https://cyberbotics.com/) with C++ controller support (the supplied `.wbt` world declares Webots **R2025a**)
- A C++ compiler and Webots' Makefile-based controller build setup
- The project's shared `robot.h` and `robot.cpp`

The Webots controller **does not use Matplot++**. It uses Webots' own simulation and display environment.

## Build and run: standalone simulator

From the project's root directory:

```bash
cmake -S . -B build
cmake --build build
./build/Differential_Drive_Bot
```

The program requests:

1. Simulation step size `dt`, in seconds
2. Maximum simulated duration, in seconds
3. One or more timestamped commands in the format **`right_velocity left_velocity execution_time`**
4. `Done` when finished entering commands
5. Initial robot parameters in the format **`x y theta wheel_size track_width starting_right_velocity starting_left_velocity`**

For example, a set of scheduled commands could be:

```text
0.05 0.05 0.0
0.02 0.08 1.0
0.08 0.02 2.0
-0.04 0.04 3.0
0.0 0.0 4.0
```

Enter each command when prompted, then enter `0` to continue or `Done` after the last command.

**Units:** positions and track width are in meters; heading is in radians; wheel *linear* velocities are in m/s; command execution times and `dt` are in seconds. In the current implementation, the `wheel_size` constructor field is carried as an object property but is not used by the standalone pose-update equations; keep its meaning consistent with your robot configuration.

During simulation, the application applies all commands due at the current simulated time, advances the pose by one `dt`, and records the resulting XY coordinates. It prints the final pose and opens a Matplot++ graph. A command takes effect on the first timestep whose elapsed simulated time is greater than or equal to its scheduled time.

**Tip:** Use a positive timestep and duration, a nonzero track width, valid numeric inputs, and commands ordered by increasing execution time. The current CLI does not enforce all these constraints.

## Build and run: Webots e-puck controller

The provided world is `worlds/diff_drive_world.wbt`. It contains a floor and an e-puck whose `controller` field is set to `diff_drive_controller`.

1. Open the `.wbt` world in Webots.
2. Ensure the controller files are in `controllers/diff_drive_controller/`.
3. Confirm the controller's `Makefile` compiles **both** `diff_drive_controller.cpp` and the shared root-level `robot.cpp`. For this directory layout, its C++ source setting can be:

   ```makefile
   CXX_SOURCES = diff_drive_controller.cpp ../../robot.cpp
   ```

   Keep the Webots-generated `Makefile.include` directive in the Makefile.
4. Build the controller through Webots' **Build** command (or its controller build workflow).
5. Reset and start the simulation. Observe the e-puck movement and the controller's console output.

The controller includes the model header through `../../robot.h`. Because Webots has its own `webots::Robot` type, it declares the user's kinematic model as `::Robot model` to distinguish the two classes.

### Webots test sequence

The current Webots controller has a built-in, five-second schedule:

| Simulated time (s) | Right wheel (m/s) | Left wheel (m/s) | Intended behavior |
|---:|---:|---:|---|
| 0.0 | 0.05 | 0.05 | Straight |
| 1.0 | 0.02 | 0.08 | Clockwise turn |
| 2.0 | 0.08 | 0.02 | Counterclockwise turn |
| 3.0 | -0.04 | 0.04 | Rotate in place |
| 4.0 | 0.00 | 0.00 | Stop |

The controller constructs the kinematic model with an approximately **4.1 cm wheel diameter** and **0.052 m track width**, and uses a **0.0205 m wheel radius** for the Webots motor command conversion:

\[
\omega_{\mathrm{wheel}} = \frac{v_{\mathrm{wheel}}}{r}
\]

This is necessary because the C++ model stores wheel **linear speeds (m/s)**, while Webots motor `setVelocity()` accepts **angular speeds (rad/s)**.

The controller configures the motors for velocity control by setting each motor's target position to infinity, applies commands at their scheduled simulation times, and advances the kinematic model using Webots' basic timestep. The console's final pose is the **model's predicted pose**, not an independently measured or validated Webots ground-truth pose.

To try another motion sequence, edit the `commands.push(...)` entries in `diff_drive_controller.cpp`, rebuild the controller, and reset the world.

## Architecture

| Component | Responsibility |
|---|---|
| `Pose2D` (`robot.h`) | Groups `x`, `y`, and `theta` |
| `Robot` (`robot.h`, `robot.cpp`) | Stores state and geometry; computes body velocity, yaw rate, and Euler pose updates |
| `main.cpp` | Reads parameters and command schedule, runs the standalone fixed-timestep loop, displays the trajectory |
| `diff_drive_controller.cpp` | Executes a built-in schedule in Webots, converts wheel-speed units, commands e-puck motors, and updates the shared model |
| `CMakeLists.txt` | Builds the standalone executable and fetches/links Matplot++ |
| Webots controller `Makefile` | Builds the Webots executable against the Webots API and shared `robot.cpp` |

The standalone program and Webots controller are **separate executables** that reuse the same kinematic class; they do not invoke one another.

## Scope and limitations

- **Kinematics, not dynamics:** wheel slip, acceleration transients, motor torque, and wheel-ground contact behavior are not modeled in `Robot`.
- **Euler integration:** motion curves are approximated using discrete timesteps, rather than analytical exact-arc integration.
- **Scheduled commands:** the standalone CLI accepts a predefined schedule; it does not provide live keyboard control during motion. The Webots controller currently uses a hardcoded schedule.
- **No pose measurement comparison:** the Webots controller computes a predicted pose but does not automatically measure or compare the physics-engine robot pose.
- **No autonomy stack:** obstacle avoidance, path planning, localization, SLAM, and sensor fusion are intentionally outside this project's scope.
- **Input and timing assumptions:** command times should be ascending, `dt > 0`, and track width nonzero. With fixed timesteps, commands not aligned to a step are applied at the first subsequent step.

## Suggested verification cases

To check the implementation with simple, interpretable inputs:

| Wheel speeds | Expected ideal behavior |
|---|---|
| `v_r == v_l > 0` | Straight forward motion |
| `v_r == -v_l` | Rotation in place, no translation |
| `v_r > v_l` | Counterclockwise curvature |
| `v_r < v_l` | Clockwise curvature |
| Both zero | Stationary pose |

Use the same coordinate convention and timestep when comparing expected poses with numerical results. Note that signed angular velocity is measured in radians per second.

## Technologies and concepts

**C++17 · CMake · Matplot++ · Gnuplot · Webots · differential-drive kinematics · forward Euler integration · discrete-time simulation · scheduled command queues · object-oriented design**

---

This project is part of a broader software engineering and robotics portfolio.
