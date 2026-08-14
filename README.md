# NIELIT Robotics Practicals Library

**Version:** `1.1.0`  
**Target Architecture:** `avr` (Arduino UNO R3 / ATmega328P)  
**Motor Driver:** Blue L293D Motor Driver Shield for Arduino  
**Curriculum Body:** National Institute of Electronics & Information Technology (NIELIT Ropar)

An educational Arduino library and practical curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)** configured for the popular **L293D Motor Driver Shield**.

---

## 🛠 Hardware Setup (L293D Motor Shield)

Plug the blue **L293D Motor Driver Shield** directly on top of your Arduino UNO.

```text
               +-------------------------------------------+
               |  [SERVOS]      ARDUINO UNO HEADERS        |
               |  (9, 10)                                  |
               |                                           |
  Left Motor   | [o]                                   [o] | Right Motor
  Terminal M1  | [o]      [IC1]       [IC2]            [o] | Terminal M2
               | [o]      L293D       L293D            [o] |
               | [o]                                   [o] |
               | [o]             [IC3]                 [o] |
               |                74HC595                    |
               |                                           |
               |  [+]  [-]      [PWR]       [ 5V GND A0-5] | Sensors Header
               |  EXT_PWR       Jumper      Analog Row     | (A0, A1, A2...)
               +---+----+---------------------+------------+
                   |    |                     |
                   |    +---------------------┴--> Battery (-) & Ground
                   +-----------------------------> Battery (+) [6.0V - 7.4V]
```

### Motor & Power Connections:
* **Left Motor:** Screw terminal **M1** (Top-Left)
* **Right Motor:** Screw terminal **M2** (Top-Right)
* **Battery Supply:** Screw terminal **EXT_PWR** (+M and GND) with **PWR Jumper ON**
* **Sensors (IR, Ultrasonic, Bluetooth):** Connect to **A0** and **A1** on the shield's analog breakout row.

---

## 📑 Robotics Practicals Curriculum

| Practical | Title & Focus | What You Will Learn |
| :--- | :--- | :--- |
| **3.1** | **Robotics Assembly & Power Dynamics** | 2WD chassis assembly, L293D shield wiring, and pre-flight rotation diagnostics on M1/M2. |
| **3.2** | **Motor Driver Control (L293D Shield)** | How L293D controls motor states: Forward, Reverse, Stop/Release, and PWM Speed Control. |
| **3.3** | **DC Gear Motor Actuation** | Independent control of dual BO motors for Forward, Reverse, Pivot turns, and Axial spins. |
| **3.4** | **Kinematic Maneuvers — Differential Steering** | Executing a 4-sided square demonstration, wide curve pivot turns, and 360° point spins. |
| **3.5** | **Speed Control via PWM & Smooth Ramps** | Regulating speed with PWM (0–255), speed presets, smooth acceleration ramps, and trim tuning. |
| **3.6** | **Wireless Smartphone Bluetooth Control** | Wireless RC control via HC-05 on pins A0/A1 with 1.5s safety auto-stop. |
| **3.7** | **Autonomous Line-Following Robot** | Closed-loop optical line tracking using dual TCRT5000 IR sensors on pins A0/A1. |
| **3.8** | **Autonomous Obstacle-Avoiding Robot** | Collision avoidance using HC-SR04 ultrasonic distance sensor on pins A0/A1. |

---

## 🚀 Quick Start Guide

### Step 1 — Open Arduino IDE & Select Board
> **Tools → Board → Arduino AVR Boards → Arduino Uno**  
> **Tools → Port → [Select Connected COM Port]**

### Step 2 — Open Any Practical Example
Navigate to:
> **File → Examples → NIELIT Robotics Practicals → [Select Practical 3.1 to 3.8]**

### Step 3 — Verify & Upload
1. Click **Verify** (✔) and **Upload** (➔).
2. Open **Serial Monitor** at **9600 baud** to view real-time diagnostics.

---

## 🛠 Hardware Bill of Materials (BOM)

* **Microcontroller:** Arduino UNO R3
* **Motor Driver:** Blue L293D Motor Driver Shield (with 2x L293D + 74HC595)
* **Chassis Kit:** 2WD Smart Robot Car Kit (2 yellow BO gear motors + 1 ball caster wheel)
* **Power Supply:** 2x 18650 Li-ion Batteries (7.4V) connected to `EXT_PWR`
* **Sensors:**
  * 2x TCRT5000 IR Reflective Sensors (Practical 3.7, on A0/A1)
  * 1x HC-SR04 Ultrasonic Distance Sensor (Practical 3.8, on A0/A1)
  * 1x HC-05 / HC-06 Bluetooth Module (Practical 3.6, on A0/A1)
