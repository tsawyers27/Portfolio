# Software Engineering & Robotics Portfolio

Hello, World! This repository collects my independent software projects and hands-on engineering work, with an emphasis on **C++**, **Python**, **robotics**, and **systems-oriented problem solving**.

I'm a technology consultant and Georgia Tech OMSCS student concentrating on computational perception and robotics. I'm building on my professional software and systems experience through projects that take an idea from requirements and design through implementation, testing, and documentation.

My interests include robotics software, autonomous systems, embedded development, and general-purpose software engineering.

## Featured projects

| Project | Technologies | Status | Focus |
| --- | --- | --- | --- |
| **Duplicate File Finder** | Python, pandas, SHA-256 | **Complete** | Recursive filesystem scanning, content-based duplicate detection, reporting, permission-error handling |
| **Grid-Based Robot Path Planner** | C++ | **Complete** | Graph search, BFS, A*, occupancy grids |
| **LiDAR Obstacle Detector** | Python | Planned | 2D LiDAR processing, coordinate transforms, obstacle clustering |
| **Differential-Drive Robot Simulator** | C++ | Planned | Robot kinematics, numerical simulation, trajectory logging |
| **Warehouse Robot Task Scheduler** | Python | Planned | Task assignment, state machines, discrete simulation |

### Duplicate File Finder — Python

A command-line utility that scans a directory and its subdirectories to identify duplicate files **by file contents**, even when filenames differ. It groups matching files by SHA-256 hash, reports their paths and sizes, and estimates potentially reclaimable disk space while retaining one copy per group. Permission errors are reported, and unreadable files are excluded from the duplicate report. The utility does **not** modify or delete files.

**Demonstrates:** filesystem traversal, hashing, error handling, data grouping, and reporting.

## Additional engineering work

These projects provide context for my robotics and embedded-systems experience. 

**Raspberry Pi 5 mobile robotics platform (ongoing).** A ROS 2 Jazzy–based indoor rover project involving a Raspberry Pi 5, 2D LiDAR, camera, IMU, and motor-control hardware. Work has included Linux bring-up, peripheral integration, and embedded motor-control experimentation. The broader SLAM/navigation system remains in development.

**ESP32-S3 soil-monitoring prototype (in development).** Embedded C++ work using ESP-IDF for Wi-Fi connectivity and notification delivery, intended to support garden soil-moisture monitoring. Future plans include sensor-driven alerts and optional watering automation; those extensions should not be interpreted as completed features.

## Technical areas

- **Languages:** Python, C++
- **Robotics:** ROS 2, robot sensing, mobile robot integration, motion and navigation fundamentals
- **Embedded / hardware:** ESP32-S3, Raspberry Pi, microcontrollers, sensors, motor control
- **Development environment:** Linux, Git, command-line tooling
- **Engineering practices:** requirements definition, modular design, debugging, testing, and technical documentation

## How to explore this portfolio

Each completed project is intended to include its own README covering the problem, requirements, setup and execution, design decisions, and known limitations. 

## About me

I work in technology consulting and am pursuing an M.S. in Computer Science through **Georgia Tech's Online Master of Science in Computer Science (OMSCS)** program, with a focus on computational perception and robotics. I'm interested in roles that combine strong software engineering with robotics, automation, and real-world systems.

**Contact:** [LinkedIn profile](https://www.linkedin.com/in/trevor-sawyers-baa282127/) · [Email](mailto:tsawyers3@gatech.edu)
