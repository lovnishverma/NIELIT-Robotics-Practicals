# NIELIT Robotics Practicals — Pinout & Wiring Reference Guide

A beginner-friendly hardware and wiring reference for the **NIELIT Robotics Practicals** series (3.1 to 3.8).

---

## 1. Understanding Your 2WD Mobile Robot Platform

```text
                  [ Front / Sensor Side ]
                            ▲
                            │
   [Left BO Motor] ─────────┴───────── [Right BO Motor]
   (Rubber Tire)                        (Rubber Tire)
                            │
                            │
                     ( BALL CASTER )
                 [ Rear / Balance Side ]
```

### Why Does the 3rd Middle Wheel Have No Motor?
* **Differential Drive Kinematics:** This robot steers using the relative speed and direction of its two motorized drive wheels.
* **The Ball Caster Wheel:** It is an unpowered, free-rolling swivel/ball that acts as a **3rd balance point** to keep the chassis level.
* **Important Assembly Rule (Leveling):** Ensure both yellow rubber wheels and the caster wheel touch the floor with equal firm pressure. If the caster standoff is too tall, the rubber wheels will float and spin freely in the air!

---

## 2. Motor Driver Wiring Guide

### Option A: Standard L298N Motor Driver Module (Recommended for Direct Pin Control)

```text
               +-----------------------------+
               |        L298N MODULE         |
               |                             |
  Motor A OUT1 | [o]                     [o] | Motor B OUT3
  Motor A OUT2 | [o]                     [o] | Motor B OUT4
               |                             |
               |  [+]   [-]   [+5V]          |
               |  12V   GND    5V            |
               +---+-----+-----+-------------+
                   |     |     |
                   |     |     +-----> Arduino 5V
                   |     +-----------> Arduino GND & Battery (-)
                   +-----------------> Motor Battery (+) [6V - 7.4V]

  Control Signal Header to Arduino:
  -------------------------------------------------------------
  L298N Pin    Arduino UNO Pin   Function
  -------------------------------------------------------------
  ENA          Pin 5 (PWM)       Left Motor Speed Control
  IN1          Pin 2             Left Motor Direction 1
  IN2          Pin 3             Left Motor Direction 2
  IN3          Pin 4             Right Motor Direction 1
  IN4          Pin 7             Right Motor Direction 2
  ENB          Pin 6 (PWM)       Right Motor Speed Control
  -------------------------------------------------------------
```

---

### Option B: Blue L293D Motor Driver Shield (Adafruit v1 Style)

If you have the blue shield that plugs directly onto the Arduino UNO:
* **Left Motor:** Wire to screw terminal **M1** (Top-Left) or **M2**.
* **Right Motor:** Wire to screw terminal **M3** (Bottom-Left) or **M4**.
* **External Power:** Connect your 2x 18650 Battery Pack (+ and -) to the **EXT_PWR (+M and GND)** screw terminal.
* **PWR Jumper:** Leave the yellow `PWR` jumper ON if you want the battery to power both Arduino and motors.
* *Note:* This shield uses an onboard 74HC595 shift register. For custom shield code, install the `Adafruit Motor Shield library (v1)` in Arduino IDE.

---

## 3. Sensor Wiring Reference

### HC-SR04 Ultrasonic Distance Sensor (Practical 3.8)
```text
  HC-SR04 Pin   Arduino Pin   Function
  -------------------------------------------------------------
  VCC           5V            5V Power Rail
  TRIG          Pin 9         Ultrasonic Trigger Pulse (10us)
  ECHO          Pin 10        Echo Return Pulse Timing
  GND           GND           Common Ground
  -------------------------------------------------------------
```

### TCRT5000 IR Line Tracking Sensors (Practical 3.7)
*Avoids pin conflicts by shifting motor direction lines to Pins 8–11:*
```text
  Sensor / Driver Pin    Arduino Pin   Description
  -------------------------------------------------------------
  Left IR Sensor (OUT)   Pin 2         Digital Line Input (Left)
  Right IR Sensor (OUT)  Pin 3         Digital Line Input (Right)
  Sensor VCC / GND       5V / GND      5V Logic Power
  Left Motor ENA (PWM)   Pin 5         Left Speed PWM
  Left Motor IN1 / IN2   Pin 8, Pin 9  Left Motor Direction
  Right Motor ENB (PWM)  Pin 6         Right Speed PWM
  Right Motor IN3 / IN4  Pin 10, Pin 11 Right Motor Direction
  -------------------------------------------------------------
```

### HC-05 / HC-06 Bluetooth Module (Practical 3.6)
```text
  HC-05 Pin     Arduino Pin   Description
  -------------------------------------------------------------
  VCC           5V            Power supply (5V)
  GND           GND           Common Ground
  TXD           Pin 12 (RX)   Connects directly to Arduino D12
  RXD           Pin 13 (TX)   Connects via 1k/2k Resistor Divider
  -------------------------------------------------------------

  Voltage Divider for 3.3V RX Pin Protection:
  Arduino D13 ---[ 1k Ohm ]---+---> HC-05 RXD
                              |
                           [ 2k Ohm ]
                              |
                         Arduino GND
```

---

## 4. Power & Safety Best Practices

1. **Dedicated Motor Battery:** Always use a 6.0V – 7.4V battery pack (e.g. 4x AA or 2x 18650 Li-ion). Never power DC motors from Arduino 5V.
2. **Common Ground:** Connect the battery negative (`-`) to Arduino `GND`. Without a common ground, logic signals cannot be recognized.
3. **Weight Distribution:** Mount your battery pack centrally between the drive wheels so rubber tires maintain firm traction on the floor.
