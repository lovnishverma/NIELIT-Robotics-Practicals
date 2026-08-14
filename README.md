# NIELIT Robotics Practicals Library

**Version:** `1.1.0`  
**Target Architecture:** `avr` (Arduino UNO R3 / ATmega328P)  
**Curriculum Body:** National Institute of Electronics & Information Technology (NIELIT Ropar)

An educational Arduino library and practical curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)**.

---

## 🛠 Hardware Driver Configuration

* **Practicals 3.1 to 3.6:** Uses the **L293D Motor Driver Shield** plugged directly on top of the Arduino UNO.
* **Practicals 3.7 & 3.8:** Uses the **L298N Motor Driver Module** connected via jumper wires for full digital pin access.

---

## 📑 Robotics Practicals Curriculum

| Practical | Title & Focus | Hardware Setup |
| :--- | :--- | :--- |
| **3.1** | **Robotics Assembly & Power Dynamics** | **L293D Shield:** Left motor on `M1`, Right motor on `M2`. Pre-flight test. |
| **3.2** | **Motor Driver Control** | **L293D Shield:** Motor on `M1`. Forward, Reverse, Release, PWM speed. |
| **3.3** | **DC Gear Motor Actuation** | **L293D Shield:** Forward, Reverse, Pivot turns, and In-place spins on `M1/M2`. |
| **3.4** | **Kinematic Maneuvers — Differential Steering** | **L293D Shield:** 4-sided square demonstration and curve maneuvers. |
| **3.5** | **Speed Control via PWM & Smooth Ramps** | **L293D Shield:** Speed presets (Slow, Med, Fast, Max) and smooth acceleration. |
| **3.6** | **Wireless Smartphone Bluetooth Control** | **L293D Shield:** HC-05 on `A0` (RX) & `A1` (TX) with 1.5s safety auto-stop. |
| **3.7** | **Autonomous Line-Following Robot** | **L298N Module:** Dual TCRT5000 IR sensors on pins `2` & `3`, motors on `5,8,9` & `6,10,11`. |
| **3.8** | **Autonomous Obstacle-Avoiding Robot** | **L298N Module:** HC-SR04 ultrasonic on pins `9` & `10`, motors on `5,2,3` & `6,4,7`. |

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
* **Practical 3.7 (Line Follower):** Left IR on **Pin 2**, Right IR on **Pin 3**, Left Motor on **Pins 5,8,9**, Right Motor on **Pins 6,10,11**.
* **Practical 3.8 (Obstacle Avoider):** Ultrasonic TRIG on **Pin 9**, ECHO on **Pin 10**, Left Motor on **Pins 5,2,3**, Right Motor on **Pins 6,4,7**.
