/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.3: Direct Current Actuation — Interfacing DC Gear Motors
  =========================================================

  Objective:
  Interface and independently control dual DC yellow BO gear motors on a 2WD robot chassis,
  testing forward thrust, reverse pull, single-motor turns, and axial spins.

  ---------------------------------------------------------
  Understanding BO Gear Motors & Chassis Movements:
  ---------------------------------------------------------
  Yellow BO motors have internal plastic gears (1:48 ratio) that increase torque to rotate wheels.
  - Forward: Both motors spin forward.
  - Reverse: Both motors spin backward.
  - Pivot Turn: Run one motor while stopping the other (wide curve).
  - Point Spin: Run one motor forward and one motor backward (spins in place).

  ---------------------------------------------------------
  Pin Connections (L298N / Standard Motor Driver):
  ---------------------------------------------------------
  Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  Motor Power:  6.0V - 7.4V Battery Pack (+ to VM/12V, - to GND)
  Arduino GND:  Common Ground connected to Battery (-)

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor
const int PIN_ENA = 5; // Left Speed PWM
const int PIN_IN1 = 2; // Left Dir 1
const int PIN_IN2 = 3; // Left Dir 2

// Right Motor
const int PIN_ENB = 6; // Right Speed PWM
const int PIN_IN3 = 4; // Right Dir 1
const int PIN_IN4 = 7; // Right Dir 2

// =====================================================
// SPEED & CALIBRATION CONSTANTS
// =====================================================

const int DRIVE_SPEED = 220; // Normal driving speed (0 - 255)
const int TURN_SPEED  = 200; // Turning & spinning speed (0 - 255)

// Speed Trim: Adjust if one motor is faster than the other
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// Timing constants (in milliseconds)
const int MOVE_TIME_MS  = 2000; // Time for each movement
const int PAUSE_TIME_MS = 800;  // Soft pause to protect gears

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void setLeftMotor(bool forward, int speed);
void setRightMotor(bool forward, int speed);
void stopMotors();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  // Set motor pins as outputs
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  // Safe startup: ensure motors are stopped
  stopMotors();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Robotics Practical 3.3"));
  Serial.println(F(" DC Gear Motor Actuation"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] Starting dual-motor movement sequence in 2 seconds...\n"));
  delay(2000);
}

// =====================================================
// MAIN LOOP: Cycles through 6 basic motor movements
// =====================================================

void loop() {
  // 1. Both Motors Forward
  Serial.println(F("[ACTION 1] Both Motors FORWARD -> Robot drives forward"));
  setLeftMotor(true, DRIVE_SPEED);
  setRightMotor(true, DRIVE_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 2. Both Motors Reverse
  Serial.println(F("[ACTION 2] Both Motors REVERSE -> Robot drives backward"));
  setLeftMotor(false, DRIVE_SPEED);
  setRightMotor(false, DRIVE_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 3. Left Motor Only (Forward)
  Serial.println(F("[ACTION 3] Left Motor ONLY Forward -> Robot pivots RIGHT"));
  setLeftMotor(true, TURN_SPEED);
  setRightMotor(true, 0);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 4. Right Motor Only (Forward)
  Serial.println(F("[ACTION 4] Right Motor ONLY Forward -> Robot pivots LEFT"));
  setLeftMotor(true, 0);
  setRightMotor(true, TURN_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 5. Spin Left on the Spot (Left Reverse, Right Forward)
  Serial.println(F("[ACTION 5] In-Place SPIN LEFT (Left Reverse, Right Forward)"));
  setLeftMotor(false, TURN_SPEED);
  setRightMotor(true, TURN_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 6. Spin Right on the Spot (Left Forward, Right Reverse)
  Serial.println(F("[ACTION 6] In-Place SPIN RIGHT (Left Forward, Right Reverse)"));
  setLeftMotor(true, TURN_SPEED);
  setRightMotor(false, TURN_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();

  Serial.println(F("\n[INFO] Movement cycle completed. Pausing 4 seconds before repeating...\n"));
  delay(4000);
}

// =====================================================
// MOTOR CONTROL PRIMITIVES
// =====================================================

void setLeftMotor(bool forward, int speed) {
  int actualSpeed = constrain(speed + LEFT_TRIM, 0, 255);
  if (actualSpeed == 0) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, 0);
  } else {
    digitalWrite(PIN_IN1, forward ? HIGH : LOW);
    digitalWrite(PIN_IN2, forward ? LOW : HIGH);
    analogWrite(PIN_ENA, actualSpeed);
  }
}

void setRightMotor(bool forward, int speed) {
  int actualSpeed = constrain(speed + RIGHT_TRIM, 0, 255);
  if (actualSpeed == 0) {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, 0);
  } else {
    digitalWrite(PIN_IN3, forward ? HIGH : LOW);
    digitalWrite(PIN_IN4, forward ? LOW : HIGH);
    analogWrite(PIN_ENB, actualSpeed);
  }
}

void stopMotors() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}
