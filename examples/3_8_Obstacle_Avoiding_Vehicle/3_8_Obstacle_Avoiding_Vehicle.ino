/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle
  =========================================================

  Objective:
  Build an autonomous collision-avoiding robot using an HC-SR04 ultrasonic
  distance sensor to detect obstacles ahead and automatically steer around them.

  ---------------------------------------------------------
  How the HC-SR04 Ultrasonic Sensor Works:
  ---------------------------------------------------------
  The sensor sends high-frequency sound waves (40 kHz) and listens for the echo:
  1. Arduino sends a 10-microsecond HIGH pulse on the TRIG pin.
  2. The sensor emits 8 ultrasonic sound bursts.
  3. The sound bounces off an obstacle and returns to the sensor.
  4. The ECHO pin stays HIGH for the time it took the sound wave to travel round-trip.
  5. Distance (cm) = (Echo Time in microseconds * 0.0343) / 2  [or Time / 58.2]

  ---------------------------------------------------------
  Collision Avoidance Logic:
  ---------------------------------------------------------
  - Clear Path (Distance > 25 cm): Cruise Forward.
  - Obstacle Ahead (Distance <= 25 cm):
      Step 1: Stop immediately.
      Step 2: Reverse backward for 400ms to gain clearance.
      Step 3: Spin Right for 500ms to face an open direction.
      Step 4: Resume forward cruising.

  ---------------------------------------------------------
  Pin Connections (Ultrasonic Robot Configuration):
  ---------------------------------------------------------
  HC-SR04 VCC       -> Arduino 5V
  HC-SR04 GND       -> Arduino GND
  HC-SR04 TRIG      -> Arduino Pin 9
  HC-SR04 ECHO      -> Arduino Pin 10
  Left Motor ENA    -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  Right Motor ENB   -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  Motor Power       -> 6.0V - 7.4V Battery Pack (+ to VM/12V, - to GND)

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

#include <NIELIT_Robotics_Practicals.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Ultrasonic Sensor Pins
const int PIN_TRIG = 9;
const int PIN_ECHO = 10;

// Motor Driver Pins
const int PIN_ENA = 5;
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;
const int PIN_ENB = 6;
const int PIN_IN3 = 4;
const int PIN_IN4 = 7;

// =====================================================
// DISTANCE & SPEED SETTINGS
// =====================================================

const int OBSTACLE_DISTANCE_CM = 25;  // Obstacle detection threshold (cm)
const int CRUISE_SPEED         = 180; // Forward speed (0 - 255)
const int TURN_SPEED           = 180; // Turning speed (0 - 255)

// Motor Speed Trim
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

long readDistanceCM();
void moveForward(int speed);
void moveBackward(int speed, int durationMs);
void spinTurn(int speed, int durationMs, bool turnRight);
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  // Configure Ultrasonic Pins
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Configure Motor Pins
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  NIELIT::Robotics::printPracticalHeader(Serial, "3.8", "Autonomous Obstacle-Avoiding Robot");
  Serial.println(F("[INFO] Testing ultrasonic sensor..."));

  long initialDist = readDistanceCM();
  Serial.print(F("[INFO] Current Distance: "));
  Serial.print(initialDist);
  Serial.println(F(" cm"));

  Serial.println(F("[INFO] Starting autonomous navigation in 3 seconds...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP: Read Distance -> Decide -> Move
// =====================================================

void loop() {
  long distance = readDistanceCM();

  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));

  // Check if distance is valid and path is clear
  if (distance > OBSTACLE_DISTANCE_CM || distance == 0) {
    // Path is clear -> Drive forward
    moveForward(CRUISE_SPEED);
  } else {
    // Obstacle detected within 25 cm!
    Serial.println(F("[ALERT] Obstacle Detected! Executing Avoidance Maneuver..."));

    // 1. Stop
    stopRobot();
    delay(200);

    // 2. Reverse slightly
    Serial.println(F(" -> Backing up..."));
    moveBackward(CRUISE_SPEED, 400);
    stopRobot();
    delay(200);

    // 3. Spin Turn to clear obstacle
    Serial.println(F(" -> Turning right to find open path..."));
    spinTurn(TURN_SPEED, 500, true);
    stopRobot();
    delay(200);
  }

  delay(60); // Small interval between distance checks
}

// =====================================================
// SENSOR READING FUNCTION
// =====================================================

long readDistanceCM() {
  // Clear the trigger pin
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  // Send 10us HIGH pulse
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read the echo return pulse (timeout after 25,000us = ~4 meters)
  long duration = pulseIn(PIN_ECHO, HIGH, 25000);

  if (duration == 0) {
    return 999; // No echo received (open space / out of range)
  }

  // Convert travel time to distance in centimeters (speed of sound = 343 m/s)
  long distanceCm = duration / 58;
  return distanceCm;
}

// =====================================================
// MOTOR CONTROL PRIMITIVES
// =====================================================

void moveForward(int speed) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
}

void moveBackward(int speed, int durationMs) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);

  delay(durationMs);
}

void spinTurn(int speed, int durationMs, bool turnRight) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  if (turnRight) {
    // Left Forward, Right Reverse
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  } else {
    // Left Reverse, Right Forward
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  }

  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);

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
