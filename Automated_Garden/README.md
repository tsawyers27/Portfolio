# ProblemStatement
My wife started a home garden on our back patio in 2026. The small raised garden bed contains cucumbers, two types of tomatoes, thyme, basil, and some flowers to encourage pollenation from bees. However, as it tends to do, life started to get in the way and my wife wasn't able to maintain a consistent watering regimen for the plants. After we discussed it for a few weeks, she agreed to let me try and introduce a small amount of automation and alerting for her. Therefore, this project was built in VS Code using the ESP-IDF extension and uses a microcontroller with a waterproof capacitive soil moisture sensor to notify my wife when the garden is starting to dry out and needs to be watered.

## Hardware
- ESP32 S3 devkit
- DFRobot Gravity IP65 Waterproof Capacitive Soil Moisture Sensor

## Programming Languages
C++

## Skills Showcased
- Embedded systems programming in C++ using ESP-IDF
- IoT device architecture and Wi-Fi connectivity
- Event-driven programming with ESP-IDF event handlers
- FreeRTOS task creation and resource management
- HTTP POST requests and REST API integration
- Secure HTTPS communication using TLS certificate bundles
- Push notification integration through ntfy
- Connection retry logic, error handling, and embedded debugging
- Secure handling of network credentials and service configuration
- Analog sensor interfacing and ADC configuration
- Sensor calibration and normalization
- Signal filtering and noise reduction
- Threshold detection and alert logic
- Time-series sensor data processing

## Planned Expansions and Features
- Add a smart irrigation valve controlled by the ESP32 to automatically start and stop water flow based on real-time soil-moisture readings.
- Implement closed-loop watering logic that continues irrigation until the soil reaches a calibrated target moisture range.
- Add safety controls such as maximum watering duration, cooldown periods, leak detection, and manual override.

## Project Status
Ongoing

## How to use
1. Clone the repo to your local using "git clone" in the terminal
2. Update the wifi network and password in wifi.cpp with your info
3. Update the ntfy topic in notification.cpp with your ntfy topic after installing ntfy on your phone and creating a topic
4. Use ESP-IDF to build the project locally
5. Use EPS-IDF to flash the project onto the ESP32 over UART
6. Wire the sensor into the ESP32 (bottom left three pins next UART plug) using a breadboard
7. Install the breadboard into a waterproof junction box of your choosing
8. Install the junction box within reach of your garden
9. Plug in the ESP32 and check your ntfy app periodically for notifications.