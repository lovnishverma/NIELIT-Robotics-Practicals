/*
  NIELIT Robotics Practicals
  Practical 3.4: Kinematic Maneuvers — 2-Wheel Robocar Basic Movements

  Objective:
  Learn differential drive steering on a 2-wheel mobile robot
  by executing fundamental motion patterns (Square navigation, Pivot turns, and Point spins).

  Wiring (Arduino to L298N Motor Driver):
  - Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  - Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  - Motor Power:  6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
  - Common GND:   Arduino GND connected to Battery (-)
*/

// Pin Definitions
const int PIN_ENA = 5;
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;
const int PIN_ENB = 6;
const int PIN_IN3 = 4;
const int PIN_IN4 = 7;

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

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.4");
  Serial.println("Differential Drive Kinematic Maneuvers");
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
