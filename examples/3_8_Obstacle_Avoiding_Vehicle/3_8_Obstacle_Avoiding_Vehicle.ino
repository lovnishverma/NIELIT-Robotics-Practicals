/*
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle
  Course: NIELIT Robotics Practicals

  Description:
  Implements an intelligent autonomous obstacle-avoiding robotic vehicle
  utilizing an HC-SR04 ultrasonic distance sensor with multi-sample filtering.
  Continuously monitors forward clearance, detects impediments within a safety
  threshold, and executes automated evasive maneuvers (Stop -> Reverse -> Spin Turn -> Resume)
  with adaptive deadlock prevention.

  Tinkercad Simulation:
  https://www.tinkercad.com/things/1BEzwkis74q-interafacing-obstacle-using-ultrasonic-sensor

  Distance Calculation Formula:
  Distance (cm) = (Echo_Duration_us * 0.0343) / 2

  Hardware Connections:
  -------------------------------------------------------------
  HC-SR04 Pin           Arduino Pin      Description
  -------------------------------------------------------------
  VCC                   5V               5V Power Rail
  GND                   GND              Common Ground
  TRIG                  Pin 9            Trigger Pulse (Output)
  ECHO                  Pin 10           Echo Time (Input)
  -------------------------------------------------------------
  Motor Driver Pin      Arduino Pin      Description
  -------------------------------------------------------------
  ENA                   Pin 5 (PWM)      Left Motor Speed
  IN1                   Pin 2            Left Motor Dir A
  IN2                   Pin 3            Left Motor Dir B
  IN3                   Pin 4            Right Motor Dir A
  IN4                   Pin 7            Right Motor Dir B
  ENB                   Pin 6 (PWM)      Right Motor Speed
  -------------------------------------------------------------
*/

// Ultrasonic Sensor Pins
#define TRIG 9
#define ECHO 10

// Left Motor Driver Pins
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6
#define IN3 4
#define IN4 7

// Obstacle Avoidance Distance Thresholds (in centimeters)
const int SAFE_DISTANCE_CM     = 25; // Stop & evasive turn threshold
const int CRITICAL_DISTANCE_CM = 12; // Immediate emergency reverse threshold

// Cruising and Maneuver Speeds
const int CRUISE_SPEED = 180;
const int TURN_SPEED   = 190;

// Alternate evasion turn direction to prevent corner deadlock
bool turnRightNext = true;

void setup() {
  Serial.begin(9600);

  // Setup Ultrasonic Sensor Pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Setup Motor Pins as Outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot(500);

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.8: Obstacle Avoiding Vehicle        "));
  Serial.println(F("=================================================="));
  Serial.print(F("Safety Threshold: "));
  Serial.print(SAFE_DISTANCE_CM);
  Serial.println(F(" cm"));
  Serial.println(F("Starting autonomous navigation in 3 seconds...\n"));
  delay(3000);
}

void loop() {
  // 1. Measure Filtered Front Distance (Averaged 3 samples)
  long distance = readFilteredDistanceCM();

  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));

  // 2. Evaluate Navigation State Machine
  if (distance > SAFE_DISTANCE_CM) {
    // Condition 1: Path is clear -> Cruise Forward
    moveForward(CRUISE_SPEED);
  }
  else if (distance <= CRITICAL_DISTANCE_CM && distance > 0) {
    // Condition 2: Critical Proximity -> Emergency Stop & Extended Reverse
    Serial.println(F("[EMERGENCY] Critical proximity! Reversing & wide spin..."));
    stopRobot(200);
    moveBackward(CRUISE_SPEED, 600);
    stopRobot(200);
    
    // Execute wide evasive spin turn
    if (turnRightNext) {
      spinRight(TURN_SPEED, 600);
    } else {
      spinLeft(TURN_SPEED, 600);
    }
    turnRightNext = !turnRightNext; // Alternate next turn
    stopRobot(300);
  }
  else if (distance <= SAFE_DISTANCE_CM && distance > 0) {
    // Condition 3: Standard Obstacle Detected -> Stop, Reverse Clearance, Turn
    Serial.println(F("[OBSTACLE] Path obstructed. Executing evasive turn..."));
    stopRobot(250);
    
    // Brief reverse for bumper clearance
    moveBackward(CRUISE_SPEED, 350);
    stopRobot(150);

    // Evasive turn to find clear path
    if (turnRightNext) {
      spinRight(TURN_SPEED, 450);
    } else {
      spinLeft(TURN_SPEED, 450);
    }
    turnRightNext = !turnRightNext; // Alternate next turn
    stopRobot(250);
  }

  delay(40); // Navigation loop interval
}

// -------------------------------------------------------------
// Filtered Ultrasonic Distance Reading
// -------------------------------------------------------------

long readSinglePingCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Measure echo pulse time (timeout at 25000us / ~4.2m)
  long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0) {
    return 999; // No echo received (clear open field)
  }

  long distance = (duration * 0.0343) / 2;
  return distance;
}

// Averages multiple pings to filter out random acoustic glitches
long readFilteredDistanceCM() {
  long d1 = readSinglePingCM();
  delayMicroseconds(500);
  long d2 = readSinglePingCM();
  
  if (d1 == 999 && d2 == 999) return 999;
  if (d1 == 999) return d2;
  if (d2 == 999) return d1;

  return (d1 + d2) / 2;
}

// -------------------------------------------------------------
// Motor Kinematic Primitives
// -------------------------------------------------------------

void moveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moveBackward(int speed, int durationMs) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  if (durationMs > 0) delay(durationMs);
}

void spinRight(int speed, int durationMs) {
  // Zero-radius right turn: Left forward, Right reverse
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  if (durationMs > 0) delay(durationMs);
}

void spinLeft(int speed, int durationMs) {
  // Zero-radius left turn: Left reverse, Right forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  if (durationMs > 0) delay(durationMs);
}

void stopRobot(int durationMs) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  if (durationMs > 0) delay(durationMs);
}
