/*
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle
  Course: NIELIT Robotics Practicals

  Description:
  Implements an intelligent autonomous obstacle-avoiding robotic vehicle
  utilizing an HC-SR04 ultrasonic distance sensor. Continuously monitors
  forward clearance, detects impediments within a safety threshold,
  and executes automated evasive maneuvers (Stop -> Reverse -> Spin Turn -> Resume).

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
  TRIG                  Pin 12           Trigger Pulse (Output)
  ECHO                  Pin 13           Echo Time (Input)
  -------------------------------------------------------------
  Motor Driver Pin      Arduino Pin      Description
  -------------------------------------------------------------
  ENA                   Pin 9 (PWM)      Left Motor Speed
  IN1                   Pin 5            Left Motor Dir A
  IN2                   Pin 6            Left Motor Dir B
  IN3                   Pin 10           Right Motor Dir A
  IN4                   Pin 11           Right Motor Dir B
  ENB                   Pin 3 (PWM)      Right Motor Speed
  -------------------------------------------------------------
*/

// Ultrasonic Sensor Pins
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// Motor Driver Pins
const int ENA = 9;
const int IN1 = 5;
const int IN2 = 6;
const int ENB = 3;
const int IN3 = 10;
const int IN4 = 11;

// Obstacle Avoidance Thresholds
const int SAFE_DISTANCE_CM     = 25; // Stop & turn if object is closer than 25cm
const int CRITICAL_DISTANCE_CM = 12; // Emergency reverse if closer than 12cm

// Cruising and Maneuver Speeds
const int CRUISE_SPEED = 190;
const int TURN_SPEED   = 200;

void setup() {
  Serial.begin(9600);

  // Setup Ultrasonic Sensor Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup Motor Pins
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
  Serial.println(F("Starting navigation in 3 seconds...\n"));
  delay(3000);
}

void loop() {
  // 1. Measure Front Distance
  long distance = readDistanceCM();

  Serial.print(F("Front Distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));

  // 2. Evaluate Navigation State
  if (distance > SAFE_DISTANCE_CM) {
    // Path is unobstructed: Cruise Forward
    moveForward(CRUISE_SPEED);
  }
  else if (distance <= CRITICAL_DISTANCE_CM && distance > 0) {
    // Critical Proximity: Immediate Stop & Back up
    Serial.println(F("[ALERT] Critical proximity detected! Reversing..."));
    stopRobot(200);
    moveBackward(CRUISE_SPEED, 600);
    stopRobot(200);
    
    // Perform wide evasive spin turn
    Serial.println(F("[Evasion] Executing wide evasive turn..."));
    spinRight(TURN_SPEED, 600);
    stopRobot(300);
  }
  else if (distance <= SAFE_DISTANCE_CM && distance > 0) {
    // Normal Obstacle: Stop and Turn Right to find clear path
    Serial.println(F("[Obstacle Detected] Performing evasive turn..."));
    stopRobot(300);
    
    // Quick reverse for bumper clearance
    moveBackward(CRUISE_SPEED, 350);
    stopRobot(150);

    // Spin turn to clear obstacle
    spinRight(TURN_SPEED, 450);
    stopRobot(300);
  }

  delay(60); // Measurement refresh interval
}

// -------------------------------------------------------------
// Sensor Helper Function
// -------------------------------------------------------------

long readDistanceCM() {
  // Clear trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send 10 microsecond ultrasonic pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo travel time in microseconds (timeout at 30ms / ~5 meters)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    // No echo received / out of range
    return 999;
  }

  // Convert time to distance (Speed of sound = 343 m/s = 0.0343 cm/us)
  long distance = (duration * 0.0343) / 2;
  return distance;
}

// -------------------------------------------------------------
// Motor Kinematic Primitives
// -------------------------------------------------------------

void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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

void spinRight(int speed, int durationMs) {
  // Zero-radius right turn: Left Forward, Right Reverse
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
