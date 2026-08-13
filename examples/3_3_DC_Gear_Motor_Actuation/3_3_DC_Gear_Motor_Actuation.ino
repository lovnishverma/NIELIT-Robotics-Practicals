/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.3: Direct Current Actuation — Interfacing DC Gear Motors
  =========================================================

  Objective:
  Interface and independently actuate dual direct current (DC) gear motors through an H-bridge driver,
  evaluating directional thrust, torque multiplication, and differential rotation.

  Description:
  Demonstrates bi-directional control of left and right DC gear motors (BO motors) mounted on a mobile chassis.
  Executes six distinct actuation maneuvers: Dual Forward Thrust, Dual Reverse Pull, Single Left Drive,
  Single Right Drive, Axial Spin Left, and Axial Spin Right.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors (3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Platform
  - External Motor Power Supply (6V - 12V Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Driver / Component Pin   Arduino UNO Pin   Function
  -------------------------------------------------------------
  ENA                      Pin 5 (PWM)       Left Motor Speed Enable
  IN1                      Pin 2             Left Motor Direction Input 1
  IN2                      Pin 3             Left Motor Direction Input 2
  IN3                      Pin 4             Right Motor Direction Input 1
  IN4                      Pin 7             Right Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor Speed Enable
  OUT1, OUT2               Terminals         Left DC Gear Motor
  OUT3, OUT4               Terminals         Right DC Gear Motor
  VCC1                     5V                Arduino 5V (Logic Supply)
  VCC2 / VM                Battery (+)       Motor Power (+6V to +12V)
  GND                      GND & Batt (-)    Common Ground Busbar
  -------------------------------------------------------------

  Working Principle:
  BO gear motors combine a high-speed DC armature with a reduction gearbox (typically 1:48).
  This gear reduction trades rotational speed for output torque, enabling the wheels to overcome
  static friction and drive the vehicle chassis. Independent dual-channel H-bridges allow the
  microcontroller to spin each motor in either direction or vary its speed via PWM.

  Expected Behavior:
  1. Maneuver 1: Both motors drive Forward at full thrust (2.5s).
  2. Maneuver 2: Both motors drive Reverse at full pull (2.5s).
  3. Maneuver 3: Left motor only drives Forward (2.0s), pivoting the chassis to the right.
  4. Maneuver 4: Right motor only drives Forward (2.0s), pivoting the chassis to the left.
  5. Maneuver 5: Left motor reverses while Right motor drives forward (Axial Spin Left, 2.0s).
  6. Maneuver 6: Left motor drives forward while Right motor reverses (Axial Spin Right, 2.0s).
  7. Clear telemetry messages are displayed on the Serial Monitor at 9600 baud.

  Notes:
  - A brief 800ms soft pause is introduced between maneuvers to prevent inductive current surges
    and mechanical stress on the plastic gear teeth.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6
#define IN3 4
#define IN4 7

// =====================================================
// SPEED & TIMING CONSTANTS
// =====================================================

const int DRIVE_SPEED = 240;
const int PIVOT_SPEED = 220;
const int SPIN_SPEED  = 200;

const int DRIVE_TIME_MS = 2500;
const int PIVOT_TIME_MS = 2000;
const int SPIN_TIME_MS  = 2000;
const int PAUSE_TIME_MS = 800;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void setMotorLeft(bool forward, int speed);
void setMotorRight(bool forward, int speed);
void stopMotors();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Safe initialization
  stopMotors();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.3: DC Gear Motor Actuation    "));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.println(F("[INFO] Commencing dual-channel motor actuation test sequence in 2s...\n"));
  delay(2000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // 1. Both Motors Forward
  Serial.println(F("[ACTION 1] Both Motors Driving FORWARD (Forward Thrust)"));
  setMotorLeft(true, DRIVE_SPEED);
  setMotorRight(true, DRIVE_SPEED);
  delay(DRIVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 2. Both Motors Reverse
  Serial.println(F("[ACTION 2] Both Motors Driving REVERSE (Reverse Pull)"));
  setMotorLeft(false, DRIVE_SPEED);
  setMotorRight(false, DRIVE_SPEED);
  delay(DRIVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 3. Left Motor Only (Forward)
  Serial.println(F("[ACTION 3] Left Motor ONLY (Forward) -> Chassis pivots Right"));
  setMotorLeft(true, PIVOT_SPEED);
  setMotorRight(false, 0);
  delay(PIVOT_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 4. Right Motor Only (Forward)
  Serial.println(F("[ACTION 4] Right Motor ONLY (Forward) -> Chassis pivots Left"));
  setMotorLeft(false, 0);
  setMotorRight(true, PIVOT_SPEED);
  delay(PIVOT_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 5. Axial Spin Left
  Serial.println(F("[ACTION 5] Axial SPIN LEFT (Counter-rotating wheels, Zero radius)"));
  setMotorLeft(false, SPIN_SPEED);
  setMotorRight(true, SPIN_SPEED);
  delay(SPIN_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 6. Axial Spin Right
  Serial.println(F("[ACTION 6] Axial SPIN RIGHT (Counter-rotating wheels, Zero radius)"));
  setMotorLeft(true, SPIN_SPEED);
  setMotorRight(false, SPIN_SPEED);
  delay(SPIN_TIME_MS);
  stopMotors();

  Serial.println(F("\n[INFO] Actuation test suite completed. Pausing 4 seconds...\n"));
  delay(4000);
}

// =====================================================
// MOTOR CONTROL PRIMITIVES
// =====================================================

void setMotorLeft(bool forward, int speed) {
  if (speed <= 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  } else {
    if (forward) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
    analogWrite(ENA, constrain(speed, 0, 255));
  }
}

void setMotorRight(bool forward, int speed) {
  if (speed <= 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  } else {
    if (forward) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    analogWrite(ENB, constrain(speed, 0, 255));
  }
}

void stopMotors() {
  setMotorLeft(true, 0);
  setMotorRight(true, 0);
}
