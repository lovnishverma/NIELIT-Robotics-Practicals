# NIELIT Robotics Practicals — Pinout & Wiring Reference Guide

A complete beginner-friendly hardware and wiring reference for the **L293D Motor Driver Shield for Arduino** across the **NIELIT Robotics Practicals** series (3.1 to 3.8).

---

## 1. L293D Motor Driver Shield Overview

```text
               +-------------------------------------------+
               |  [SERVOS]      ARDUINO UNO HEADERS        |
               |  (9, 10)                                  |
               |                                           |
  Motor Left   | [o]                                   [o] | Motor Right
  Terminal M1  | [o]      [IC1]       [IC2]            [o] | Terminal M2
               | [o]      L293D       L293D            [o] |
               | [o]                                   [o] |
               | [o]             [IC3]                 [o] |
               |                74HC595                    |
               |                                           |
               |  [+]  [-]      [PWR]       [ 5V GND A0-5] | Sensor Headers
               |  EXT_PWR       Jumper      Analog Row     | (A0, A1, A2...)
               +---+----+---------------------+------------+
                   |    |                     |
                   |    +---------------------┴--> Battery Negative (-) & Common GND
                   +-----------------------------> Battery Positive (+) [6.0V - 7.4V]
```

### Key Shield Connections:
1. **Left Motor:** Wire directly to blue screw terminal **M1** (Top-Left).
2. **Right Motor:** Wire directly to blue screw terminal **M2** (or M3/M4).
3. **Battery Power (6.0V - 7.4V):** Wire battery pack (+ and -) to the **EXT_PWR (+M and GND)** screw terminal.
4. **Power Jumper (`PWR`):** Keep the yellow/green jumper installed to power both the Arduino and shield from the battery pack.

---

## 2. Sensor Wiring to the Shield (Analog Headers)

The L293D Shield provides dedicated 5V, GND, and Analog pin headers (**A0 to A5**) along the bottom-right edge:

### Practical 3.6: HC-05 / HC-06 Bluetooth Module
```text
  HC-05 Pin     Shield Header Pin       Description
  -------------------------------------------------------------
  VCC           5V (Analog Header)      5V Power Supply
  GND           GND (Analog Header)     Common Ground
  TXD           A0                      SoftwareSerial RX (Receives from phone)
  RXD           A1                      SoftwareSerial TX (Transmits to phone via 1k/2k divider)
  -------------------------------------------------------------
```

### Practical 3.7: Dual TCRT5000 IR Line Tracking Sensors
```text
  Sensor Pin             Shield Header Pin   Description
  -------------------------------------------------------------
  Left IR Sensor (OUT)   A0                  Digital Input (Left line detect)
  Right IR Sensor (OUT)  A1                  Digital Input (Right line detect)
  Sensor VCC / GND       5V / GND            5V Logic Power
  -------------------------------------------------------------
```

### Practical 3.8: HC-SR04 Ultrasonic Distance Sensor
```text
  HC-SR04 Pin   Shield Header Pin       Description
  -------------------------------------------------------------
  VCC           5V (Analog Header)      5V Power Supply
  GND           GND (Analog Header)     Common Ground
  TRIG          A0                      10us Ultrasonic Trigger Output
  ECHO          A1                      Echo Pulse Return Input
  -------------------------------------------------------------
```

---

## 3. Top Troubleshooting Guide for Beginners

1. **Wheel Rotates Backwards:**
   * Simply swap the two motor wires in the blue screw terminal (**M1** for Left, **M2** for Right).
2. **Car Veers Left or Right:**
   * Adjust `LEFT_TRIM` or `RIGHT_TRIM` at the top of the sketch (e.g. `LEFT_TRIM = +15`).
3. **Robot Jitters or Stalls:**
   * Check that the 2x 18650 battery cells are charged (>7.0V total).
   * Ensure the **PWR Jumper** is placed on the shield.
