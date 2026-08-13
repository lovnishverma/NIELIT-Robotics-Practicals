/*
  Practical 3.4: Kinematic Maneuvers — 2-Wheel Robocar Basic Movements
  Course: NIELIT Robotics Practicals

  Description:
  Implements fundamental kinematic motion primitives for a 2-wheel differential
  drive mobile robot (Robocar). Demonstrates modular navigation routines:
  Forward, Backward, Pivot Turn Left/Right, Point/Spin Turn, and Stop.
  Executes an autonomous geometric square navigation drill.

  Differential Drive Kinematics:
  - Forward:      Left Motor (CW),  Right Motor (CW)
  - Backward:     Left Motor (CCW), Right Motor (CCW)
  - Pivot Left:   Left Motor (OFF), Right Motor (CW)
  - Pivot Right:  Left Motor (CW),  Right Motor (OFF)
  - Spin Left:    Left Motor (CCW), Right Motor (CW)  (Zero-radius turn)
  - Spin Right:   Left Motor (CW),  Right Motor (CCW) (Zero-radius turn)

  Hardware Connections:
  -------------------------------------------------------------
  L293D / Driver Pin    Arduino Pin      Description
  -------------------------------------------------------------
  ENA                   Pin 5 (PWM)      Left Motor Speed
  IN1                   Pin 2            Left Motor Input 1
  IN2                   Pin 3            Left Motor Input 2
  IN3                   Pin 4            Right Motor Input 1
  IN4                   Pin 7            Right Motor Input 2
  ENB                   Pin 6 (PWM)      Right Motor Speed
  VCC2 (VM)             +6V to +12V      Battery Pack Positive (+)
  GND                   GND              Common Ground
  -------------------------------------------------------------
*/

// Left Motor Pins
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Pins
#define ENB 6
#define IN3 4
#define IN4 7

// Default cruise and turn speeds (0 - 255)
const int CRUISE_SPEED = 200;
const int TURN_SPEED   = 180;

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot(500);

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.4: Robocar Kinematic Maneuvers      "));
  Serial.println(F("=================================================="));
  Serial.println(F("Starting navigation drill in 3 seconds...\n"));
  delay(3000);
}

void loop() {
  Serial.println(F("--- Starting Square Navigation Drill ---"));

  // Traverse a 4-sided square path
  for (int side = 1; side <= 4; side++) {
    Serial.print(F("Traversing Side "));
    Serial.println(side);
    
    // 1. Move straight
    moveForward(CRUISE_SPEED, 2000);
    stopRobot(500);

    // 2. 90-degree right turn (tune duration based on chassis/floor friction)
    Serial.println(F("Executing 90-degree Point Turn Right..."));
    spinRight(TURN_SPEED, 600);
    stopRobot(500);
  }

  Serial.println(F("Square path completed!\n"));
  delay(2000);

  // Additional Kinematic Demonstration: S-Curve maneuvers
  Serial.println(F("--- Demonstrating Pivot Turns ---"));
  Serial.println(F("Pivot Turn Left (Wide curve)..."));
  turnLeft(CRUISE_SPEED, 1500);
  stopRobot(500);

  Serial.println(F("Pivot Turn Right (Wide curve)..."));
  turnRight(CRUISE_SPEED, 1500);
  stopRobot(500);

  Serial.println(F("Reversing back to start..."));
  moveBackward(CRUISE_SPEED, 2000);
  stopRobot(3000);
}

// -------------------------------------------------------------
// Kinematic Movement Primitives
// -------------------------------------------------------------

void moveForward(int speed, int durationMs) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  if (durationMs > 0) delay(durationMs);
}

void moveBackward(int speed, int durationMs) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  if (durationMs > 0) delay(durationMs);
}

void turnLeft(int speed, int durationMs) {
  // Left motor stopped, right motor drives forward
  analogWrite(ENA, 0);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  if (durationMs > 0) delay(durationMs);
}

void turnRight(int speed, int durationMs) {
  // Left motor drives forward, right motor stopped
  analogWrite(ENA, speed);
  analogWrite(ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  if (durationMs > 0) delay(durationMs);
}

void spinLeft(int speed, int durationMs) {
  // Zero-radius left spin: Left reverses, Right drives forward
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  if (durationMs > 0) delay(durationMs);
}

void spinRight(int speed, int durationMs) {
  // Zero-radius right spin: Left drives forward, Right reverses
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  if (durationMs > 0) delay(durationMs);
}

void stopRobot(int durationMs) {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  if (durationMs > 0) delay(durationMs);
}
