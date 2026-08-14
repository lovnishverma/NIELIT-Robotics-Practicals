/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.2: Inductive Load Control — Interfacing the Motor Driver (L293D / L298N)
  =========================================================

  Objective:
  Understand how an H-Bridge motor driver controls the direction and speed of a DC motor
  using control logic (Forward, Reverse, Active Brake, Coasting Stop, and PWM Speed).

  ---------------------------------------------------------
  How an H-Bridge Motor Driver Works:
  ---------------------------------------------------------
  A microcontroller (Arduino) output pin can only supply ~20mA of current at 5V, but a DC motor
  needs 300mA - 1000mA of current. The motor driver acts as an electronic switch (H-Bridge)
  that takes low-power control signals from the Arduino to control high-power battery current to the motor.

  H-Bridge Truth Table (Left Motor Example):
  +-----+-----+-----+-------------------+------------------------------------------+
  | ENA | IN1 | IN2 | Motor Action      | What happens electrically                |
  +-----+-----+-----+-------------------+------------------------------------------+
  | PWM | HIGH| LOW | Forward Spin      | Current flows Terminal A -> Terminal B   |
  | PWM | LOW | HIGH| Reverse Spin      | Current flows Terminal B -> Terminal A   |
  | PWM | LOW | LOW | Active Brake      | Both terminals grounded (stops quickly)  |
  |  0  |  X  |  X  | Coasting Stop     | Power cut off (motor spins down slowly)  |
  +-----+-----+-----+-------------------+------------------------------------------+

  ---------------------------------------------------------
  Pin Connections (Arduino UNO to Driver):
  ---------------------------------------------------------
  ENA (Speed PWM) -> Pin 5
  IN1 (Direction 1) -> Pin 2
  IN2 (Direction 2) -> Pin 3
  VCC / 5V Logic   -> Arduino 5V
  GND / Ground     -> Arduino GND & Battery (-)
  VM / Motor Power -> Battery (+) [6.0V - 7.4V]

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

#include <NIELIT_Robotics_Practicals.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

const int PIN_ENA = 5; // Speed control via PWM (0 - 255)
const int PIN_IN1 = 2; // Direction Pin 1
const int PIN_IN2 = 3; // Direction Pin 2

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  // Initialize motor in stopped state
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 0);

  NIELIT::Robotics::printPracticalHeader(Serial, "3.2", "Motor Driver H-Bridge Control");
  Serial.println(F("[INFO] Starting H-Bridge demonstration in 2 seconds...\n"));
  delay(2000);
}

// =====================================================
// MAIN LOOP: Cycles through all H-Bridge states
// =====================================================

void loop() {
  // STATE 1: Forward Rotation at Full Speed
  Serial.println(F("[STATE 1] FORWARD Rotation (IN1 = HIGH, IN2 = LOW, ENA = 255)"));
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 255);
  delay(3000);

  // STATE 2: Active Dynamic Brake (Instant Halt)
  Serial.println(F("[STATE 2] ACTIVE BRAKE (IN1 = LOW, IN2 = LOW, ENA = 255) -> Halts quickly"));
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 255);
  delay(1500);

  // STATE 3: Reverse Rotation at Full Speed
  Serial.println(F("[STATE 3] REVERSE Rotation (IN1 = LOW, IN2 = HIGH, ENA = 255)"));
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENA, 255);
  delay(3000);

  // STATE 4: Coasting Stop (Natural Inertial Spin-down)
  Serial.println(F("[STATE 4] COASTING STOP (ENA = 0) -> Motor power disconnected"));
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 0);
  delay(2000);

  // STATE 5: Speed Control (PWM Demonstration)
  Serial.println(F("[STATE 5] PWM SPEED CONTROL (Forward at 50% Speed: ENA = 128)"));
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 128);
  delay(3000);

  // Stop motor
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);

  Serial.println(F("\n[INFO] H-Bridge demonstration completed. Pausing 4 seconds before repeating...\n"));
  delay(4000);
}
