# NIELIT Robotics Practicals — Hardware Bill of Materials (BOM)

This document provides a comprehensive hardware specification and Bill of Materials for conducting the **NIELIT Robotics Practicals (3.1 to 3.8)**.

---

## 1. Core Hardware Components

| Item | Component Description | Quantity | Operating Voltage | Key Specifications | Relevant Practicals |
| :---: | :--- | :---: | :---: | :--- | :---: |
| **1** | **Arduino UNO R3** (or compatible AVR board) | 1 | 5V (USB) / 7V-12V (VIN) | ATmega328P MCU, 16 MHz, 32 KB Flash, 2 KB SRAM, 6 PWM channels | All (3.1 – 3.8) |
| **2** | **2WD Robotic Chassis Kit** | 1 | N/A | Acrylic chassis plate, 2x rubber drive wheels, 1x omnidirectional ball caster | All (3.1 – 3.8) |
| **3** | **DC Yellow BO Gear Motors** | 2 | 3.0V – 6.0V DC (Nominal) | 1:48 gear reduction, plastic spur gearbox, ~200 RPM @ 6V, ~800mA stall current | All (3.1 – 3.8) |
| **4** | **L293D Motor Driver IC / Shield** (or L298N Module) | 1 | Logic: 5V<br>Motor: 6.0V – 7.4V | Dual H-Bridge, 600mA continuous per channel, internal back-EMF clamp diodes | All (3.1 – 3.8) |
| **5** | **HC-SR04 Ultrasonic Distance Sensor** | 1 | 5V DC | 40 kHz ultrasonic transceiver, 2cm – 400cm range, 15° measuring angle | 3.8 |
| **6** | **TCRT5000 IR Line Tracking Sensor Modules** | 2 | 3.3V – 5.0V DC | Infrared phototransistor pair, onboard LM393 comparator, sensitivity potentiometer | 3.7 |
| **7** | **HC-05 / HC-06 Bluetooth Serial Breakout Board** | 1 | VCC: 5V (onboard 3.3V LDO)<br>Logic: 3.3V TTL | 2.4 GHz Bluetooth v2.0+EDR, default 9600 baud UART, requires 3.3V RX divider | 3.6 |
| **8** | **Voltage Divider Resistors** | 1 set | N/A | 1x 1kΩ resistor, 1x 2kΩ resistor (for 5V to 3.3V RX level shifting on Bluetooth) | 3.6 |
| **9** | **Motor Battery Pack Holder** | 1 | 6.0V – 7.4V | 4x 1.5V AA battery holder (6.0V) or 2S Li-ion battery holder (7.4V nominal) | All (3.1 – 3.8) |
| **10** | **Premium Jumper Wires** | 1 pack | N/A | Assorted Male-to-Male, Male-to-Female, Female-to-Female jumper wires | All (3.1 – 3.8) |
| **11** | **Black Electrical Tape** | 1 roll | N/A | 15mm – 19mm width for creating contrasting line follower tracks | 3.7 |

---

## 2. Power Supply Domain Architecture

```text
+-------------------------------------------------------------------------+
|                         POWER SUPPLY DOMAINS                            |
+-------------------------------------------------------------------------+

[LOGIC DOMAIN]                                   [MOTOR DOMAIN]
Arduino UNO 5V Rail                              Dedicated Battery Pack
- Powered via USB Cable or 5V Regulator          - 4x AA Alkaline (6.0V) or 2S Li-ion (7.4V)
- Powers ATmega328P MCU                          - Powers Motor Driver VCC2 / VM pin
- Powers HC-SR04 VCC (5V)                        - Powers DC Gear Motors
- Powers TCRT5000 IR Modules (5V)                - Drives High Inductive Stall Currents
- Powers HC-05 Breakout VCC (5V)
       |                                                |
       +-----------------------+------------------------+
                               |
                               v
                     [COMMON GROUND BUSBAR]
                     Arduino GND tied to Battery (-)
```

---

## 3. Electrical Ratings & Critical Safety Rules

1. **Motor Voltage Scoping:** DC yellow BO motors are designed for 3V to 6V. With an L293D driver saturation drop of ~1.4V to 1.8V ($V_{CE,sat}$), a **6.0V to 7.4V** battery pack yields approximately 4.5V to 5.6V directly across the motor coils. Never connect 12V or 3S LiPo packs directly to 3V-rated BO motors.
2. **Current Isolation:** Never draw DC motor power from the Arduino 5V header pin. Motor stall current (>800mA per motor) exceeds the current capacity of the onboard 5V linear regulator and will cause brownout resets.
3. **Common Ground Bonding:** The negative terminal of the motor battery pack **MUST** connect directly to an Arduino `GND` pin to maintain a shared 0V reference for TTL control logic.
4. **Bluetooth RX Level Shifting:** The HC-05 RXD pin is 3.3V logic tolerant. Always install the 1kΩ / 2kΩ resistor divider between Arduino A1 (TX) and HC-05 RXD.
