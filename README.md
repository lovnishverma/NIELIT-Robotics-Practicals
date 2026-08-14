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

---

## 📋 Code Status & Physical Lab Reality Check

### 1. The Code Status (Software Side: 100% Ready)
* **Zero Missing Libraries:** `AFMotor` is bundled directly in `src/`. Sketches compile with zero errors.
* **Exact Pinout Mapping:**
  * **3.1 – 3.6:** Match the exact L293D Shield registers (`M1`, `M2`, `A0`, `A1`).
  * **3.7 – 3.8:** Match the L298N module direct pins (`D2, D3, D8, D9, D10, D11` and `D9, D10, D2, D3, D4, D7`).
* **Timer Safety:** No pin conflicts between PWM timers, SoftwareSerial, and sensor interrupts.
* **Fail-safes Included:** 1.5-second Bluetooth auto-stop, soft motor pauses, and sensor timeout protection.

### 2. The 5 Physical Lab Factors (Hardware Reality Check)
Even with 100% perfect code, beginners will encounter these physical behaviors. Here is how the code is prepared to handle them:

#### ⚠️ 1. Motor Polarity (50% chance a wheel spins backwards on first test)
* **The Reality:** DC motors have two unpolarized wires (Red and Black). If plugged in reverse, the wheel will spin backward.
* **The Fix:** Practical 3.1 runs an isolated 1-wheel diagnostic. If a wheel spins backward, simply swap its two wires at the screw terminal.

#### ⚠️ 2. Cheap Yellow BO Motor Variance (Car drifting slightly)
* **The Reality:** Inexpensive yellow gear motors have a 10%–20% manufacturing speed variance. At equal PWM, one motor will spin slightly faster than the other.
* **The Fix:** Every sketch includes `LEFT_TRIM` and `RIGHT_TRIM` at the top:
```cpp
// If robot drifts right, increase LEFT_TRIM (e.g. +15)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;
```

#### ⚠️ 3. Chassis Leveling & The 3rd Caster Wheel
* **The Reality:** If the brass spacers on the metal ball caster are too tall, the rear lifts and the yellow rubber drive wheels float in the air without grip.
* **The Fix:** Ensure all 3 points (Left rubber tire, Right rubber tire, and Caster ball) press firmly against a flat table.

#### ⚠️ 4. Battery Voltage & Power
* **The Reality:** If using weak/discharged batteries (<6.0V), DC motors will pull down the voltage and cause the Arduino to reset continuously.
* **The Fix:** Use charged **2x 18650 Li-ion batteries (7.4V)** or fresh **4x AA batteries (6.0V)**. Keep the `PWR` jumper ON on the L293D Shield.

#### ⚠️ 5. Sensor Tuning in Practicals 3.7 & 3.8
* **Practical 3.7 (Line Follower):** Students must turn the tiny blue potentiometer screw on the TCRT5000 IR modules so the onboard LED lights up only when placed over black electrical tape (and turns off over white floor).
* **Practical 3.8 (Obstacle Avoider):** Test against flat, hard obstacles (cardboard box, wall, plastic bin) placed 10–30 cm away. Soft cloth or angled cushions may deflect ultrasonic waves.
