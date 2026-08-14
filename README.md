# Voice Controlled Wheelchair

An affordable smart-wheelchair prototype that combines **voice-based movement control, Bluetooth communication, ultrasonic obstacle detection, GPS tracking, SOS emergency alerts, and an ESP8266 web dashboard**.

## Project overview

The system uses an **Arduino Uno R3** as the main controller. Voice commands are sent from an Android smartphone through an **HC-05 Bluetooth module**. The Arduino interprets the commands and controls the wheelchair motors through an **L298N motor driver**.

For safety, an **HC-SR04 ultrasonic sensor** monitors the path and can stop the motors when an obstacle is detected. An **ESP8266** with a **NEO-6M GPS module** provides location monitoring, while an SOS push button triggers an emergency state and buzzer. The ESP8266 hosts a lightweight web dashboard for status and GPS information.

## Features

- Voice commands for forward, backward, left, right, and stop
- HC-05 Bluetooth communication
- Arduino Uno based motor control
- Ultrasonic obstacle detection with safety stop
- ESP8266 Wi-Fi web dashboard
- NEO-6M GPS location tracking
- SOS emergency button and buzzer
- Browser-based map view using GPS coordinates
- Event history and connection status on the dashboard

## Hardware

| Component | Purpose |
|---|---|
| Arduino Uno R3 | Main controller |
| HC-05 Bluetooth | Receives commands from smartphone |
| HC-SR04 ultrasonic sensor | Obstacle detection |
| L298N motor driver | Drives DC motors |
| 4 × 12V geared DC motors | Wheelchair movement |
| ESP8266 | Wi-Fi and web dashboard |
| NEO-6M GPS | Location tracking |
| SOS push button | Emergency trigger |
| Buzzer | Audible emergency indication |
| 12V 1200mAh Li-ion battery | Power source |
| 12V-to-5V regulator | Regulated supply |

## Software

- Arduino IDE
- Arduino C/C++
- ESP8266WiFi
- SoftwareSerial
- TinyGPS++
- HTML/CSS/JavaScript embedded in the ESP8266 sketch

## Command mapping

| Command | Character | Action |
|---|---:|---|
| Forward | `F` | Move forward |
| Backward | `B` | Move backward briefly, then stop |
| Left | `L` | Turn left briefly, then stop |
| Right | `R` | Turn right briefly, then stop |
| Stop | `S` | Stop motors |

## Repository structure

```text
voice-controlled-wheelchair/
├── Arduino/
│   └── wheelchair_control.ino
├── ESP8266/
│   └── wheelchair_dashboard.ino
├── Documentation/
│   └── README_NOTES.md
├── .gitignore
├── LICENSE
└── README.md
```

## Setup

### 1. Arduino controller

1. Open `Arduino/wheelchair_control.ino` in Arduino IDE.
2. Select the Arduino Uno board and the correct serial/USB port.
3. Connect the HC-05, ultrasonic sensor and L298N according to your project circuit.
4. Upload the sketch.
5. Configure the smartphone voice-control application to send the command characters shown above.

### 2. ESP8266 dashboard

1. Open `ESP8266/wheelchair_dashboard.ino` in Arduino IDE.
2. Install the required ESP8266 board package and libraries.
3. Replace:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

with your own Wi-Fi credentials. **Do not commit real credentials to GitHub.**
4. Connect the NEO-6M GPS, SOS button and buzzer according to the project circuit.
5. Upload the sketch to the ESP8266.
6. Open the IP address printed in the Serial Monitor in a browser on the same Wi-Fi network.

## Safety note

This repository represents an **academic prototype**, not a medically certified or production-ready mobility device. Test with the wheels safely elevated or in a controlled environment before any human use. Verify motor polarity, power wiring, emergency-stop behavior, obstacle threshold and wireless behavior for your specific hardware.

## Testing covered in the project

The project report describes testing of wheelchair movement, ultrasonic obstacle detection, SOS emergency behavior, GPS location updates and the web dashboard. The reported obstacle test uses a predefined safety distance and stops the motors when an obstacle is detected.

## Limitations

- Voice recognition can be affected by background noise or unclear pronunciation.
- Bluetooth/Wi-Fi operation has a limited communication range.
- Ultrasonic sensing may not reliably detect every small, sharp or transparent object.
- The prototype does not provide fully autonomous navigation.
- Additional advanced features would increase system complexity and cost.

## Future scope

Possible extensions documented for the project include improved AI-based voice recognition, multi-sensor/LiDAR obstacle detection, semi-autonomous navigation, health monitoring, route mapping, geofencing and richer activity logs.

## Academic project

**Project:** Voice Controlled Wheelchair – An Assistive Technology for Improved Mobility  
**Department:** Electronics & Communication Engineering  
**Institution:** Bapuji Institute of Engineering and Technology, Davangere  
**Academic year:** 2025–2026

## Credits

This repository is prepared from the project source/report supplied for the academic project. Personal contact details and Wi-Fi credentials from the original report have intentionally not been included in this public repository package.
