/*
  NIELIT Robotics Practicals
  Practical 3.3: Direct Current Actuation — Interfacing DC Gear Motors

  Objective:
  Interface and independently control dual DC yellow BO gear motors on an L293D shield,
  testing forward drive, reverse drive, single-wheel turns, and in-place spins.

  Hardware:
  - Arduino UNO with Blue L293D Motor Shield
  - Left Motor  -> Connected to M1
  - Right Motor -> Connected to M2
  - Battery Pack -> Connected to EXT_PWR (+M and GND)
*/

#include <AFMotor.h>

// Connect Left Motor to M1, Right Motor to M2
AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

// Speed Settings (0 - 255)
const int DRIVE_SPEED = 220; // Driving speed
const int TURN_SPEED  = 200; // Turning speed

// Motor Speed Trim (Adjust if one motor spins faster)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

const int MOVE_TIME_MS  = 2000; // Duration for each move
const int PAUSE_TIME_MS = 800;  // Pause to protect plastic gears

void setup() {
  Serial.begin(9600);

  stopMotors();

  Serial.println("NIELIT Robotics Practical 3.3");
  Serial.println("L293D Shield - DC Gear Motor Movement Tests");
  Serial.println("Starting in 2 seconds...\n");
  delay(2000);
}

void loop() {
  // 1. Both Motors Forward
  Serial.println("[Move 1] Both Motors FORWARD -> Robot drives forward");
  setSpeed(DRIVE_SPEED, DRIVE_SPEED);
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 2. Both Motors Reverse
  Serial.println("[Move 2] Both Motors REVERSE -> Robot drives backward");
  setSpeed(DRIVE_SPEED, DRIVE_SPEED);
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 3. Left Motor Only (Forward)
  Serial.println("[Move 3] Left Motor ONLY Forward -> Robot turns RIGHT");
  setSpeed(TURN_SPEED, 0);
  motorLeft.run(FORWARD);
  motorRight.run(RELEASE);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 4. Right Motor Only (Forward)
  Serial.println("[Move 4] Right Motor ONLY Forward -> Robot turns LEFT");
  setSpeed(0, TURN_SPEED);
  motorLeft.run(RELEASE);
  motorRight.run(FORWARD);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 5. In-Place Spin Left
  Serial.println("[Move 5] In-Place SPIN LEFT (Left Reverse, Right Forward)");
  setSpeed(TURN_SPEED, TURN_SPEED);
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 6. In-Place Spin Right
  Serial.println("[Move 6] In-Place SPIN RIGHT (Left Forward, Right Reverse)");
  setSpeed(TURN_SPEED, TURN_SPEED);
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  delay(MOVE_TIME_MS);
  stopMotors();

  Serial.println("\nMovement sequence complete. Repeating in 4 seconds...\n");
  delay(4000);
}

// Motor Control Helper Functions

void setSpeed(int leftSpd, int rightSpd) {
  int actualLeft  = constrain(leftSpd + LEFT_TRIM, 0, 255);
  int actualRight = constrain(rightSpd + RIGHT_TRIM, 0, 255);
  motorLeft.setSpeed(actualLeft);
  motorRight.setSpeed(actualRight);
}

void stopMotors() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}
