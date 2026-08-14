/*
  NIELIT Robotics Practicals
  Practical 3.4: Kinematic Maneuvers — 2-Wheel Robocar Basic Movements

  Objective:
  Learn differential drive steering on an L293D shield robot
  by executing fundamental motion patterns (Square navigation, Pivot turns, and Point spins).

  Hardware:
  - Arduino UNO with Blue L293D Motor Shield
  - Left Motor  -> M1
  - Right Motor -> M2
  - Battery Pack -> EXT_PWR (+M and GND)
*/

#include <AFMotor.h>

AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

// Speed and Timing Settings
const int CRUISE_SPEED = 200; // Driving speed (0 - 255)
const int TURN_SPEED   = 180; // Turning speed (0 - 255)

// Motor Speed Trim (Adjust if robot drifts left or right)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

const int STRAIGHT_TIME_MS = 2000; // Time to drive each side of the square
const int TURN_TIME_90DEG  = 600;  // Time for 90-degree turn (adjust for your floor)
const int PAUSE_TIME_MS    = 500;  // Pause between maneuvers

void setup() {
  Serial.begin(9600);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.4");
  Serial.println("L293D Shield - Kinematic Navigation Maneuvers");
  Serial.println("Place robot on a flat open floor.");
  Serial.println("Starting in 3 seconds...\n");
  delay(3000);
}

void loop() {
  Serial.println("--- Starting 4-Sided Square Demo ---");

  // Drive in a square (4 straight lines + 4 right turns)
  for (int side = 1; side <= 4; side++) {
    Serial.print("[Square] Driving Side ");
    Serial.println(side);
    moveForward(CRUISE_SPEED, STRAIGHT_TIME_MS);
    stopRobot();
    delay(PAUSE_TIME_MS);

    Serial.println("[Square] Turning 90 degrees Right");
    spinRight(TURN_SPEED, TURN_TIME_90DEG);
    stopRobot();
    delay(PAUSE_TIME_MS);
  }

  Serial.println("\n--- Demonstrating Wide Pivot Turns ---");

  // Wide Pivot Turn Left
  Serial.println("[Pivot] Wide Turn Left (Left stopped, Right forward)");
  pivotLeft(TURN_SPEED, 1500);
  stopRobot();
  delay(PAUSE_TIME_MS);

  // Wide Pivot Turn Right
  Serial.println("[Pivot] Wide Turn Right (Left forward, Right stopped)");
  pivotRight(TURN_SPEED, 1500);
  stopRobot();
  delay(PAUSE_TIME_MS);

  // Straight Reverse
  Serial.println("[Reverse] Driving Straight Backward");
  moveBackward(CRUISE_SPEED, 1500);
  stopRobot();

  Serial.println("\nManeuvers complete. Repeating in 5 seconds...\n");
  delay(5000);
}

// Movement Helper Functions

void moveForward(int speed, int durationMs) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  delay(durationMs);
}

void moveBackward(int speed, int durationMs) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  delay(durationMs);
}

void pivotLeft(int speed, int durationMs) {
  motorLeft.run(RELEASE);
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorRight.run(FORWARD);
  delay(durationMs);
}

void pivotRight(int speed, int durationMs) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(RELEASE);
  delay(durationMs);
}

void spinLeft(int speed, int durationMs) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  delay(durationMs);
}

void spinRight(int speed, int durationMs) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  delay(durationMs);
}

void stopRobot() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}
