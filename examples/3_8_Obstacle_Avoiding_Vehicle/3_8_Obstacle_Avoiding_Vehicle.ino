/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle
  =========================================================

  Objective:
  Implement an autonomous collision avoidance system using an HC-SR04 ultrasonic distance sensor,
  multi-sample acoustic noise filtering, and a reactive evasive navigation state machine.

  Description:
  Demonstrates autonomous obstacle detection and evasion on a mobile robot.
  Measures time-of-flight acoustic echo duration to calculate forward spatial clearance in centimeters.
  Executes automated navigation routines: Forward Cruise, Obstacle Avoidance (Stop -> Reverse -> Spin Turn),
  and Critical Emergency Evasion with adaptive alternating turn directions to prevent corner deadlock.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - HC-SR04 Ultrasonic Distance Sensor Module
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply (6V - 12V Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Module / Driver Pin      Arduino UNO Pin   Description
  -------------------------------------------------------------
  HC-SR04 VCC              5V                5V Regulated Supply
  HC-SR04 GND              GND               Common Ground
  HC-SR04 TRIG             Pin 9             Ultrasonic Trigger Pulse Output
  HC-SR04 ECHO             Pin 10            Echo Return Pulse Input
  ENA                      Pin 5 (PWM)       Left Motor Speed Enable
  IN1                      Pin 2             Left Motor Direction Input 1
  IN2                      Pin 3             Left Motor Direction Input 2
  IN3                      Pin 4             Right Motor Direction Input 1
  IN4                      Pin 7             Right Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor Speed Enable
  VCC2 / VM                Battery (+)       Motor Power (+6V to +12V)
  GND                      GND & Batt (-)    Common Ground Busbar
  -------------------------------------------------------------

  Working Principle:
  The HC-SR04 sensor transmits a 40 kHz ultrasonic burst upon receiving a 10 microsecond HIGH pulse on `TRIG`.
  The `ECHO` pin remains HIGH for the duration of the sound wave's round trip.
  Distance is calculated via:
    Distance (cm) = (Echo_Duration_us * 0.0343) / 2
  Dual-sample filtering eliminates occasional single-ping acoustic reflection noise glitches.

  Obstacle Avoidance Decision State Table:
  +---------------------------+-----------------------+-----------------------------------------------+
  | Distance Measurement      | Navigation State      | Robot Action Executed                         |
  +---------------------------+-----------------------+-----------------------------------------------+
  | Distance > 25 cm          | Path Clear            | Cruise Forward at normal speed                |
  | 12 cm < Distance <= 25 cm | Impediment Detected   | Stop -> Reverse 350ms -> Spin Turn 450ms      |
  | 0 < Distance <= 12 cm     | Critical Proximity    | Stop -> Reverse 600ms -> Wide Spin Turn 600ms |
  +---------------------------+-----------------------+-----------------------------------------------+

  Expected Behavior:
  1. Startup: Safe initialization with a 3-second preparation delay.
  2. Cruise: In open space, the vehicle drives steadily forward.
  3. Avoidance: Upon approaching a wall or object within 25cm, the robot halts, backs up slightly,
     executes an evasive spin turn, and resumes forward travel in a clear direction.

  Notes:
  - Alternating turn direction (`turnRightNext = !turnRightNext`) prevents the vehicle from getting
    trapped in continuous corner loops.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

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

// =====================================================
// DISTANCE THRESHOLDS & SPEED CONSTANTS
// =====================================================

const int SAFE_DISTANCE_CM     = 25; // Stop & turn threshold
const int CRITICAL_DISTANCE_CM = 12; // Emergency reverse threshold

const int CRUISE_SPEED = 180; // PWM forward cruising speed (0-255)
const int TURN_SPEED   = 190; // PWM evasive turn speed (0-255)

// Alternating turn direction flag for deadlock prevention
bool turnRightNext = true;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

long readSinglePingCM();
long readFilteredDistanceCM();
void moveForward(int speed);
void moveBackward(int speed, int durationMs);
void spinRight(int speed, int durationMs);
void spinLeft(int speed, int durationMs);
void stopRobot(int durationMs);

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Safe initialization
  stopRobot(500);

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.8: Obstacle Avoiding Vehicle  "));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.print(F("[INFO] Safety Threshold: "));
  Serial.print(SAFE_DISTANCE_CM);
  Serial.println(F(" cm"));
  Serial.println(F("[INFO] Starting autonomous navigation in 3s...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // 1. Measure Filtered Distance (Averaged 2 pings)
  long distance = readFilteredDistanceCM();

  Serial.print(F("[SENSOR] Distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));

  // 2. Navigation State Machine
  if (distance > SAFE_DISTANCE_CM) {
    // Condition 1: Path unobstructed -> Cruise Forward
    moveForward(CRUISE_SPEED);
  }
  else if (distance <= CRITICAL_DISTANCE_CM && distance > 0) {
    // Condition 2: Critical proximity -> Emergency stop & extended reverse
    Serial.println(F("[EMERGENCY] Critical proximity! Reversing and executing wide evasion..."));
    stopRobot(200);
    moveBackward(CRUISE_SPEED, 600);
    stopRobot(200);

    if (turnRightNext) {
      spinRight(TURN_SPEED, 600);
    } else {
      spinLeft(TURN_SPEED, 600);
    }
    turnRightNext = !turnRightNext; // Alternate turn direction to prevent corner deadlock
    stopRobot(300);
  }
  else if (distance <= SAFE_DISTANCE_CM && distance > 0) {
    // Condition 3: Normal obstacle detected -> Stop, brief reverse, spin turn
    Serial.println(F("[OBSTACLE] Path obstructed. Executing evasive turn..."));
    stopRobot(250);

    // Brief reverse for bumper clearance
    moveBackward(CRUISE_SPEED, 350);
    stopRobot(150);

    if (turnRightNext) {
      spinRight(TURN_SPEED, 450);
    } else {
      spinLeft(TURN_SPEED, 450);
    }
    turnRightNext = !turnRightNext; // Alternate turn direction
    stopRobot(250);
  }

  delay(40); // Navigation loop interval
}

// -------------------------------------------------------------
// FILTERED ULTRASONIC DISTANCE SENSOR HELPERS
// -------------------------------------------------------------

long readSinglePingCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Measure echo pulse time (timeout at 25000us / ~4.2 meters)
  long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0) {
    return 999; // No echo received (clear open field)
  }

  long distance = (duration * 0.0343) / 2;
  return distance;
}

long readFilteredDistanceCM() {
  long d1 = readSinglePingCM();
  delayMicroseconds(500);
  long d2 = readSinglePingCM();

  if (d1 == 999 && d2 == 999) return 999;
  if (d1 == 999) return d2;
  if (d2 == 999) return d1;

  return (d1 + d2) / 2;
}

// =====================================================
// MOTOR KINEMATIC PRIMITIVES
// =====================================================

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
