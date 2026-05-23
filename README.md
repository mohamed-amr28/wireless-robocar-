# 🚗 Smart Robotic Car
 
A smart robotic car system with three operating modes, built with Arduino and controlled via a custom Flutter mobile app over Bluetooth.
 
---

## 📖 About the Project
 
This project is an end-to-end robotic car system that integrates embedded systems, sensor engineering, and mobile app development. The car supports three distinct operating modes and communicates wirelessly with a Flutter-based Android app via a Bluetooth module.
 
---

## ✨ Key Features

* **📱 Wireless Bluetooth Control:** Seamless communication with a custom mobile app for real-time driving and mode switching.

* **Path Memory:** Record a sequence of manual driving inputs and replay them automatically.

## ⚙️ Operating Modes
 
| Mode | Description |
|------|-------------|
| 🕹️ **Manual Mode** | Full directional control (forward, backward, left, right) via the mobile app over Bluetooth |
| 🤖 **Autonomous Mode** | The car navigates independently using ultrasonic sensors for real-time obstacle detection and avoidance |
| 🅿️ **Auto Park Mode** | The car detects a suitable parking space using ultrasonic sensors and executes the parking maneuver automatically |

* **🔋 Battery Monitoring:** Real-time SoC (State of Charge) tracking with low-battery alerts to prevent motor stalling.
* **🏗️ Professional Code Architecture:** Clean separation of concerns using Application (APP) and Hardware Abstraction (HAL) layers.

---

## 🛠️ Hardware Requirements

* 1x Arduino (Uno / Mega / Nano)
* 4x DC Motors with Wheels (4WD Chassis)
* 1x L298N Motor Driver
* 1x HC-05 or HC-06 Bluetooth Module
* 3x HC-SR04 Ultrasonic Sensors (Front, Left, Right)
* 1x 12V Battery Pack (with voltage divider circuit connected to A0 for monitoring)

---
🏗️ System Architecture
[ Flutter App ]
      |
   Bluetooth (HC-05)
      |
[ Arduino ]
      |
   ┌──────────────────────┐
   │  Motor Driver (L298N) │
   │  Ultrasonic Sensors   │
   │  4x DC Motors         │
   └──────────────────────┘


---

## 🚀 Installation & Setup

1. **Clone the repository:**
```bash
git clone https://github.com/YourUsername/YourRepoName.git

```

## 📡 Bluetooth Command Protocol

If you wish to build your own app or use a serial terminal to control the car, send the following character commands (9600 baud rate):

| Command | Action | Command | Action |
| --- | --- | --- | --- |
| `F` / `f` | Move Forward | `m` | Switch to Manual Mode |
| `B` / `b` | Move Backward | `X` | Switch to Autonomous Mode |
| `L` / `l` | Turn Left | `P` | Initialize Auto-Park Mode |
| `R` / `r` | Turn Right | `K` | Start Path Recording |
| `I` / `G` | Diagonal Forward Right/Left | `T` | Stop Path Recording |
| `J` / `H` | Diagonal Backward Right/Left | `E` | Playback Recorded Path |
| `S` | Stop All Motors | `Q` | Stop Playback |
| `1`-`9` | Adjust Speed (PWM 130 - 255) | `0` | Set Speed to 0 |

---

## ⚙️ Calibration (Auto-Park)

Because the Auto-Park feature uses time-based open-loop control, you may need to adjust the timers based on your floor's friction and your battery voltage.

To calibrate, open `src/APP/autopark1.h` and adjust the milliseconds in the parking matrix:

```cpp
#define TURN1_MS            1050UL  // Time for initial reverse pivot
#define STRAIGHT_MS         850UL   // Time to push deep into the gap
#define TURN2_MS            1050UL  // Time to counter-steer and straighten out

```
## 👥 Team
 
| Name | Role |
|------|------|
| Ali Helmy | Team Member |
| Mekky Mohamed | Team Member |
| Mohamed Saad | Team Member |
| Mohamed Ahmed | Team Member |
| Mohamed Fawzy | Team Member |
| Mohamed Ali Mohamed | Team Member |
| Abdelrahman Walid | Team Member |
 
### Supervisors
- **Prof. Ahmed Salem** — Academic Supervisor
- **Eng. Mohamed Sedeek** — Technical Supervisor
---


*Created by [Mohamed Amr/HV-1 TEAM] - May 2026*
