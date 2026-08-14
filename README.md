# NIELIT Robotics Practicals Library

**Version:** `1.1.0`  
**Target Architecture:** `avr` (Arduino UNO R3 / Nano / ATmega328P)  
**Curriculum Body:** National Institute of Electronics & Information Technology (NIELIT Ropar)

An educational Arduino library and beginner-friendly curriculum repository packaging the **NIELIT Robotics Practical Programs (3.1 to 3.8)** as installable, standalone Arduino Examples.

Designed for students, educators, and robotics enthusiasts learning mobile robotics, differential drive kinematics, motor driver electronics, wireless control, and autonomous sensor navigation.

---

## 🚗 Understanding Your 2-Wheel Differential Drive Robot

Many beginners wonder: *"Why does the 3rd middle wheel have no motor?"* or *"Why is my car veering or spinning in circles?"*

```text
                  [ FRONT / SENSOR SIDE ]
                            ▲
                            │
   [Left BO Motor] ─────────┴───────── [Right BO Motor]
   (Rubber Drive Tire)                 (Rubber Drive Tire)
                            │
                            │
                     ( BALL CASTER )
                 [ REAR / BALANCE SIDE ]
```

### 1. How Steering Works (Differential Drive)
Unlike a standard car or tricycle with a steering wheel/fork, this robot uses **Differential Steering** (just like a tank, wheelchair, or robot vacuum cleaner):
* **Forward:** Both Left and Right motors spin forward at equal speed.
* **Backward:** Both Left and Right motors spin backward at equal speed.
* **Pivot Turn (Wide Curve):** One motor spins forward while the other is stopped.
* **Point Spin (360° on the Spot):** Left motor spins reverse while Right motor spins forward (spins in place with zero turning radius).

### 2. The Role of the 3rd Middle Wheel (Ball Caster)
* The metal ball caster is **unpowered and rolls freely in any direction**.
* Its only job is to act as a **3rd balance support** to keep the robot upright.

### 3. Top Beginner Troubleshooting Tips
| Problem | Cause | Easy Solution |
| :--- | :--- | :--- |
| **Car spins in circles or won't move** | Caster wheel height is uneven; rubber wheels are floating | Place car on a flat table. Adjust brass spacers so **both rubber wheels and the caster touch the floor firmly**. |
| **Car veers/drifts to one side** | BO motors have small manufacturing speed differences | Adjust `LEFT_TRIM` or `RIGHT_TRIM` in the code (e.g. `LEFT_TRIM = +15`). |
| **Wheels spin backwards** | DC motor wires connected in reverse polarity | Swap the two wires of that motor at the motor driver screw terminal. |
| **Arduino resets when motors start** | Motors powered from Arduino 5V pin | Always use a separate 6V–7.4V battery pack for motors with a shared GND. |

---

## 📑 Robotics Practicals Curriculum

| Practical | Title & Focus | What You Will Learn |
| :--- | :--- | :--- |
| **3.1** | **Robotics Assembly & Power Dynamics** | 2WD chassis assembly, battery power wiring, common ground, and pre-flight rotation diagnostics. |
| **3.2** | **Motor Driver H-Bridge Control (L293D / L298N)** | How H-Bridge switches current for Forward, Reverse, Active Braking, and Coasting Stop. |
| **3.3** | **DC Gear Motor Actuation** | Independent control of Left and Right BO motors to execute 6 fundamental movement primitives. |
| **3.4** | **Kinematic Maneuvers — Differential Steering** | Navigating in a square, wide pivot turns, and zero-radius 360° point spins. |
| **3.5** | **Speed Control via PWM & Smooth Ramps** | Regulating speed with PWM (0–255), speed presets, smooth acceleration ramps, and trim tuning. |
| **3.6** | **Wireless Smartphone Bluetooth Control** | Remote control via HC-05/HC-06 Bluetooth module with safety auto-stop. |
| **3.7** | **Autonomous Line-Following Robot** | Closed-loop optical tracking using dual TCRT5000 IR reflectance sensors. |
| **3.8** | **Autonomous Obstacle-Avoiding Robot** | Collision avoidance using HC-SR04 ultrasonic distance sensor with automatic reverse & turn. |

---

## 🚀 Quick Start Guide for Students

### Step 1 — Open Arduino IDE & Set Board
> **Tools → Board → Arduino AVR Boards → Arduino Uno**  
> **Tools → Port → [Select Connected COM Port]**

### Step 2 — Install Library
1. Download or clone this repository as a `.zip` archive.
2. In Arduino IDE:
   > **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP file.

### Step 3 — Open Any Practical Example
Navigate to:
> **File → Examples → NIELIT Robotics Practicals → [Select Practical 3.1 to 3.8]**

### Step 4 — Verify, Upload & Open Serial Monitor
1. Click **Verify** (✔) then **Upload** (➔).
2. Open **Serial Monitor** (**Tools → Serial Monitor**) and set baud rate to **9600 baud**.

---

## 🔌 Standard Wiring Reference (L298N Module)

```text
  Driver Pin   Arduino UNO Pin   Function
  -------------------------------------------------------------
  ENA          Pin 5 (PWM)       Left Motor Speed Control
  IN1          Pin 2             Left Motor Direction 1
  IN2          Pin 3             Left Motor Direction 2
  IN3          Pin 4             Right Motor Direction 1
  IN4          Pin 7             Right Motor Direction 2
  ENB          Pin 6 (PWM)       Right Motor Speed Control
  VCC / 5V     5V                Arduino 5V Power
  GND          GND & Batt (-)    Common Ground (Mandatory!)
  VM / 12V     Battery (+)       Motor Battery Rail (6.0V - 7.4V)
  -------------------------------------------------------------
```

> **Using the Blue L293D Motor Shield?** See the full wiring details in [`extras/Pinouts_and_Wiring_Guide.md`](extras/Pinouts_and_Wiring_Guide.md).

---

## 🛠 Hardware Bill of Materials (BOM)

* **Microcontroller:** Arduino UNO R3 (ATmega328P)
* **Chassis Kit:** 2WD Smart Robot Car Kit (2 yellow BO gear motors + 1 metal ball caster wheel)
* **Motor Driver:** L298N Module or L293D Driver
* **Power Supply:** 2x 18650 Li-ion Batteries (7.4V) or 4x AA Battery Holder (6.0V)
* **Sensors:**
  * 2x TCRT5000 IR Reflective Sensor Modules (Practical 3.7)
  * 1x HC-SR04 Ultrasonic Distance Sensor (Practical 3.8)
  * 1x HC-05 / HC-06 Bluetooth Module (Practical 3.6)

---

## 📂 Repository Structure

```text
NIELIT-Robotics-Practicals/
├── library.properties                  # Arduino Library Manager metadata
├── keywords.txt                        # Syntax highlighting definitions
├── INSTALL.txt                         # Installation instructions
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
    ├── Hardware_BOM.md                 # Detailed component list
    ├── Pinouts_and_Wiring_Guide.md     # Wiring and pinout diagrams
    └── Validation_Matrix.md            # Hardware & verification checklist
```
