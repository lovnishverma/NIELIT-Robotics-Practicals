# NIELIT Robotics Practicals Library

**Version:** `1.1.0`  
**Target Architecture:** `avr` (Arduino UNO R3 / Nano / ATmega328P)  
**Curriculum Body:** National Institute of Electronics & Information Technology (NIELIT Ropar)  
**License:** MIT  

An educational Arduino library and practical curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)** as installable, standalone Arduino examples.

Designed for students, educators, and robotics engineers, this curriculum covers embedded robotics fundamentals, 2-wheel differential drive kinematics, motor driver electronics (L293D Shield & L298N Module), wireless Bluetooth teleoperation, optical infrared line following, and ultrasonic autonomous collision avoidance.

---

## 📑 Robotics Practicals Curriculum Overview

| Practical | Syllabus Title | Driver Used | Core Learning Focus | Simulation / Downloads |
| :--- | :--- | :--- | :--- | :--- |
| **3.1** | **Robotics Assembly & Power Dynamics** | **L293D Shield** | Chassis assembly, dual battery power isolation, common GND reference, and pre-flight rotation diagnostics. | [Tinkercad Simulation](https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d) |
| **3.2** | **Motor Driver H-Bridge Control** | **L293D Shield** | H-Bridge switching theory, inductive motor control, Forward, Reverse, Active Brake, Coasting Stop, and PWM. | [Tinkercad Simulation](https://www.tinkercad.com/things/iKSMCzOMKdK-dc-motor-with-l293d-and-arduino) |
| **3.3** | **DC Gear Motor Actuation** | **L293D Shield** | Gear reduction dynamics, independent bi-directional actuation of left and right BO motors, and directional verification. | [Tinkercad Simulation](https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d) |
| **3.4** | **Kinematic Maneuvers — Robocar** | **L293D Shield** | Differential drive kinematics: Forward, Backward, Pivot Turns (wide curves), and Point Spins (zero-radius in-place). | [Tinkercad Simulation](https://www.tinkercad.com/things/2MrhkgLgUiQ-2-wheeled-robot) |
| **3.5** | **Speed Control via PWM & Ramps** | **L293D Shield** | Pulse Width Modulation speed regulation (0–255), multi-tier speed presets, smooth acceleration ramps, and trim tuning. | [Tinkercad Simulation](https://www.tinkercad.com/things/9QkFpe7W9eF-dc-motorspeed-controlvia-pwmpulse-width-modulation) |
| **3.6** | **Wireless Bluetooth Teleoperation** | **L293D Shield** | Wireless remote control via HC-05/HC-06 on pins A0/A1, single-byte RC command parsing, and 1.5s safety auto-stop. | [Download Android APK](https://github.com/lovnishverma/NIELIT-Robotics-Practicals/blob/main/examples/3_6_Wireless_Bluetooth_Kinematic/NIELIT-Robotics-Controller.apk) |
| **3.7** | **Autonomous Line-Following Robot** | **L298N Module** | Closed-loop optical tracking with dual TCRT5000 IR reflectance sensors, discrete differential forward steering. | *(Simulation available in extras)* |
| **3.8** | **Autonomous Obstacle-Avoiding Vehicle**| **L298N Module** | Autonomous navigation using HC-SR04 ultrasonic distance sensor, acoustic time-of-flight, and evasive maneuvers. | [Tinkercad Simulation](https://www.tinkercad.com/things/1BEzwkis74q-interafacing-obstacle-using-ultrasonic-sensor) |

---

## 🛠 Master Hardware Allocation Matrix

The curriculum utilizes a dedicated hardware architecture optimized for each educational stage:

| Practical | Topic | Motor Driver Used | Motor Connections | Sensor / Peripheral Connections |
| :--- | :--- | :--- | :--- | :--- |
| **3.1** | **Assembly & Pre-Flight** | **L293D Motor Shield** | Left $\rightarrow$ `M1`, Right $\rightarrow$ `M2` | Status LED on `Pin 13` |
| **3.2** | **Driver H-Bridge Control** | **L293D Motor Shield** | Single Motor on `M1` | Onboard Shield Controls |
| **3.3** | **DC Gear Motors** | **L293D Motor Shield** | Left $\rightarrow$ `M1`, Right $\rightarrow$ `M2` | Dual Channel Actuation |
| **3.4** | **Kinematic Maneuvers** | **L293D Motor Shield** | Left $\rightarrow$ `M1`, Right $\rightarrow$ `M2` | Differential Kinematics |
| **3.5** | **PWM Speed Control** | **L293D Motor Shield** | Left $\rightarrow$ `M1`, Right $\rightarrow$ `M2` | Timer-based PWM Speed Ramping |
| **3.6** | **Bluetooth Remote Control** | **L293D Motor Shield** | Left $\rightarrow$ `M1`, Right $\rightarrow$ `M2` | HC-05 `TXD` $\rightarrow$ `A0` (RX), `RXD` $\rightarrow$ `A1` (TX) |
| **3.7** | **Line Following Robot** | **L298N Module** | Left $\rightarrow$ `Pins 5, 8, 9`<br>Right $\rightarrow$ `Pins 6, 10, 11` | Left IR $\rightarrow$ `Pin 2`, Right IR $\rightarrow$ `Pin 3` |
| **3.8** | **Obstacle Avoiding Robot** | **L298N Module** | Left $\rightarrow$ `Pins 5, 2, 3`<br>Right $\rightarrow$ `Pins 6, 4, 7` | HC-SR04 `TRIG` $\rightarrow$ `Pin 9`, `ECHO` $\rightarrow$ `Pin 10` |

### Why This Dual-Driver Architecture?
1. **Practicals 3.1 to 3.6 (L293D Shield):** The blue L293D shield plugs directly on top of the Arduino Uno, eliminating messy breadboard wiring for early motor control, kinematics, PWM ramping, and Bluetooth.
2. **Practicals 3.7 & 3.8 (L298N Module):** Multi-sensor autonomous robotics (dual TCRT5000 IR lines and HC-SR04 ultrasound) requires direct access to high-speed hardware timer pins and digital GPIO lines. The standalone L298N module provides clear wire routing without shield pin limitations.

---

## 🚗 Mobile Robot Kinematics & Differential Drive Physics

### 1. The 2-Wheel Differential Drive Architecture

Unlike four-wheel Ackerman steering cars (which steer using a front mechanical rack-and-pinion), mobile robots use **Differential Drive Kinematics**:

```text
               CONFIGURATION A:                       CONFIGURATION B:
            Caster Leading at FRONT                Caster Trailing at REAR
        (Shopping Cart / Tricycle Style)       (Front-Wheel Drive / Tail-dragger)
        ────────────────────────────────       ───────────────────────────────────
                     ▲                                      ▲
               [ FORWARD ]                            [ FORWARD ]
                    │                                      │
             ( BALL CASTER )                        [Left] ───┴─── [Right]
                    │                                      │
                    │                                      │
             [Left] ───┴─── [Right]                 ( BALL CASTER )
```

### 2. Differential Drive Kinematic Equations
Motion is governed by the relative velocities of the left wheel ($v_L$) and right wheel ($v_R$):

$$\text{Linear Velocity: } v = \frac{v_R + v_L}{2}$$

$$\text{Angular Velocity: } \omega = \frac{v_R - v_L}{L} \quad (\text{where } L \text{ is the wheelbase track width})$$

* **Straight Forward:** $v_L = +v, \; v_R = +v \implies \omega = 0$
* **Straight Reverse:** $v_L = -v, \; v_R = -v \implies \omega = 0$
* **Pivot Turn Left (Wide Curve):** $v_L = 0, \; v_R = +v \implies \text{Robot pivots around stationary left wheel}$
* **Pivot Turn Right (Wide Curve):** $v_L = +v, \; v_R = 0 \implies \text{Robot pivots around stationary right wheel}$
* **Point Spin Left (360° on the Spot):** $v_L = -v, \; v_R = +v \implies \text{Zero turning radius rotation around chassis center}$
* **Point Spin Right (360° on the Spot):** $v_L = +v, \; v_R = -v \implies \text{Zero turning radius rotation around chassis center}$

### 3. Demystifying the 3rd Wheel (The Ball Caster)
* **Why does the 3rd wheel have no motor?**  
  In a 2WD differential drive robot, steering is performed entirely by varying the speed and direction of the two yellow drive motors. The metal ball caster is a **passive, unpowered, omni-directional balance support** designed to keep the robot upright.
* **Assembly Leveling Rule:** Ensure all three ground contact points (Left rubber tire, Right rubber tire, and Caster ball) touch the flat floor with equal pressure. If the caster standoff is too tall, the drive wheels float in the air and lose traction.

---

## 🔌 Hardware Schematics & Wiring Guides

### 1. L293D Motor Driver Shield (Practicals 3.1 to 3.6)

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
               |  [+]  [-]      [PWR]       [ 5V GND A0-5] | Bluetooth Module
               |  EXT_PWR       Jumper      Analog Row     | (A0 = RX, A1 = TX)
               +---+----+---------------------+------------+
                   |    |                     |
                   |    +---------------------┴--> Battery Negative (-) & Common GND
                   +-----------------------------> Battery Positive (+) [6.0V - 7.4V]
```

#### Bluetooth HC-05 Resistor Voltage Divider (3.3V RX Protection):
Arduino Pin A1 (5V TX) connects to HC-05 RXD via a 1kΩ / 2kΩ divider:

$$\text{Arduino A1 (5V TX)} \xrightarrow{\quad 1\text{k}\Omega \quad} \text{HC-05 RXD} \xrightarrow{\quad 2\text{k}\Omega \quad} \text{GND}$$

---

### 2. L298N Dual H-Bridge Motor Driver Module (Practicals 3.7 & 3.8)

```text
               +-----------------------------+
               |        L298N MODULE         |
               |                             |
  Left Motor   | [o]                     [o] | Right Motor
  OUT1 & OUT2  | [o]                     [o] | OUT3 & OUT4
               |                             |
               |  [+]   [-]   [+5V]          |
               |  12V   GND    5V            |
               +---+-----+-----+-------------+
                   |     |     |
                   |     |     +-----> Arduino 5V
                   |     +-----------> Arduino GND & Battery (-)
                   +-----------------> Battery (+) [6.0V - 7.4V]
```

#### Practical 3.7 (Line Following Pinout):
* **Left TCRT5000 IR Sensor:** `OUT` $\rightarrow$ Arduino `Pin 2`
* **Right TCRT5000 IR Sensor:** `OUT` $\rightarrow$ Arduino `Pin 3`
* **L298N Left Motor:** `ENA` $\rightarrow$ `Pin 5` (PWM), `IN1` $\rightarrow$ `Pin 8`, `IN2` $\rightarrow$ `Pin 9`
* **L298N Right Motor:** `ENB` $\rightarrow$ `Pin 6` (PWM), `IN3` $\rightarrow$ `Pin 10`, `IN4` $\rightarrow$ `Pin 11`

#### Practical 3.8 (Obstacle Avoidance Pinout):
* **HC-SR04 Ultrasonic Sensor:** `TRIG` $\rightarrow$ `Pin 9`, `ECHO` $\rightarrow$ `Pin 10`, `VCC` $\rightarrow$ `5V`, `GND` $\rightarrow$ `GND`
* **L298N Left Motor:** `ENA` $\rightarrow$ `Pin 5` (PWM), `IN1` $\rightarrow$ `Pin 2`, `IN2` $\rightarrow$ `Pin 3`
* **L298N Right Motor:** `ENB` $\rightarrow$ `Pin 6` (PWM), `IN3` $\rightarrow$ `Pin 4`, `IN4` $\rightarrow$ `Pin 7`

---

## 📋 Code Status & Physical Lab Reality Check

### 1. The Code Status (Software Side: 100% Verified)
* **Zero Missing Libraries:** `AFMotor` driver is bundled directly in `src/`. Sketches compile with zero external downloads.
* **Exact Pinout Alignment:** Every register, timer, and GPIO line matches the specified hardware driver.
* **Timer Safety:** No conflicts between PWM timers, `SoftwareSerial`, and external interrupts.
* **Fail-safes Included:** 1.5-second Bluetooth auto-stop, soft motor pauses to protect gearbox teeth, and sensor timeout protection.

---

### 2. The 5 Physical Lab Factors (Hardware Reality Check)

Even with 100% verified code, mobile robotics involves real-world physics. Here is how to handle physical lab behaviors:

#### ⚠️ 1. Motor Polarity (50% chance a wheel rotates backwards on first run)
* **The Reality:** DC motors have two unpolarized leads (Red and Black). If connected in reverse, the wheel rotates backward.
* **The Fix:** Practical 3.1 runs an isolated single-wheel diagnostic. If a wheel rotates backward, swap its two wires at the screw terminal.

#### ⚠️ 2. Inherent Yellow BO Motor RPM Variance (Car drifting slightly)
* **The Reality:** Inexpensive yellow gear motors have a 10%–20% manufacturing speed tolerance. At equal PWM, one motor naturally spins slightly faster.
* **The Fix:** Every sketch includes `LEFT_TRIM` and `RIGHT_TRIM` at the top:
```cpp
// If robot drifts right, increase LEFT_TRIM (e.g. +15) or decrease RIGHT_TRIM (-15)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;
```

#### ⚠️ 3. Chassis Leveling & Caster Height
* **The Reality:** If the brass standoffs on the ball caster are too tall, the drive wheels float off the ground and slip.
* **The Fix:** Ensure all 3 contact points (Left rubber tire, Right rubber tire, and Caster ball) press firmly against a flat table.

#### ⚠️ 4. Battery Voltage & Power Delivery
* **The Reality:** If using discharged batteries (<6.0V), DC motors will pull down the logic voltage and cause continuous Arduino brownout resets.
* **The Fix:** Use charged **2x 18650 Li-ion batteries (7.4V)** or fresh **4x AA batteries (6.0V)**. Keep the `PWR` jumper ON on the L293D Shield.

#### ⚠️ 5. Sensor Tuning in Practicals 3.7 & 3.8
* **Practical 3.7 (Line Follower):** Adjust the tiny blue potentiometer screw on each TCRT5000 IR module so the indicator LED turns ON over black electrical tape and turns OFF over white floor.
* **Practical 3.8 (Obstacle Avoider):** Test against flat, hard obstacles (cardboard, wall, plastic bin) placed 10–30 cm away. Soft cloth or curtains can absorb ultrasonic pings.

---

## 🚀 Quick Start Guide for Students

### Step 1 — Open Arduino IDE & Select Board
1. Connect Arduino UNO via USB.
2. Select your board and port:
   > **Tools → Board → Arduino AVR Boards → Arduino Uno**  
   > **Tools → Port → [Select Connected COM Port]**

### Step 2 — Install NIELIT Robotics Practicals Library
1. Download or clone this repository as a `.zip` archive.
2. In the Arduino IDE:
   > **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP file or folder.

### Step 3 — Open Any Practical Example
Navigate to:
> **File → Examples → NIELIT Robotics Practicals → [Select Practical 3.1 to 3.8]**

### Step 4 — Verify, Upload & Monitor
1. Click **Verify** (✔) then **Upload** (➔).
2. Open **Serial Monitor** (**Tools → Serial Monitor**) at **9600 baud** to view real-time diagnostics.

---

## 🛠 Hardware Bill of Materials (BOM)

| Component | Specification | Quantity | Notes |
| :--- | :--- | :---: | :--- |
| **Microcontroller** | Arduino UNO R3 (ATmega328P) | 1 | 5V Logic, 16 MHz AVR |
| **Motor Driver 1** | Blue L293D Motor Driver Shield | 1 | Plugged on top of UNO (Practicals 3.1 – 3.6) |
| **Motor Driver 2** | Red L298N Dual H-Bridge Module | 1 | Standalone module with flyback diodes (Practicals 3.7 & 3.8) |
| **Robotic Chassis** | 2WD Acrylic Kit with Ball Caster | 1 | Dual yellow BO gear motors (1:48 gear ratio) |
| **Power Supply** | 2x 18650 Li-ion Pack (7.4V) or 4x AA (6V) | 1 | External motor supply rail |
| **Line Sensors** | TCRT5000 IR Reflectance Modules | 2 | Digital outputs with potentiometer tuning (Practical 3.7) |
| **Distance Sensor** | HC-SR04 Ultrasonic Distance Sensor | 1 | 5V DC, 40 kHz ultrasound transducer pair (Practical 3.8) |
| **Wireless Module** | HC-05 / HC-06 Bluetooth Breakout | 1 | Default 9600 baud, 3.3V UART (Practical 3.6) |
| **Resistors** | 1kΩ and 2kΩ Resistors | 1 set | For 5V-to-3.3V voltage divider on Bluetooth RX pin |

---

## 📂 Repository Structure

```text
NIELIT-Robotics-Practicals/
├── library.properties                  # Arduino Library Manager metadata
├── keywords.txt                        # Syntax highlighting definitions
├── INSTALL.txt                         # Installation instructions
├── README.md                           # Master documentation
├── src/
│   ├── AFMotor.h                       # Zero-dependency L293D shield driver header
│   ├── AFMotor.cpp                     # Zero-dependency L293D shield driver implementation
│   ├── NIELIT_Robotics_Practicals.h    # Core helper library header
│   └── NIELIT_Robotics_Practicals.cpp  # Core library implementation
├── examples/
│   ├── 3_1_Robotics_Assembly_Power_Distribution/
│   ├── 3_2_Inductive_Load_Control_L293D/
│   ├── 3_3_DC_Gear_Motor_Actuation/
│   ├── 3_4_Kinematic_Maneuvers_Robocar/
│   ├── 3_5_Speed_Vector_PWM_Control/
│   ├── 3_6_Wireless_Bluetooth_Kinematic/
│   ├── 3_7_Line_Following_Robot/
│   └── 3_8_Obstacle_Avoiding_Vehicle/
└── extras/
    ├── Hardware_BOM.md                 # Detailed component list
    ├── Pinouts_and_Wiring_Guide.md     # Comprehensive pinout schematics
    └── Validation_Matrix.md            # Hardware & verification checklist
```

---

## 👨‍🏫 Authors & Institutional Credits

* **Curriculum Design & Technical Authorship:**
  * **Dr. Sarwan Singh** — Joint Director, National Institute of Electronics & Information Technology (NIELIT Ropar)
  * **Lovnish Verma** — Project Engineer, NIELIT Ropar
* **Organization:** National Institute of Electronics & Information Technology (NIELIT), Ministry of Electronics & Information Technology (MeitY), Government of India.
