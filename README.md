# Embedded-Color-Based-Automated-Sorting-System-Using-ESP32
Automated color-based object sorting system using ESP32, TCS34725 color sensor, ultrasonic sensing, servo actuators, and conveyor automation.

# Overview
This project implements an automated color-based sorting system using an ESP32 microcontroller, TCS34725 color sensor, HC-SR04 ultrasonic sensor, servo motors, and a conveyor belt mechanism. The system detects object colors in real-time and automatically directs items into the appropriate sorting path.

# Features
- Real-time object detection using HC-SR04 ultrasonic sensor
- Color recognition using TCS34725 RGB color sensor
- Automated sorting mechanism using SG90 servo motors
- Conveyor belt control with L298N motor driver
- Finite State Machine (FSM) implementation for process control
- Automatic classification of Red, Green, and Other objects

# Hardware Components
| Component | Function |
|------------|------------|
| ESP32 DevKitC V4 | Main controller and decision-making unit |
| TCS34725 Color Sensor | Detects object colors |
| HC-SR04 Ultrasonic Sensor | Detects object presence |
| SG90 Servo Motor (2x) | Controls sorting gates |
| L298N Motor Driver | Controls conveyor motor |
| DC Motor + Conveyor Belt | Moves objects through the system |
| 6V Battery | Powers the DC motor |
| 5V Adapter | Powers ESP32 and sensors |
| Breadboard & Jumper Wires | Circuit connections |

# System Workflow
- HC-SR04 detects an incoming object.
- ESP32 activates the conveyor.
- Object moves toward the color sensor.
- TCS34725 reads RGB values.
- ESP32 classifies the detected color.
- Servo mechanism directs the object:
    - Red → Red bin
    - Green → Green bin
    - Others → Default path
- System returns to idle state.

# Finite State Machine (FSM)
| State | Description |
|---------|-------------|
| IDLE | Waiting for object detection |
| KONVEYOR_MAJU | Conveyor moves object toward sensor |
| DETEKSI_WARNA | Reads and classifies object color |
| SORTIR_MERAH | Sorts red objects |
| SORTIR_HIJAU | Sorts green objects |
| SORTIR_LAIN | Sorts other colors |
| SELESAI | Sorting process completed |

# Color Classification Logic
| Condition | Classification |
|------------|------------|
| R > G and R > B and R > 15 | RED |
| G > R and G > B and G > 15 | GREEN |
| Otherwise | OTHER |

# Project Images
Circuit Diagram

images/circuit-diagram.png

Physical Prototype

images/prototype.jpg

System Block Diagram

images/block-diagram.png

Sorting Process

images/demo.jpg
