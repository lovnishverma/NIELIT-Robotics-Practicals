/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.4: Kinematic Maneuvers — 2-Wheel Robocar Basic Movements
  =========================================================

  Objective:
  Learn differential drive steering kinematics for a 2-wheel mobile robot
  by executing fundamental motion patterns (Square navigation, Pivot turns, and Point spins).

  ---------------------------------------------------------
  Differential Drive Steering Explained:
  ---------------------------------------------------------
  1. Forward: Both Left and Right wheels turn forward at equal speed.
  2. Backward: Both Left and Right wheels turn backward at equal speed.
  3. Pivot Turn: One wheel spins forward while the other wheel is stopped.
     -> The robot swings in a wide circular curve around the stationary wheel.
  4. Point Spin (In-Place 360°): Left wheel reverses while Right wheel drives forward.
     -> The robot spins on the spot with zero turning radius!

  The 3rd wheel (metal ball caster) simply rolls along to keep the robot upright.

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

#include <NIELIT_Robotics_Practicals.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

const int PIN_ENA = 5;
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;
const int PIN_ENB = 6;
const int PIN_IN3 = 4;
const int PIN_IN4 = 7;

// =====================================================
// SPEED & TIMING CALIBRATION
// =====================================================

const int CRUISE_SPEED = 200; // Driving speed (0 - 255)
const int TURN_SPEED   = 180; // Turning speed (0 - 255)

// Motor Speed Trim (Increase/decrease if car drifts to one side)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// Timing durations (in milliseconds)
const int STRAIGHT_TIME_MS = 2000; // Time to drive one side of the square
const int TURN_TIME_90DEG  = 600;  // Time needed for a 90-degree in-place turn (adjust for your floor)
const int PAUSE_TIME_MS    = 500;  // Pause between maneuvers

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void moveForward(int speed, int durationMs);
void moveBackward(int speed, int durationMs);
void pivotLeft(int speed, int durationMs);
void pivotRight(int speed, int durationMs);
void spinLeft(int speed, int durationMs);
void spinRight(int speed, int durationMs);
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  NIELIT::Robotics::printPracticalHeader(Serial, "3.4", "Differential Drive Kinematic Maneuvers");
  Serial.println(F("[INFO] Place robot on an open, flat floor."));
  Serial.println(F("[INFO] Starting navigation maneuvers in 3 seconds...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP: Executes Square Path followed by Curve Demos
// =====================================================

void loop() {
  Serial.println(F("--- STARTING 4-SIDED SQUARE DEMO ---"));

  // Drive in a square (4 sides: Straight -> 90-degree Right Turn)
  for (int side = 1; side <= 4; side++) {
    Serial.print(F("[SQUARE] Driving Side "));
    Serial.println(side);
    moveForward(CRUISE_SPEED, STRAIGHT_TIME_MS);
    stopRobot();
    delay(PAUSE_TIME_MS);

    Serial.println(F("[SQUARE] Executing 90-degree Right Point Spin"));
    spinRight(TURN_SPEED, TURN_TIME_90DEG);
    stopRobot();
    delay(PAUSE_TIME_MS);
  }

  Serial.println(F("\n--- DEMONSTRATING WIDE PIVOT TURNS ---"));

  // Wide Pivot Turn Left
  Serial.println(F("[PIVOT] Wide Pivot Turn LEFT (Left wheel stopped, Right wheel forward)"));
  pivotLeft(TURN_SPEED, 1500);
  stopRobot();
  delay(PAUSE_TIME_MS);

  // Wide Pivot Turn Right
  Serial.println(F("[PIVOT] Wide Pivot Turn RIGHT (Left wheel forward, Right wheel stopped)"));
  pivotRight(TURN_SPEED, 1500);
  stopRobot();
  delay(PAUSE_TIME_MS);

  // Straight Reverse
  Serial.println(F("[REVERSE] Driving Straight Backward"));
  moveBackward(CRUISE_SPEED, 1500);
  stopRobot();

  Serial.println(F("\n[INFO] Kinematic demonstration complete. Pausing 5 seconds before repeating...\n"));
  delay(5000);
}

// =====================================================
// KINEMATIC MOTION PRIMITIVES
// =====================================================

void moveForward(int speed, int durationMs) {
  int actualLeft  = constrain(speed + LEFT_TRIM, 0, 255);
  int actualRight = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, actualLeft);
  analogWrite(PIN_ENB, actualRight);

  delay(durationMs);
}

void moveBackward(int speed, int durationMs) {
  int actualLeft  = constrain(speed + LEFT_TRIM, 0, 255);
  int actualRight = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, actualLeft);
  analogWrite(PIN_ENB, actualRight);

  delay(durationMs);
}

void pivotLeft(int speed, int durationMs) {
  int actualRight = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, actualRight);

  delay(durationMs);
}

void pivotRight(int speed, int durationMs) {
  int actualLeft = constrain(speed + LEFT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, actualLeft);
  analogWrite(PIN_ENB, 0);

  delay(durationMs);
}

void spinLeft(int speed, int durationMs) {
  int actualLeft  = constrain(speed + LEFT_TRIM, 0, 255);
  int actualRight = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, actualLeft);
  analogWrite(PIN_ENB, actualRight);

  delay(durationMs);
}

void spinRight(int speed, int durationMs) {
  int actualLeft  = constrain(speed + LEFT_TRIM, 0, 255);
  int actualRight = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, actualLeft);
  analogWrite(PIN_ENB, actualRight);

  delay(durationMs);
}

void stopRobot() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}
