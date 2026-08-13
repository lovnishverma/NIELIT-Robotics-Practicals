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

### Standard Arduino UNO Mapping:
| Driver Function | L293D IC Pin | Arduino UNO Pin | Purpose |
|-----------------|--------------|-----------------|---------|
| **ENA** | Pin 1 | **D9** (PWM) | Left Motor Speed Control |
| **IN1** | Pin 2 | **D5** | Left Motor Direction A |
| **IN2** | Pin 7 | **D6** | Left Motor Direction B |
| **IN3** | Pin 10 | **D10** | Right Motor Direction A |
| **IN4** | Pin 15 | **D11** | Right Motor Direction B |
| **ENB** | Pin 9 | **D3** (PWM) | Right Motor Speed Control |
| **VCC1** | Pin 16 | **5V** | Logic Power from Arduino |
| **VCC2 / VM** | Pin 8 | **Battery (+)** | Motor Power (+6V to +12V) |
| **GND** | Pins 4, 5, 12, 13 | **GND** | Common Ground (Arduino + Battery) |

---

## 2. Sensor & Module Pin Assignments

### HC-SR04 Ultrasonic Distance Sensor (Practical 3.8)
| HC-SR04 Pin | Arduino Pin | Description |
|-------------|-------------|-------------|
| **VCC** | 5V | 5V Power |
| **TRIG** | **D12** | Ultrasonic trigger pulse output |
| **ECHO** | **D13** | Echo return pulse input |
| **GND** | GND | Ground |

### IR Line Tracking Sensors (TCRT5000) (Practical 3.7)
| Sensor Module | Arduino Pin | Description |
|---------------|-------------|-------------|
| **Left IR (OUT)** | **D2** | Digital Line Detection (Left) |
| **Right IR (OUT)** | **D4** | Digital Line Detection (Right) |
| **VCC** | 5V | Power Supply |
| **GND** | GND | Common Ground |

### HC-05 / HC-06 Bluetooth Module (Practical 3.6)
| Bluetooth Module | Arduino Pin | Notes |
|------------------|-------------|-------|
| **VCC** | 5V | 5V Power Supply |
| **GND** | GND | Common Ground |
| **TXD** | **D12** (RX) | SoftwareSerial Receive |
| **RXD** | **D13** (TX) | SoftwareSerial Transmit (Use 1k/2k divider for 3.3V) |

---

## 3. Power Distribution Architecture

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
