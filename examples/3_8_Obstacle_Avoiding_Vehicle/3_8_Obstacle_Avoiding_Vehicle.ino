/*
  NIELIT Robotics Practicals
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle

  Objective:
  Build an autonomous collision-avoiding robot using an HC-SR04 ultrasonic
  distance sensor and the L293D Motor Shield.

  Wiring on L293D Motor Shield:
  - HC-SR04 TRIG  -> Analog Pin A0
  - HC-SR04 ECHO  -> Analog Pin A1
  - HC-SR04 Power -> 5V / GND on shield's analog breakout header
  - Left Motor    -> Screw Terminal M1
  - Right Motor   -> Screw Terminal M2
  - Battery Pack  -> EXT_PWR (+M and GND) on shield
*/

#include <AFMotor.h>

// Connect Left Motor to M1, Right Motor to M2
AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

// Ultrasonic Sensor Pins (connected to Analog header on shield)
const int PIN_TRIG = A0;
const int PIN_ECHO = A1;

// Distance and Speed Settings
const int OBSTACLE_DISTANCE_CM = 25;  // Obstacle threshold in cm
const int CRUISE_SPEED         = 180; // Forward driving speed (0 - 255)
const int TURN_SPEED           = 180; // Avoidance turn speed (0 - 255)

// Motor Speed Trim
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  // Configure sensor pins
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.8");
  Serial.println("L293D Shield - Obstacle-Avoiding Robot");
  Serial.println("Connect TRIG to A0, ECHO to A1.");
  Serial.println("Starting in 3 seconds...\n");
  delay(3000);
}

void loop() {
  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If path is clear, drive forward
  if (distance > OBSTACLE_DISTANCE_CM || distance == 0) {
    moveForward(CRUISE_SPEED);
  } else {
    // Obstacle detected within 25 cm!
    Serial.println("[Alert] Obstacle detected! Backing up and turning...");

    // Step 1: Stop
    stopRobot();
    delay(200);

    // Step 2: Reverse slightly
    moveBackward(CRUISE_SPEED, 400);
    stopRobot();
    delay(200);

    // Step 3: Spin turn right
    spinTurn(TURN_SPEED, 500, true);
    stopRobot();
    delay(200);
  }

  delay(60); // Small interval between readings
}

// Ultrasonic Sensor Reading Helper

long readDistanceCM() {
  // Clear trigger pin
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  // Send 10us trigger pulse
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read return echo duration (timeout after 25ms)
  long duration = pulseIn(PIN_ECHO, HIGH, 25000);

  if (duration == 0) {
    return 999; // Out of range or no echo received
  }

  // Convert travel time to distance in centimeters (speed of sound = 343 m/s)
  long distanceCm = duration / 58;
  return distanceCm;
}

// Motor Control Helper Functions

void moveForward(int speed) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}

void moveBackward(int speed, int durationMs) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  delay(durationMs);
}

void spinTurn(int speed, int durationMs, bool turnRight) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));

  if (turnRight) {
    motorLeft.run(FORWARD);
    motorRight.run(BACKWARD);
  } else {
    motorLeft.run(BACKWARD);
    motorRight.run(FORWARD);
  }

  delay(durationMs);
}

void stopRobot() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}
