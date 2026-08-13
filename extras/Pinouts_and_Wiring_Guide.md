# NIELIT Robotics Practicals — Pinout & Wiring Reference Guide

This reference guide details hardware connections, motor driver architectures, sensor pin mappings, and power distribution configurations for the **NIELIT Robotics Practicals** series (3.1 to 3.8).

---

## 1. L293D Dual H-Bridge Motor Driver Pinout

```text
               +---v---+
  (ENA) 1,2EN -| 1   16|- VCC1 (Logic 5V)
    (IN1)  1A -| 2   15|- 4A (IN4)
   (OUT1)  1Y -| 3   14|- 4Y (OUT4)
          GND -| 4   13|- GND
          GND -| 5   12|- GND
   (OUT2)  2Y -| 6   11|- 3Y (OUT3)
    (IN2)  2A -| 7   10|- 3A (IN3)
   (VM)  VCC2 -| 8    9|- 3,4EN (ENB)
               +-------+
```

---

## 2. Standard Pin Configurations

### Standard 2WD Chassis / Motor & Bluetooth Configuration (Practicals 3.1 - 3.6, 3.8)
| Driver Function | L293D IC Pin | Arduino UNO Pin | Purpose |
|-----------------|--------------|-----------------|---------|
| **ENA** | Pin 1 | **D5** (PWM) | Left Motor Speed Control |
| **IN1** | Pin 2 | **D2** | Left Motor Direction A |
| **IN2** | Pin 7 | **D3** | Left Motor Direction B |
| **IN3** | Pin 10 | **D4** | Right Motor Direction A |
| **IN4** | Pin 15 | **D7** | Right Motor Direction B |
| **ENB** | Pin 9 | **D6** (PWM) | Right Motor Speed Control |
| **VCC1** | Pin 16 | **5V** | Logic Power from Arduino |
| **VCC2 / VM** | Pin 8 | **Battery (+)** | Motor Power (+6V to +12V) |
| **GND** | Pins 4, 5, 12, 13 | **GND** | Common Ground (Arduino + Battery) |

### Line Follower Configuration (Practical 3.7)
*Avoids pin conflict since D2 and D3 are dedicated to IR Line Sensors:*
| Function | Arduino UNO Pin | Description |
| :--- | :--- | :--- |
| **LEFT_SENSOR** | **D2** | Digital Line Detection (Left IR) |
| **RIGHT_SENSOR** | **D3** | Digital Line Detection (Right IR) |
| **ENA** | **D5 (PWM)** | Left Motor Speed |
| **IN1** | **D8** | Left Motor Direction A |
| **IN2** | **D9** | Left Motor Direction B |
| **ENB** | **D6 (PWM)** | Right Motor Speed |
| **IN3** | **D10** | Right Motor Direction A |
| **IN4** | **D11** | Right Motor Direction B |

---

## 3. Sensor & Module Pin Assignments

### HC-SR04 Ultrasonic Distance Sensor (Practical 3.8)
| HC-SR04 Pin | Arduino Pin | Description |
|-------------|-------------|-------------|
| **VCC** | 5V | 5V Power Rail |
| **TRIG** | **D9** | Ultrasonic trigger pulse output |
| **ECHO** | **D10** | Echo return pulse input |
| **GND** | GND | Common Ground |

### IR Line Tracking Sensors (TCRT5000) (Practical 3.7)
| Sensor Module | Arduino Pin | Description |
|---------------|-------------|-------------|
| **Left IR (OUT)** | **D2** (`LEFT_SENSOR`) | Digital Line Detection (Left) |
| **Right IR (OUT)** | **D3** (`RIGHT_SENSOR`) | Digital Line Detection (Right) |
| **VCC** | 5V | Power Supply |
| **GND** | GND | Common Ground |

### HC-05 / HC-06 Bluetooth Module (Practical 3.6)
| Bluetooth Module | Arduino Pin | Notes |
|------------------|-------------|-------|
| **VCC** | 5V | 5V Power Supply |
| **GND** | GND | Common Ground |
| **TXD** | **D12** (RX) | SoftwareSerial Receive |
| **RXD** | **D13** (TX) | SoftwareSerial Transmit (via 3.3V divider) |

---

## 4. Power Distribution Architecture

```text
    +-----------------------------------------------+
    |            BATTERY PACK (6V - 12V)            |
    +-----------------------+-----------------------+
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
   |  Arduino GND  | <------- USB 5V or Vin Step-down
   +---------------+
```

> **Critical Rule**: Always connect the negative terminal of the motor battery pack directly to the Arduino `GND` pin. Never operate the motor driver without a common ground reference.
