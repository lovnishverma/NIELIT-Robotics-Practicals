# NIELIT Robotics Practicals — Pinout & Wiring Reference Guide

This reference guide details hardware connections, motor driver architectures, sensor pin mappings, and power distribution configurations for the **NIELIT Robotics Practicals** series (3.1 to 3.8).

---

## 1. L293D Dual H-Bridge Motor Driver Pinout

```text
               +---v---+
  (ENA) 1,2EN -| 1   16|- VCC1 (Logic 5V from Arduino)
    (IN1)  1A -| 2   15|- 4A (IN4)
   (OUT1)  1Y -| 3   14|- 4Y (OUT4)
          GND -| 4   13|- GND
          GND -| 5   12|- GND
   (OUT2)  2Y -| 6   11|- 3Y (OUT3)
    (IN2)  2A -| 7   10|- 3A (IN3)
   (VM)  VCC2 -| 8    9|- 3,4EN (ENB)
               +-------+
```

### Driver IC Characteristics:
- **L293D:** Includes internal flyback clamp diodes across all 4 half-H channels to suppress back-EMF spikes.
- **L298N Module:** Standard red breakout boards include external Schottky/rectifier flyback diodes. If using a bare L298N IC, external diodes (e.g. 1N5819 or 1N4007) are required.

---

## 2. Standard Pin Configurations

### Standard 2WD Chassis / Motor & Bluetooth Configuration (Practicals 3.1 - 3.6, 3.8)
| Driver Function | L293D IC Pin | Arduino UNO Pin | Purpose | Notes |
| :--- | :---: | :---: | :--- | :--- |
| **ENA** | Pin 1 | **D5** (PWM) | Left Motor Speed Control | Timer0 (~976 Hz) |
| **IN1** | Pin 2 | **D2** | Left Motor Direction Input 1 | Digital Output |
| **IN2** | Pin 7 | **D3** | Left Motor Direction Input 2 | Digital Output |
| **IN3** | Pin 10 | **D4** | Right Motor Direction Input 1 | Digital Output |
| **IN4** | Pin 15 | **D7** | Right Motor Direction Input 2 | Digital Output |
| **ENB** | Pin 9 | **D6** (PWM) | Right Motor Speed Control | Timer0 (~976 Hz) |
| **VCC1** | Pin 16 | **5V** | Logic Power from Arduino | Regulated 5V Rail |
| **VCC2 / VM** | Pin 8 | **Battery (+)** | Motor Power Supply | **6.0V – 7.4V Recommended** |
| **GND** | Pins 4, 5, 12, 13 | **GND** | Common Ground Busbar | Common with Battery (-) |

---

### Line Follower Configuration (Practical 3.7)
*Avoids pin conflict because D2 and D3 are dedicated to the digital IR Line Sensors:*
| Function | Arduino UNO Pin | Description | Notes |
| :--- | :---: | :--- | :--- |
| **LEFT_SENSOR** | **D2** | Digital Line Detection (Left TCRT5000) | Digital Input |
| **RIGHT_SENSOR** | **D3** | Digital Line Detection (Right TCRT5000) | Digital Input |
| **ENA** | **D5** (PWM) | Left Motor Speed Control | Timer0 (~976 Hz) |
| **IN1** | **D8** | Left Motor Direction Input 1 | Digital Output |
| **IN2** | **D9** | Left Motor Direction Input 2 | Digital Output |
| **ENB** | **D6** (PWM) | Right Motor Speed Control | Timer0 (~976 Hz) |
| **IN3** | **D10** | Right Motor Direction Input 1 | Digital Output |
| **IN4** | **D11** | Right Motor Direction Input 2 | Digital Output |

---

## 3. Sensor & Module Pin Assignments

### HC-SR04 Ultrasonic Distance Sensor (Practical 3.8)
| HC-SR04 Pin | Arduino Pin | Description | Operating Voltage |
| :--- | :---: | :--- | :---: |
| **VCC** | 5V | 5V Power Supply Rail | 5V DC |
| **TRIG** | **D9** | Ultrasonic 10us Trigger Pulse Output | 5V TTL |
| **ECHO** | **D10** | Echo Return Pulse Time Input | 5V TTL |
| **GND** | GND | Common Ground | 0V |

### TCRT5000 IR Line Tracking Sensors (Practical 3.7)
| Sensor Module Pin | Arduino Pin | Description | Operating Voltage |
| :--- | :---: | :--- | :---: |
| **Left IR (OUT)** | **D2** (`LEFT_SENSOR`) | Digital Line Detection (Left) | 5V TTL |
| **Right IR (OUT)**| **D3** (`RIGHT_SENSOR`)| Digital Line Detection (Right)| 5V TTL |
| **VCC** | 5V | Power Supply Rail | 5V DC |
| **GND** | GND | Common Ground | 0V |

### HC-05 / HC-06 Bluetooth Breakout Module (Practical 3.6)
| Bluetooth Module Pin | Arduino Pin | Description | Notes |
| :--- | :---: | :--- | :--- |
| **VCC** | 5V | Power Supply (Breakout onboard LDO) | 5V DC |
| **GND** | GND | Common Ground | 0V |
| **TXD** | **D12** (RX) | SoftwareSerial Receive from Bluetooth | 3.3V TTL out to Arduino |
| **RXD** | **D13** (TX) | SoftwareSerial Transmit to Bluetooth | **Requires 1k/2k Resistor Divider to 3.3V** |

---

## 4. Power Distribution Architecture

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

> **Critical Safety Rule**: Always connect the negative terminal of the motor battery pack directly to the Arduino `GND` pin. Never operate the motor driver without a common ground reference. Never connect 12V or 3S LiPo packs directly to 3V–6V rated BO motors.
