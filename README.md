# NIELIT Robotics Practicals Library

**Version:** `1.0.0`  
**Target Architecture:** `avr` (Arduino UNO R3 / Nano / ATmega328P)  
**Curriculum Body:** National Institute of Electronics & Information Technology (NIELIT Ropar)

An educational Arduino library and practical curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)** as installable, standalone Arduino Examples.

This library is designed for students, educators, and robotics enthusiasts learning embedded robotics, mobile robot kinematics, motor driver electronics, wireless teleoperation, and autonomous navigation algorithms.

---

## 📑 Robotics Practicals Curriculum

| Practical | Title & Core Focus | Simulation Link(s) |
| :--- | :--- | :--- |
| **3.1** | **Robotics Assembly & Power Distribution Dynamics**<br>Dual DC motor chassis assembly, separate logic/motor power rails, common GND bonding, and pre-flight rotation diagnostics. | [Tinkercad Simulation](https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d) |
| **3.2** | **Inductive Load Control — Interfacing the L293D Motor Driver Shield**<br>H-Bridge switching architecture, inductive back-EMF suppression, and state verification (Forward, Active Dynamic Brake, Reverse, Coasting Stop, PWM). | *(simulation link will be added soon)* |
| **3.3** | **Direct Current Actuation — Interfacing DC Gear Motors**<br>Gear reduction dynamics, independent bi-directional actuation of left and right BO motors, and directional verification. | [Tinkercad Simulation](https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d) |
| **3.4** | **Kinematic Maneuvers — 2-Wheel Robocar Basic Movements**<br>Differential drive kinematic motion primitives (Forward, Backward, Pivot Turns, Point Spins) executing an open-loop geometric demonstration. | *(simulation link will be added soon)* |
| **3.5** | **Speed Vector Regulation — PWM Motor Control**<br>Pulse Width Modulation speed regulation, multi-tier speed presets, smooth acceleration/deceleration ramps to reduce inrush current, and differential trim calibration. | *(simulation link will be added soon)* |
| **3.6** | **Wireless Kinematic – Bluetooth Interfacing**<br>Wireless teleoperation via HC-05/HC-06 Bluetooth module, case-insensitive RC protocol parser, speed scaling, and communication timeout auto-stop. | *(simulation link will be added soon)* |
| **3.7** | **Autonomous Navigation Capstone I — Line-Following Robot**<br>Dual infrared (TCRT5000) reflectance tracking, surface absorption detection, configurable black/white polarity, and closed-loop discrete differential steering. | *(simulation link will be added soon)* |
| **3.8** | **Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle**<br>Autonomous collision avoidance using HC-SR04 ultrasonic distance sensor, dual-sample acoustic noise filtering, deterministic startup validation, and defensive safe stop on sensor timeout. | [Tinkercad Simulation](https://www.tinkercad.com/things/1BEzwkis74q-interafacing-obstacle-using-ultrasonic-sensor) |

---

## 🚀 Recommended Student Workflow

Students install the library once via Arduino IDE to access all 8 practicals directly under the **Examples** menu.

### Step 1 — Install Board Package
Ensure you have the **Arduino AVR Boards** core installed in the Arduino IDE:
> **Tools → Board → Arduino AVR Boards → Arduino Uno**

### Step 2 — Install NIELIT Robotics Practicals Library
1. Download or clone this repository as a `.zip` archive.
2. In Arduino IDE, navigate to:
   > **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP file or repository folder.

Alternatively, if installing via Library Manager:
> **Tools → Manage Libraries... → Search `NIELIT Robotics Practicals` → Install**

### Step 3 — Open a Practical Example
Navigate to:
> **File → Examples → NIELIT Robotics Practicals → [Select Practical 3.1 to 3.8]**

### Step 4 — Verify Hardware Connections & Power
Review the circuit diagram, pin table, and voltage constraints provided in the top header comments of the sketch or in [`extras/Pinouts_and_Wiring_Guide.md`](extras/Pinouts_and_Wiring_Guide.md).

### Step 5 — Upload and Monitor Telemetry
1. Click **Verify** (✔) to compile the sketch.
2. Click **Upload** (➔) to flash the program to the Arduino.
3. Open **Serial Monitor** at **9600 baud** to view real-time diagnostics and sensor telemetry.

---

## 🛠 Hardware Bill of Materials (BOM)

| Component | Specification | Quantity | Notes |
| :--- | :--- | :---: | :--- |
| **Microcontroller** | Arduino UNO R3 (ATmega328P) | 1 | 5V Logic, 16 MHz, Hardware Timer0 PWM on D5/D6 |
| **Robotic Platform** | 2WD Chassis with ball caster wheel | 1 | Standard educational 2-wheel mobile robot kit |
| **Actuators** | DC Yellow BO Gear Motors | 2 | Nominal: 3V–6V DC, 1:48 gear ratio |
| **Motor Driver** | L293D Dual H-Bridge Driver IC / Shield | 1 | Dual H-Bridge with internal clamp diodes |
| **Motor Power Supply** | 4x AA Battery Holder (6.0V) or 2S Li-ion (7.4V) | 1 | Dedicated motor power rail (**6.0V – 7.4V Recommended**) |
| **Distance Sensor** | HC-SR04 Ultrasonic Distance Sensor | 1 | 5V DC, 40 kHz ultrasound transducer pair (Practical 3.8) |
| **Line Sensors** | TCRT5000 IR Line Tracking Modules | 2 | Dual phototransistor reflectance modules (Practical 3.7) |
| **Wireless Module** | HC-05 / HC-06 Bluetooth Breakout Board | 1 | Default 9600 baud (Practical 3.6, requires 3.3V RX divider) |
| **Level Shifter** | 1kΩ and 2kΩ Resistors | 1 set | For 5V-to-3.3V voltage divider on Bluetooth RX pin |

> For the detailed component specifications and lab equipment guide, see [`extras/Hardware_BOM.md`](extras/Hardware_BOM.md).

---

## 🔌 GPIO & Pinout Configuration

### 1. Standard Motor Driver Pins (Practicals 3.1 – 3.6, 3.8)
```cpp
// Left Motor Driver Pins
#define ENA 5   // Timer0 PWM Speed Enable (~976 Hz)
#define IN1 2   // Left Motor Direction Input 1
#define IN2 3   // Left Motor Direction Input 2

// Right Motor Driver Pins
#define ENB 6   // Timer0 PWM Speed Enable (~976 Hz)
#define IN3 4   // Right Motor Direction Input 1
#define IN4 7   // Right Motor Direction Input 2
```

### 2. Ultrasonic Sensor Pins (Practical 3.8)
```cpp
#define TRIG 9   // Ultrasonic Trigger Pulse Output (10us pulse)
#define ECHO 10  // Echo Return Pulse Input (pulseIn timing)
```

### 3. Line Follower IR Sensor & Motor Pins (Practical 3.7)
*Avoids GPIO pin conflicts by dedicating D2/D3 to IR Sensors and shifting motor lines to D8–D11:*
```cpp
// IR Sensor Digital Inputs
#define LEFT_SENSOR  2
#define RIGHT_SENSOR 3

// Motor Driver Direction Pins
#define ENA 5    // Left Motor Speed (Timer0 PWM)
#define IN1 8    // Left Motor Direction Input 1
#define IN2 9    // Left Motor Direction Input 2
#define ENB 6    // Right Motor Speed (Timer0 PWM)
#define IN3 10   // Right Motor Direction Input 1
#define IN4 11   // Right Motor Direction Input 2
```

### 4. Wireless Bluetooth Module Pins (Practical 3.6)
```cpp
#define BT_RX_PIN 12   // Arduino RX <- HC-05 TXD (3.3V TTL out)
#define BT_TX_PIN 13   // Arduino TX -> HC-05 RXD (via 1k/2k 3.3V voltage divider)
```

> For comprehensive IC pinout schematics and truth tables, see [`extras/Pinouts_and_Wiring_Guide.md`](extras/Pinouts_and_Wiring_Guide.md).

---

## ⚡ Power Distribution & Electrical Safety Rules

```text
    +-------------------------------------------------------+
    |         MOTOR BATTERY PACK (6.0V - 7.4V)              |
    |         (4x AA Alkaline or 2S Li-ion Pack)            |
    +---------------------------+---------------------------+
                                |
               +----------------+----------------+
               | (+)                             | (-)
               v                                 v
       +---------------+                 +---------------+
       | L293D / L298N |                 |  COMMON GND   |
       | Motor VCC2/VM |                 |    BUSBAR     |
       +---------------+                 +-------+-------+
                                                 |
               +---------------------------------+
               |
               v
       +---------------+
       |  Arduino GND  | <------- 5V Regulated USB Supply
       +---------------+
```

### Safety Principles:
1. **Never Power Motors Directly from the Arduino 5V Pin:** DC motors draw high stall currents (>800mA) and generate inductive back-EMF spikes that can destroy the Arduino linear regulator or cause continuous MCU brownout resets.
2. **Mandatory Common Ground Reference:** The negative (`-`) terminal of the motor battery pack **MUST** connect directly to an Arduino `GND` pin. Without a shared 0V reference, TTL control signals cannot be recognized by the motor driver.
3. **Motor Voltage Scoping:** DC yellow BO motors are rated for 3V to 6V. Accounting for the L293D bipolar saturation drop ($V_{CE,sat} \approx 1.4\text{V}-1.8\text{V}$), a **6.0V to 7.4V** external supply is recommended. Never connect 12V or 3S LiPo battery packs directly to 3V-rated BO gearboxes.
4. **Bluetooth RX Protection:** The HC-05 baseband transceiver operates at 3.3V logic. Always use a 1kΩ / 2kΩ resistor divider between Arduino D13 (5V TX) and the HC-05 RXD pin to prevent over-voltage degradation.

---

## 🧭 Autonomous Navigation Capstone Details

### Capstone I: Line Following (Practical 3.7)
* **Closed-Loop Discrete Differential Steering:** Uses discrete forward speed differentials (`SLOW_SPEED = 50` on the inside wheel, `TURN_SPEED = 180` on the outside wheel) to smoothly steer the vehicle toward the line without reverse-motor jerk.
* **Track Geometry:** Designed for 15mm–19mm black electrical tape straddled by dual TCRT5000 sensors spaced 22mm–28mm apart.
* **Configurable Polarity:** Set `BLACK_LINE_MODE = true` for black line on white surface, or `false` for inverted modules.

### Capstone II: Obstacle Avoidance (Practical 3.8)
* **Deterministic Startup Validation:** Validates ultrasonic sensor health across a 10-attempt bounded check before autonomous mode is activated. If the sensor fails, the robot halts and locks in safe stop.
* **Strict Defensive Safety Model (`UNKNOWN ≠ CLEAR`):**
  | Distance Reading | Operating State | Executed Action |
  | :--- | :--- | :--- |
  | **Valid Distance $> 25\text{ cm}$** | Path Confirmed Clear | Cruise Forward (`CRUISE_SPEED = 180`) |
  | **$12\text{ cm} < \text{Valid Distance} \le 25\text{ cm}$** | Impediment Detected | Stop → Reverse 350ms → Spin Turn 450ms |
  | **$0 < \text{Valid Distance} \le 12\text{ cm}$** | Critical Proximity | Emergency Stop → Extended Reverse 600ms → Wide Spin Turn 600ms |
  | **Timeout (`-1`) / Sensor Loss** | Sensor Fault / Unknown | **DEFENSIVE SAFE STOP (Hold in place)** |
* **Deadlock Prevention:** Alternates evasive turn direction (`turnRightNext = !turnRightNext`) to prevent corner trapping.

---

## 📁 Repository Structure

```text
NIELIT-Robotics-Practicals/
├── .github/
│   └── workflows/
│       └── compile.yml                 # Automated CI compilation workflow
├── library.properties                  # Arduino Library Manager metadata
├── keywords.txt                        # Syntax highlighting definitions
├── INSTALL.txt                         # Quick install guide
├── LICENSE                             # MIT License
├── README.md                           # Master documentation
├── src/
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
    ├── Pinouts_and_Wiring_Guide.md     # IC pinouts and wiring schematics
    ├── Validation_Matrix.md            # Verification status and HW check guide
    └── Hardware_BOM.md                 # Complete Bill of Materials
```

---

## 🔍 Verification Status & Known Limitations

For complete details on automated compiler validation vs physical laboratory testing requirements, refer to [`extras/Validation_Matrix.md`](extras/Validation_Matrix.md).

* **Statically & Compiler-Verified:** 100% build pass with 0 errors on official AVR core (`arduino-cli compile --fqbn arduino:avr:uno`), pin conflict absence, defensive timeout stops, and bounds clamping.
* **Requires Physical Hardware Verification:** Motor terminal wire polarity (bench rotation test), floor-specific 90° turn timing calibration (`TURN_TIME_MS`), and TCRT5000 optical potentiometer threshold tuning.

---

## 👥 Authors & Maintainers

* **Dr. Sarwan Singh**
* **Lovnish Verma**

*National Institute of Electronics & Information Technology (NIELIT Ropar)*  
*Embedded Systems, Robotics, AI & IoT Division*

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
