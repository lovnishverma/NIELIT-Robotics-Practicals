# NIELIT Robotics Practicals Library

**Version:** `1.0.3`

An all-in-one Arduino library and practical curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)** as installable, standalone Arduino Examples.

This library is designed for students, educators, and robotics enthusiasts learning embedded robotics, mobile robot kinematics, motor driver electronics, wireless teleoperation, and autonomous navigation algorithms.

---

## 📑 Robotics Practicals Curriculum

| Practical | Title & Focus | Simulation Link(s) |
| :--- | :--- | :--- |
| **3.1** | **Robotics Assembly & Power Distribution Dynamics**<br>Dual DC motor chassis assembly, separate logic/motor power rails, common GND bonding, and startup diagnostics. | [Tinkercad Simulation](https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d) |
| **3.2** | **Inductive Load Control — Interfacing the L293D Motor Driver Shield**<br>H-Bridge internal switching architecture, inductive back-EMF suppression, and state verification (CW, CCW, Active Brake, Coast). | *(simulation link will be added soon)* |
| **3.3** | **Direct Current Actuation — Interfacing DC Gear Motors**<br>Gear reduction dynamics, independent bi-directional actuation of left and right BO motors, and directional verification. | [Tinkercad Simulation](https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d) |
| **3.4** | **Kinematic Maneuvers — 2-Wheel Robocar Basic Movements**<br>Differential drive kinematic motion primitives (Forward, Backward, Pivot Turns, Point Spins) running an autonomous square navigation drill. | *(simulation link will be added soon)* |
| **3.5** | **Speed Vector Regulation — PWM Motor Control**<br>Pulse Width Modulation speed regulation, multi-tier speed presets, smooth acceleration/deceleration ramps, and differential trim calibration. | *(simulation link will be added soon)* |
| **3.6** | **Wireless Kinematic – Bluetooth Interfacing**<br>Wireless teleoperation via HC-05/HC-06 Bluetooth module, standard RC character protocol parsing, and fail-safe safety watchdog timer. | *(simulation link will be added soon)* |
| **3.7** | **Autonomous Navigation Capstone I — Line-Following Robot**<br>Dual infrared (IR TCRT5000) reflectance tracking, surface absorption detection, configurable black/white polarity, and reactive differential steering. | *(simulation link will be added soon)* |
| **3.8** | **Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle**<br>Autonomous collision avoidance using HC-SR04 ultrasonic distance sensor, real-time echo timing calculation, safety distance thresholding, and automated evasive escape routines. | [Tinkercad Simulation](https://www.tinkercad.com/things/1BEzwkis74q-interafacing-obstacle-using-ultrasonic-sensor) |

---

# 🚀 Recommended Student Workflow

Students only need to install the library once.

### Step 1 — Install Board Package
Ensure you have the appropriate board core installed in the Arduino IDE (e.g., **Arduino AVR Boards** for Arduino Uno/Nano, or **ESP32** for ESP32-based robots).

Select your board:
> **Tools → Board → Arduino AVR Boards → Arduino Uno**

### Step 2 — Install NIELIT Robotics Practicals
1. Download or clone this repository as a `.zip` archive.
2. In Arduino IDE, navigate to:
   > **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP file or repository folder.

Alternatively, if installing via Library Manager:
> **Tools → Manage Libraries... → Search `NIELIT Robotics Practicals` → Install**

### Step 3 — Install Third-Party Dependencies
Most practicals use standard built-in Arduino core libraries. If working with SoftwareSerial or Servo modules, verify that they are available.

### Step 4 — Open a Practical
Go to:
> **File → Examples → NIELIT Robotics Practicals → [Select Practical]**

### Step 5 — Verify Hardware Connections
Follow the circuit diagrams and pin definitions provided in the top header comments of each practical sketch.

### Step 6 — Select Port and Board
Select:
* Target Board: **Arduino Uno** (or corresponding board)
* Appropriate COM / Serial Port

### Step 7 — Verify and Upload
1. Click **Verify** (✔) to compile the sketch.
2. Click **Upload** (➔) to flash the program to the robot.
3. Open **Serial Monitor** at **9600 baud** to view real-time diagnostics and telemetry.

---

# 📦 Third-Party Dependencies

| Practical | Required Dependency | Included in Core? | Notes |
| :--- | :--- | :--- | :--- |
| **3.1 – 3.5** | None (Standard Arduino Core) | Yes | Uses `Arduino.h` standard GPIO and PWM APIs |
| **3.6** | `SoftwareSerial` | Yes (AVR Core) | Included with Arduino AVR board package |
| **3.7** | None (Digital IR Comparator) | Yes | Uses standard digital inputs |
| **3.8** | None (Pulse timing / HC-SR04) | Yes | Uses `pulseIn()` timing API |

---

# 🛠 Hardware Components

The practical series utilizes standard laboratory robotics equipment:

* **Microcontroller:** Arduino UNO R3 / Nano / ESP32
* **Robotic Platform:** 2-Wheel Drive (2WD) chassis with ball caster wheel
* **Actuators:** 2x DC Yellow BO (Battery Operated) Gear Motors (3V–6V / 1:48 gear ratio)
* **Motor Driver:** L293D IC / L293D Motor Shield or L298N Dual H-Bridge Module
* **Power Supply:**
  * 1x 7.4V Li-ion battery pack (2x 18650 cells) or 4x 1.5V AA battery holder for motors
  * 5V USB / 9V battery for Arduino logic
* **Sensors & Communication Modules:**
  * 1x HC-SR04 Ultrasonic Distance Sensor
  * 2x TCRT5000 Infrared (IR) Line Tracking Sensor Modules
  * 1x HC-05 or HC-06 Bluetooth Serial Module
* **Miscellaneous:** Jumper wires (Male-to-Female, Male-to-Male), breadboard/sensor mounting plate, USB programming cable, chassis on/off switch.

---

# 🔌 GPIO & Pinout Configuration

The standard pin configuration across the NIELIT Robotics series is mapped as follows:

### Motor Driver (L293D / L298N)
| Function | Arduino UNO Pin | Description |
| :--- | :--- | :--- |
| **ENA** | **Pin 9 (PWM)** | Left Motor Speed Control |
| **IN1** | **Pin 5** | Left Motor Direction A |
| **IN2** | **Pin 6** | Left Motor Direction B |
| **IN3** | **Pin 10** | Right Motor Direction A |
| **IN4** | **Pin 11** | Right Motor Direction B |
| **ENB** | **Pin 3 (PWM)** | Right Motor Speed Control |

### Sensors & Wireless Modules
| Module | Module Pin | Arduino UNO Pin |
| :--- | :--- | :--- |
| **HC-SR04 Ultrasonic** | `TRIG` | **Pin 12** |
| **HC-SR04 Ultrasonic** | `ECHO` | **Pin 13** |
| **IR Line Sensor Left** | `OUT` | **Pin 2** |
| **IR Line Sensor Right** | `OUT` | **Pin 4** |
| **HC-05 / HC-06 Bluetooth** | `TXD` | **Pin 12** (Arduino RX via SoftwareSerial) |
| **HC-05 / HC-06 Bluetooth** | `RXD` | **Pin 13** (Arduino TX via 3.3V divider) |

> **Note:** For more detailed pinout diagrams and driver IC schematics, refer to [`extras/Pinouts_and_Wiring_Guide.md`](file:///c:/Users/princ/Desktop/NIELIT-Robotics-Practicals/extras/Pinouts_and_Wiring_Guide.md).

---

# ⚡ Power Distribution & Electrical Safety

```text
               +----------------------------------+
               |     BATTERY PACK (+6V to +12V)   |
               +----------------+-----------------+
                                |
             +------------------+------------------+
             | (+)                                 | (-)
             v                                     v
     +---------------+                     +---------------+
     | L293D / L298N |                     |  COMMON GND   |
     | Motor VCC2/VM |                     |     RAIL      |
     +---------------+                     +-------+-------+
                                                   |
             +-------------------------------------+
             |
             v
     +---------------+
     |  Arduino GND  | <------- 5V Regulated USB Supply
     +---------------+
```

### Safety Principles:
1. **Never Power Motors Directly from Arduino 5V Pin:** DC motors draw significant inductive current spikes and stall currents (up to 1A+) that can damage the onboard Arduino linear voltage regulator or cause immediate MCU brownout resets.
2. **Common Ground Connection:** The negative (`-`) terminal of the motor battery pack **MUST** be firmly tied to the Arduino `GND` pin. Without a common ground reference, logic control signals cannot be recognized by the motor driver.
3. **Inductive Back-EMF Protection:** The L293D IC features internal clamp diodes. If using bare transistors or standard H-bridges without built-in diodes, ensure external flyback diodes (e.g. 1N4007) are installed across motor terminals.

---

# 🕹️ Kinematic Control & Maneuvers

The robotic platform utilizes **Differential Drive Kinematics**:

* **Forward:** Both Left & Right wheels rotate forward at equal velocity ($v_L = v_R > 0$).
* **Reverse:** Both Left & Right wheels rotate backward at equal velocity ($v_L = v_R < 0$).
* **Pivot Turn (Wide Radius):** One wheel remains stationary while the opposing wheel drives forward ($v_L = 0, v_R > 0$ or $v_L > 0, v_R = 0$).
* **Point Spin (Zero Turning Radius):** Left and Right wheels rotate in opposite directions at equal velocity ($v_L = -v_R$).

---

# 📱 Bluetooth Remote Control Protocol

Practical **3.6** utilizes standard wireless character commands compatible with popular Android Arduino RC applications:

| Character | Command Action |
| :---: | :--- |
| `F` | Drive Forward |
| `B` | Drive Backward |
| `L` | Spin Left (Point turn) |
| `R` | Spin Right (Point turn) |
| `G` | Forward Left Curve |
| `I` | Forward Right Curve |
| `H` | Backward Left Curve |
| `J` | Backward Right Curve |
| `S` | Stop All Motors |
| `0` – `9` | Speed Presets (0 = lowest duty cycle, 9 / 'q' = 100% PWM) |

> **Fail-Safe Watchdog:** If the Bluetooth signal drops or the mobile device disconnects while the vehicle is driving, the embedded watchdog timer will automatically stop the robot within **1000 ms** to avoid collisions.

---

# 🧭 Autonomous Navigation Capstones

### Capstone I: Line Tracking (Practical 3.7)
* Uses dual TCRT5000 infrared reflectance sensors mounted under the front bumper.
* Detects contrast between high-absorption dark tape (logic `HIGH`) and reflective white floor (logic `LOW`).
* Continuously evaluates sensor states to apply proactive differential steering corrections.

### Capstone II: Obstacle Avoidance (Practical 3.8)
* Employs the HC-SR04 ultrasonic sensor emitting 40 kHz acoustic pulses.
* Calculates spatial clearance in centimeters: $\text{Distance} = \frac{\Delta t \times 0.0343}{2}$.
* Implements a state machine:
  1. **Clear Path ($> 25\text{ cm}$):** Cruising forward.
  2. **Impediment ($12\text{ cm} - 25\text{ cm}$):** Stop, brief reverse clearance, execute evasive spin turn.
  3. **Critical Hazard ($< 12\text{ cm}$):** Emergency full stop, multi-step reverse and wide-angle escape maneuver.

---

# 📚 NIELIT Library API

The package exposes a helper library API that can be included in custom student projects:

```cpp
#include <NIELIT_Robotics_Practicals.h>

void setup() {
    Serial.begin(9600);

    // Print library version banner
    NIELIT::Robotics::printLibraryInfo(Serial);
    
    // Print standardized practical header
    NIELIT::Robotics::printPracticalHeader(Serial, "3.4", "Kinematic Maneuvers");
}

void loop() {
}
```

---

# 📁 Package Structure

```text
NIELIT-Robotics-Practicals/
├── library.properties
├── keywords.txt
├── INSTALL.txt
├── LICENSE
├── README.md
├── src/
│   ├── NIELIT_Robotics_Practicals.h
│   └── NIELIT_Robotics_Practicals.cpp
├── examples/
│   ├── 3_1_Robotics_Assembly_Power_Distribution/
│   │   └── 3_1_Robotics_Assembly_Power_Distribution.ino
│   ├── 3_2_Inductive_Load_Control_L293D/
│   │   └── 3_2_Inductive_Load_Control_L293D.ino
│   ├── 3_3_DC_Gear_Motor_Actuation/
│   │   └── 3_3_DC_Gear_Motor_Actuation.ino
│   ├── 3_4_Kinematic_Maneuvers_Robocar/
│   │   └── 3_4_Kinematic_Maneuvers_Robocar.ino
│   ├── 3_5_Speed_Vector_PWM_Control/
│   │   └── 3_5_Speed_Vector_PWM_Control.ino
│   ├── 3_6_Wireless_Bluetooth_Kinematic/
│   │   └── 3_6_Wireless_Bluetooth_Kinematic.ino
│   ├── 3_7_Line_Following_Robot/
│   │   └── 3_7_Line_Following_Robot.ino
│   └── 3_8_Obstacle_Avoiding_Vehicle/
│       └── 3_8_Obstacle_Avoiding_Vehicle.ino
└── extras/
    └── Pinouts_and_Wiring_Guide.md
```

---

# 🔧 Troubleshooting

### 1. Motors are rotating in the reverse direction
* **Fix:** Swap the two wire leads connecting the motor terminals to the motor driver output (`OUT1`/`OUT2` or `OUT3`/`OUT4`).

### 2. Robot does not move or motor hums/whines without turning
* **Fix:** The PWM value may be below the static friction threshold of the gearbox. Increase PWM to at least `150`–`180`, or check if the motor battery voltage has dropped.

### 3. Arduino resets or disconnects whenever motors start
* **Fix:** Classic brownout condition caused by drawing motor current through the Arduino 5V regulator. Separate motor power (use dedicated battery) and ensure common ground (`GND`) is connected.

### 4. Bluetooth module does not pair or respond
* **Fix:** Check baud rate (default is `9600`). Verify that Arduino `TX` connects to HC-05 `RX` (via voltage divider) and Arduino `RX` connects to HC-05 `TX`.

### 5. Line sensor does not detect black tape
* **Fix:** Adjust the onboard sensitivity potentiometer on the TCRT5000 module until the onboard indicator LED turns on over white and off over black. Ensure sensor height is 5mm–10mm above the track.

---

# 🏷️ Version

Current release:
```text
v1.0.3
```

---

# 🎓 Educational Purpose

This library is designed for educational laboratory courses in:
* Mobile Robotics & Autonomous Vehicles
* Embedded Systems & Microcontroller Interfacing
* Mechatronics & Actuator Control
* Kinematics & PWM Speed Regulation
* Sensor Integration & Computer-Controlled Navigation

---

# 👥 Authors

* **Dr. Sarwan Singh**
* **Lovnish Verma**

*NIELIT / Embedded Systems, AI & IoT Training*

---

# 📄 License

This project is licensed under the MIT License — see the [LICENSE](file:///c:/Users/princ/Desktop/NIELIT-Robotics-Practicals/LICENSE) file for details.
