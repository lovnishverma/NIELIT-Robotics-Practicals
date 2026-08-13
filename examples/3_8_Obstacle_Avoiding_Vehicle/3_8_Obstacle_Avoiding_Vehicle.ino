/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle
  =========================================================

  Objective:
  Implement an autonomous collision avoidance system using an HC-SR04 ultrasonic distance sensor,
  dual-sample acoustic noise filtering, deterministic sensor health validation, and a defensive navigation state machine.

  Description:
  Demonstrates autonomous obstacle detection and evasion on a mobile robot.
  Measures time-of-flight acoustic echo duration to calculate forward spatial clearance in centimeters.
  Executes automated navigation routines: Forward Cruise (on confirmed clear path), Obstacle Avoidance (Stop -> Reverse -> Spin Turn),
  Critical Emergency Evasion, and a Defensive Safe Stop on sensor timeout or hardware communication loss.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - HC-SR04 Ultrasonic Distance Sensor Module (5V DC VCC, 40 kHz ultrasound transducer pair)
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors (Nominal: 3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply: 6.0V - 7.4V (e.g. 4x AA Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Module / Driver Pin      Arduino UNO Pin   Description
  -------------------------------------------------------------
  HC-SR04 VCC              5V                5V Regulated Supply Rail
  HC-SR04 GND              GND               Common Ground
  HC-SR04 TRIG             Pin 9             Ultrasonic Trigger Pulse Output
  HC-SR04 ECHO             Pin 10            Echo Return Pulse Input
  ENA                      Pin 5 (PWM)       Left Motor Speed Enable (Timer0)
  IN1                      Pin 2             Left Motor Direction Input 1
  IN2                      Pin 3             Left Motor Direction Input 2
  IN3                      Pin 4             Right Motor Direction Input 1
  IN4                      Pin 7             Right Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor Speed Enable (Timer0)
  VCC2 / VM                Battery (+)       Motor Power (6.0V - 7.4V Recommended)
  GND                      GND & Batt (-)    Common Ground Busbar (Mandatory)
  -------------------------------------------------------------

  Working Principle & Ultrasonic Timing:
  The HC-SR04 emits an 8-cycle 40 kHz ultrasonic burst when a 10 microsecond HIGH pulse is applied to `TRIG`.
  The `ECHO` output goes HIGH and remains HIGH until the acoustic reflection is received.
  Distance is calculated using the speed of sound in dry air at 20 deg C (~343 m/s = 0.0343 cm/us):
    Distance (cm) = (Echo_Duration_microseconds * 0.0343) / 2
  Dual-sample filtering averages two successive pings to eliminate isolated acoustic bounce glitches.

  Obstacle Avoidance Decision State Table:
  +--------------------------------+-----------------------+-----------------------------------------------+
  | Distance Measurement           | Navigation State      | Robot Action Executed                         |
  +--------------------------------+-----------------------+-----------------------------------------------+
  | Valid Distance > 25 cm         | Path Confirmed Clear  | Cruise Forward at normal speed                |
  | 12 cm < Valid Distance <= 25 cm| Impediment Detected   | Stop -> Reverse 350ms -> Spin Turn 450ms      |
  | 0 < Valid Distance <= 12 cm    | Critical Proximity    | Stop -> Reverse 600ms -> Wide Spin Turn 600ms |
  | Timeout (-1) / Invalid Echo    | Sensor Fault/Unknown  | DEFENSIVE SAFE STOP (Hold in place)           |
  +--------------------------------+-----------------------+-----------------------------------------------+

  Expected Behavior:
  1. Startup Validation: Evaluates ultrasonic sensor health across 10 bounded retries before entering autonomous mode.
  2. Cruise: Only drives forward when distance is actively verified greater than 25 cm.
  3. Avoidance: Upon approaching a wall within 25cm, the robot halts, reverses, executes an evasive turn, and resumes.
  4. Defensive Safety Stop: If the sensor is unplugged, unpowered, or times out, the robot stops immediately.
  5. Deadlock Prevention: Alternating turn direction (`turnRightNext = !turnRightNext`) prevents corner trapping.

  Sensor Failure & Blind-Zone Safety Rules:
  - An echo timeout (`pulseIn == 0`) returns `-1` (UNKNOWN / SENSOR_TIMEOUT).
  - UNKNOWN distance is NEVER treated as clear space. The robot halts to prevent unguided collisions.

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
#define ENA 5   // Timer0 PWM
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6   // Timer0 PWM
#define IN3 4
#define IN4 7

// =====================================================
// DISTANCE THRESHOLDS & SPEED CONSTANTS
// =====================================================

const int SAFE_DISTANCE_CM     = 25; // Stop & turn threshold (cm)
const int CRITICAL_DISTANCE_CM = 12; // Emergency reverse threshold (cm)

const int CRUISE_SPEED = 180; // PWM forward cruising speed (0-255)
const int TURN_SPEED   = 190; // PWM evasive turn speed (0-255)

// Alternating turn direction flag for deadlock prevention
bool turnRightNext = true;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

long readSinglePingCM();
long readFilteredDistanceCM();
bool validateSensorHealth();
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

  // Safe initialization: All motors stopped
  stopRobot(500);

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.8: Obstacle Avoiding Vehicle  "));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.print(F("[INFO] Safety Threshold: "));
  Serial.print(SAFE_DISTANCE_CM);
  Serial.println(F(" cm"));
  Serial.println(F("[INFO] Executing deterministic sensor startup validation..."));

  // Deterministic sensor validation: Must obtain valid echo before entering autonomous mode
  bool sensorReady = validateSensorHealth();

  if (!sensorReady) {
    Serial.println(F("[ERROR] Sensor validation FAILED. No echo received."));
    Serial.println(F("[FATAL] Check HC-SR04 connections: VCC(5V), GND, TRIG(D9), ECHO(D10)."));
    Serial.println(F("[HALT] Robot locked in safe stop. Autonomous mode aborted."));
    while (true) {
      stopRobot(1000); // Lock indefinitely in safe stop
    }
  }

  Serial.println(F("[INFO] Sensor validation PASSED. Ultrasonic transducer online."));
  Serial.println(F("[INFO] Starting autonomous navigation in 3s...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // 1. Measure Filtered Distance (Averaged pings, returns -1 on timeout)
  long distance = readFilteredDistanceCM();

  Serial.print(F("[SENSOR] Measured Distance: "));
  if (distance == -1) {
    Serial.println(F("TIMEOUT / UNKNOWN"));
  } else {
    Serial.print(distance);
    Serial.println(F(" cm"));
  }

  // 2. Defensive Navigation State Machine
  // Condition 1: Path is confirmed clear by active sensor measurement
  if (distance > SAFE_DISTANCE_CM) {
    moveForward(CRUISE_SPEED);
  }
  // Condition 2: Critical proximity (Impediment closer than 12cm)
  else if (distance <= CRITICAL_DISTANCE_CM && distance > 0) {
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
  // Condition 3: Standard obstacle detected (12cm to 25cm)
  else if (distance <= SAFE_DISTANCE_CM && distance > 0) {
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
  // Condition 4: Defensive Safety Stop on Sensor Timeout (-1) or Failure
  else {
    Serial.println(F("[SAFETY HALT] Sensor timeout or echo loss! Halting motors."));
    stopRobot(100);
  }

  delay(40); // Navigation loop sampling interval
}

// -------------------------------------------------------------
// SENSOR VALIDATION & FILTERED MEASUREMENT HELPERS
// -------------------------------------------------------------

bool validateSensorHealth() {
  // Perform up to 10 bounded ping attempts to confirm sensor responsiveness
  for (int attempt = 1; attempt <= 10; attempt++) {
    long d = readSinglePingCM();
    if (d > 0 && d <= 400) {
      Serial.print(F("[INFO] Echo detected on attempt "));
      Serial.print(attempt);
      Serial.print(F(": "));
      Serial.print(d);
      Serial.println(F(" cm"));
      return true;
    }
    delay(100);
  }
  return false;
}

long readSinglePingCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Measure echo pulse time (timeout at 25000us / ~4.2 meters)
  long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0) {
    return -1; // No echo received within timeout window
  }

  long distance = (duration * 0.0343) / 2;
  return distance;
}

long readFilteredDistanceCM() {
  long d1 = readSinglePingCM();
  delayMicroseconds(500);
  long d2 = readSinglePingCM();

  if (d1 == -1 && d2 == -1) return -1;
  if (d1 == -1) return d2;
  if (d2 == -1) return d1;

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
