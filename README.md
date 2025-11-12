# 🚗 Smart Bio-Sensing Seat Belt System

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc/)
[![Status](https://img.shields.io/badge/Status-Prototype-orange.svg)]()

## 📋 Table of Contents
- [Overview](#overview)
- [Problem Statement](#problem-statement)
- [Key Features](#key-features)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
- [Circuit Diagram](#circuit-diagram)
- [Software Components](#software-components)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Future Scope](#future-scope)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## 🎯 Overview

The Smart Bio-Sensing Seat Belt System is an innovative IoT-based safety solution that integrates multiple biometric sensors and Brain-Computer Interface (BCI) technology into a vehicle seat belt. The system continuously monitors the driver's physical and mental state to prevent road accidents caused by fatigue, unconsciousness, or health emergencies.

This proactive safety system can:
- Monitor vital signs in real-time
- Detect driver drowsiness and consciousness levels
- Alert the driver through multiple channels
- Automatically initiate emergency protocols
- Communicate with smart vehicles for safe emergency stops

## ⚠️ Problem Statement

Road accidents caused by driver fatigue, unconsciousness, or sudden health issues (heart attacks, high blood pressure) are increasing globally. Current automotive safety systems are largely **reactive** - they respond after an accident occurs. There is a critical need for a **proactive solution** that:

✓ Monitors driver health continuously
✓ Detects deteriorating conditions before accidents occur
✓ Takes autonomous safety measures
✓ Alerts emergency services automatically

## ✨ Key Features

### Health Monitoring
- **Heart Rate Monitoring**: Continuous pulse tracking using PPG sensor
- **Blood Pressure Detection**: Non-invasive BP measurement
- **Alcohol Detection**: MQ-3 sensor for breath alcohol analysis

### Neurological Monitoring
- **Brain-Computer Interface (BCI)**: EEG-based consciousness and drowsiness detection
- **Real-time alertness assessment**: Detects microsleep and attention lapses

### Smart Connectivity
- **Bluetooth Integration**: Real-time data transmission to mobile app
- **Wearable Alerts**: Vibration notifications via smartwatch/band
- **Emergency Calling**: Automatic ambulance dispatch with GPS location

### Vehicle Control
- **Smart Vehicle Integration**: Communication via CAN bus/OBD-II
- **Auto-slowdown**: Gradual speed reduction when driver is unresponsive
- **Safe Pull-over**: Autonomous parking to safe location
- **Audible Siren**: Alerts nearby vehicles and pedestrians

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    SMART SEAT BELT MODULE                   │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │ Heart Rate   │  │Blood Pressure│  │   Alcohol    │     │
│  │   Sensor     │  │   Sensor     │  │   Detector   │     │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘     │
│         │                  │                  │              │
│         └──────────────────┼──────────────────┘              │
│                           │                                 │
│  ┌──────────────┐         │         ┌──────────────┐       │
│  │     BCI      │         │         │  Bluetooth   │       │
│  │  EEG Module  ├─────────┼─────────┤    Module    │───────┼──> Mobile App
│  └──────────────┘         │         └──────────────┘       │
│                           │                                 │
│                    ┌──────▼────────┐                        │
│                    │  Arduino/MCU  │                        │
│                    │  Controller   │                        │
│                    └──────┬────────┘                        │
└───────────────────────────┼─────────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
   ┌────▼─────┐      ┌─────▼──────┐     ┌─────▼──────┐
   │Emergency │      │  Vehicle   │     │ Smartwatch │
   │  System  │      │   ECU      │     │   Alert    │
   │(Ambulance│      │(CAN/OBD-II)│     │  System    │
   └──────────┘      └────────────┘     └────────────┘
```

## 🔧 Hardware Components

### Microcontroller
- **Arduino Uno/Mega** or **ESP32** (for Bluetooth/WiFi)
  - Voltage: 5V
  - Clock Speed: 16 MHz (Arduino) / 240 MHz (ESP32)

### Sensors

| Component | Model | Purpose | Interface |
|-----------|-------|---------|----------|
| Heart Rate Sensor | MAX30100/MAX30102 | Pulse & SpO2 monitoring | I2C |
| Blood Pressure Sensor | Analog BP Sensor | Non-invasive BP measurement | Analog |
| Alcohol Sensor | MQ-3 | Breath alcohol detection | Analog |
| EEG Module | OpenBCI / NeuroSky MindWave | Brain wave monitoring | Serial/SPI |
| GPS Module | NEO-6M | Location tracking | UART |

### Communication
- **Bluetooth Module**: HC-05 / HC-06 or built-in (ESP32)
- **GSM Module**: SIM800L/SIM900 (for emergency calls)

### Actuators
- **Buzzer**: 5V active buzzer for audio alerts
- **Vibration Motor**: For wearable alert device
- **Relay Module**: For vehicle control interface

### Power Supply
- 12V car battery with voltage regulator
- 5V/3.3V power supply modules

## 📊 Circuit Diagram

> **Note**: Detailed circuit schematics are available in the `/hardware/schematics/` folder.

### Pin Configuration

```cpp
// Arduino Pin Assignments
#define HEART_RATE_SDA    A4  // I2C Data
#define HEART_RATE_SCL    A5  // I2C Clock
#define BP_SENSOR_PIN     A0  // Analog input
#define ALCOHOL_SENSOR    A1  // Analog input
#define BCI_RX           10  // Serial RX
#define BCI_TX           11  // Serial TX
#define BLUETOOTH_RX      2  // Software Serial RX
#define BLUETOOTH_TX      3  // Software Serial TX
#define GPS_RX            4  // GPS RX
#define GPS_TX            5  // GPS TX
#define BUZZER_PIN        8  // Digital output
#define RELAY_PIN         9  // Vehicle control
```

## 💻 Software Components

### 1. Microcontroller Code (Arduino/C++)
- Sensor data acquisition
- Signal processing and filtering
- Alert threshold detection
- Communication protocol handling
- Emergency system triggering

### 2. Mobile Application
- **Platform**: Android/iOS
- **Framework**: Flutter / React Native (planned)
- **Features**:
  - Real-time vital signs dashboard
  - Historical data visualization
  - Emergency contact management
  - Alert notifications
  - GPS tracking

### 3. Vehicle Integration Module
- CAN bus communication protocol
- OBD-II interface
- Speed control commands
- Autonomous braking signals

## 🚀 Installation

### Prerequisites
- Arduino IDE (version 1.8.19 or later)
- Required Libraries:
  ```
  - Wire.h (I2C communication)
  - MAX30100_PulseOximeter.h
  - SoftwareSerial.h
  - TinyGPS++.h
  - SPI.h
  ```

### Hardware Setup
1. **Connect Sensors**:
   - Follow the pin configuration table
   - Ensure proper power supply (5V/3.3V as required)
   - Use pull-up resistors for I2C lines (4.7kΩ)

2. **Seat Belt Integration**:
   - Mount sensors on seat belt fabric
   - Ensure sensors make contact with driver's body
   - Use flexible PCB for sensor connections

3. **Power Connection**:
   - Connect to vehicle 12V system
   - Use voltage regulators for stable 5V/3.3V

### Software Setup
1. **Clone the repository**:
   ```bash
   git clone https://github.com/Smart-chinnodu/smart-bio-sensing-seatbelt.git
   cd smart-bio-sensing-seatbelt
   ```

2. **Install Arduino Libraries**:
   - Open Arduino IDE
   - Go to Sketch → Include Library → Manage Libraries
   - Install required libraries listed above

3. **Upload Code**:
   ```bash
   # Open src/arduino/main.ino in Arduino IDE
   # Select your board and COM port
   # Click Upload
   ```

4. **Mobile App Setup** (Coming Soon):
   ```bash
   cd mobile-app
   flutter pub get
   flutter run
   ```

## 📱 Usage

### Initial Setup
1. **Power on the system** when vehicle ignition is turned on
2. **Wear the seat belt** ensuring sensors make proper contact
3. **Pair Bluetooth** with your smartphone
4. **Open the mobile app** and complete initial calibration

### Normal Operation
1. System continuously monitors vital signs
2. Green LED indicates normal operation
3. Data is logged and transmitted to app
4. Alerts are triggered based on thresholds:
   - **Yellow Alert**: Mild fatigue detected
   - **Orange Alert**: Significant drowsiness
   - **Red Alert**: Emergency condition

### Emergency Protocol
When critical condition is detected:
1. ⚠️ **Audible alarm** activated
2. 📳 **Wearable vibration** alert sent
3. 📱 **Mobile notification** pushed
4. 🚗 **Vehicle slowdown** initiated
5. 📞 **Emergency call** placed with GPS location
6. 🚨 **Siren** activated to alert nearby traffic

## 📁 Project Structure

```
smart-bio-sensing-seatbelt/
│
├── src/
│   ├── arduino/              # Microcontroller code
│   │   ├── main.ino         # Main Arduino sketch
│   │   ├── sensors.ino      # Sensor handling functions
│   │   ├── communication.ino # Bluetooth/GSM functions
│   │   └── emergency.ino    # Emergency protocols
│   │
│   ├── mobile-app/          # Mobile application (planned)
│   │   ├── lib/
│   │   └── assets/
│   │
│   └── vehicle-interface/   # CAN bus integration (planned)
│
├── hardware/
│   ├── schematics/          # Circuit diagrams (Fritzing/KiCad)
│   ├── pcb/                 # PCB designs
│   └── 3d-models/           # Enclosure designs
│
├── docs/
│   ├── images/              # Project images
│   ├── datasheets/          # Component datasheets
│   ├── research/            # Research papers and references
│   └── user-manual.md       # Detailed user guide
│
├── tests/                   # Test scripts
│
├── README.md
├── LICENSE
└── .gitignore
```

## 🔮 Future Scope

### Short-term Enhancements
- [ ] Machine Learning model for personalized drowsiness detection
- [ ] Cloud-based data analytics platform
- [ ] Integration with more vehicle models
- [ ] iOS app development

### Long-term Vision
- [ ] Commercial partnerships with automotive OEMs
- [ ] Fleet management integration for logistics companies
- [ ] National emergency network connectivity
- [ ] AI-powered predictive health analytics
- [ ] Integration with autonomous vehicle systems

### Research Areas
- Advanced BCI signal processing
- Multi-modal sensor fusion algorithms
- Real-time stress and emotion detection
- Energy harvesting from vehicle vibrations

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Please ensure your code follows the existing style and includes appropriate comments.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📧 Contact

**Project Developer**: Smart Chinnodu  
**Institution**: Dadi Institute of Engineering and Technology  
**Email**: [Your Email]  
**GitHub**: [@Smart-chinnodu](https://github.com/Smart-chinnodu)  

## 🙏 Acknowledgments

- Thanks to the open-source community for Arduino libraries
- Research inspiration from various automotive safety papers
- Guidance from faculty at DIET

---

⭐ If you find this project useful, please consider giving it a star!  
🐛 Found a bug? Open an issue!  
💡 Have suggestions? We'd love to hear them!

**Built with ❤️ for safer roads**
