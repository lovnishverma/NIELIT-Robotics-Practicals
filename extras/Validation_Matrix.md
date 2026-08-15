# NIELIT Robotics Practicals — Verification & Validation Matrix

This document provides an honest, publication-grade record of the verification status, automated compiler checks, safety models, and remaining laboratory hardware validation requirements for the **NIELIT Robotics Practicals (3.1 to 3.8)**.

---

## 1. Practical Verification Matrix

| Practical | Focus / Title | Static Code Review | Compiler Check (`arduino:avr:uno`) | Safety & Failure Handling | Physical Hardware Verification | Status |
| :---: | :--- | :---: | :---: | :---: | :---: | :---: |
| **3.1** | **Robotics Assembly & Power Distribution Dynamics** | Verified | **PASS (0 Errors)** | Pre-flight 4-phase polarity check; safe power-down boot state | Requires bench rotation check to confirm motor terminal polarity | **COMPILE VERIFIED (Requires HW Check)** |
| **3.2** | **Inductive Load Control — L293D** | Verified | **PASS (0 Errors)** | Active dynamic braking verified; flyback clamp diode distinction documented | Requires oscilloscope / current probe check of back-EMF suppression | **COMPILE VERIFIED (Requires HW Check)** |
| **3.3** | **DC Gear Motor Actuation** | Verified | **PASS (0 Errors)** | Soft pauses between opposing thrusts to protect gearbox gears | Requires bench testing of motor torque under mechanical load | **COMPILE VERIFIED (Requires HW Check)** |
| **3.4** | **Kinematic Maneuvers — Robocar** | Verified | **PASS (0 Errors)** | Directional lines established before PWM; open-loop limits disclosed | Requires physical calibration of `TURN_TIME_MS` for 90° turn on lab floor | **COMPILE VERIFIED (Requires HW Check)** |
| **3.5** | **Speed Vector / PWM Control** | Verified | **PASS (0 Errors)** | Soft-start acceleration & deceleration ramps reduce inrush current | Requires physical straight-line calibration of `LEFT/RIGHT_MOTOR_TRIM` | **COMPILE VERIFIED (Requires HW Check)** |
| **3.6** | **Wireless Bluetooth Kinematic** | Verified | **PASS (0 Errors)** | 1500ms communication timeout auto-stop; 3.3V RX level shifter documented | Requires RF pairing and distance range test with physical HC-05 module | **COMPILE VERIFIED (Requires HW Check)** |
| **3.7** | **Line Following Robot** | Verified | **PASS (0 Errors)** | Smooth discrete differential steering (no motor reversing); polarity toggle | Requires optical threshold tuning of TCRT5000 potentiometers on physical line | **COMPILE VERIFIED (Requires HW Check)** |
| **3.8** | **Obstacle Avoiding Vehicle** | Verified | **PASS (0 Errors)** | Safe obstacle distance threshold evasion (25cm); out-of-range timeout safety handling | Requires physical obstacle detection testing on soft/angled surfaces | **COMPILE VERIFIED (Requires HW Check)** |

---

## 2. Verification Methodology

### A. Statically & Compiler-Verified Elements
1. **Compilation Integrity:** All 8 `.ino` example sketches compile cleanly with 0 errors and 0 warnings using `arduino-cli` with target core `arduino:avr:uno`.
2. **Pin Assignment Consistency:** No GPIO pin contention across any example. Digital pins 2 and 3 are appropriately reassigned to IR sensors in Practical 3.7 with motor controls shifted to pins 8, 9, 10, 11.
3. **Timer Allocation:** PWM speed enable pins (ENA=D5, ENB=D6) are mapped to hardware Timer0 (~976.56 Hz).
4. **Safety & Bounds Clamping:** All PWM variables are bounded via `constrain(speed, 0, 255)` to prevent buffer overflows or unintended roll-over.
5. **Defensive Failure Handling:**
   - Practical 3.8 enforces out-of-range timeout return (999 cm) and immediate evasive maneuvers (stop $\rightarrow$ backup $\rightarrow$ spin turn) when obstacles are $\le 25\text{ cm}$.
   - Practical 3.6 enforces a 1500ms communication watchdog timeout auto-stop if wireless Bluetooth packets cease while moving.

---

### B. Requirements for Physical Laboratory Hardware Validation

The following items cannot be validated by software compilation alone and must be verified on physical hardware in the laboratory:

1. **Motor Wiring Polarity:** Because motors may be wired differently depending on assembly, students must perform a bench test with wheels elevated off the surface to ensure logical forward matches physical forward motion.
2. **Floor Friction & Turn Calibration:** Open-loop turn durations (e.g. `TURN_TIME_MS = 600` in Practical 3.4) depend on axle track width, wheel diameter, tire traction, and battery voltage.
3. **IR Sensor Comparator Thresholds:** TCRT5000 onboard potentiometers must be physically adjusted with a screwdriver to ensure clean digital logic transitions between the black electrical tape and the floor surface.
4. **Ultrasonic Acoustic Geometry:** HC-SR04 ultrasonic sound waves reflect differently off soft fabrics or angled surfaces. Students should verify detection on rigid perpendicular test obstacles.
5. **Battery Discharge Voltage:** Battery packs should be monitored with a multimeter to ensure voltage does not drop below 6.0V under motor load.
