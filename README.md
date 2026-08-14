# NIELIT Robotics Practicals Library

**Version:** `1.1.0`  
**Target Architecture:** `avr` (Arduino UNO R3 / ATmega328P)  
**Curriculum Body:** National Institute of Electronics & Information Technology (NIELIT Ropar)

An educational Arduino library and practical curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)**.

---

## 🛠 Hardware Allocation

| Practical | Topic | Motor Driver Used | Connections |
| :--- | :--- | :--- | :--- |
| **3.1** | **Assembly & Pre-Flight** | **L293D Motor Shield** | Left Motor on `M1`, Right Motor on `M2` |
| **3.2** | **Driver H-Bridge Control** | **L293D Motor Shield** | Motor on `M1` (Forward, Reverse, Release, PWM) |
| **3.3** | **DC Gear Motors** | **L293D Motor Shield** | Dual BO Motors on `M1` & `M2` |
| **3.4** | **Kinematic Maneuvers** | **L293D Motor Shield** | Square path, Pivot curves, and Point spins on `M1` & `M2` |
| **3.5** | **PWM Speed Control** | **L293D Motor Shield** | Speed presets and smooth acceleration ramps on `M1` & `M2` |
| **3.6** | **Bluetooth Remote Control** | **L293D Motor Shield** | Motors on `M1` & `M2`, HC-05 on `A0` (RX) & `A1` (TX) |
| **3.7** | **Line Following Robot** | **L298N Module** | Left IR $\rightarrow$ Pin 2, Right IR $\rightarrow$ Pin 3<br>Left Motor $\rightarrow$ Pins 5, 8, 9, Right Motor $\rightarrow$ Pins 6, 10, 11 |
| **3.8** | **Obstacle Avoiding Robot** | **L298N Module** | HC-SR04 TRIG $\rightarrow$ Pin 9, ECHO $\rightarrow$ Pin 10<br>Left Motor $\rightarrow$ Pins 5, 2, 3, Right Motor $\rightarrow$ Pins 6, 4, 7 |

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
2. Open **Serial Monitor** at **9600 baud** to view real-time telemetry.

---

## 🔌 Wiring Quick Reference

### 1. Practicals 3.1 – 3.6 (L293D Shield)
* Left Motor $\rightarrow$ **M1**
* Right Motor $\rightarrow$ **M2**
* Battery $\rightarrow$ **EXT_PWR (+M and GND)**
* Bluetooth (3.6) $\rightarrow$ **A0 (RX), A1 (TX)**

### 2. Practicals 3.7 & 3.8 (L298N Module)
* **Practical 3.7 (Line Follower):** Left IR on **Pin 2**, Right IR on **Pin 3**, Left Motor on **Pins 5, 8, 9**, Right Motor on **Pins 6, 10, 11**.
* **Practical 3.8 (Obstacle Avoider):** Ultrasonic TRIG on **Pin 9**, ECHO on **Pin 10**, Left Motor on **Pins 5, 2, 3**, Right Motor on **Pins 6, 4, 7**.
