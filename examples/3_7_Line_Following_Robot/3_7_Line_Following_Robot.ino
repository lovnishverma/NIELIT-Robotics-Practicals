/*
  Practical 3.7: Autonomous Navigation Capstone I — Line-Following Robot
  Course: NIELIT Robotics Practicals

  Description:
  Implements an autonomous line-following mobile robot utilizing dual infrared (IR)
  reflectance sensors (e.g., TCRT5000 modules) with smooth differential steering.
  Features real-time sensor telemetry, configurable line polarity (Black line on
  White surface vs White line on Black surface), and intersection/end detection.

  Sensor Calibration & Logic (Standard: Black line on White surface):
  +-------------+--------------+------------------+--------------------------------------+
  | Left Sensor | Right Sensor | Track Condition  | Robot Maneuver                       |
  +-------------+--------------+------------------+--------------------------------------+
  |  0 (White)  |  0 (White)   | Centered on Path | Forward (Both motors drive forward)  |
  |  1 (Black)  |  0 (White)   | Veering Right    | Steer Left (Left slower, Right fast) |
  |  0 (White)  |  1 (Black)   | Veering Left     | Steer Right (Left fast, Right slower)|
  |  1 (Black)  |  1 (Black)   | Intersection/End | Stop vehicle                         |
  +-------------+--------------+------------------+--------------------------------------+

  Hardware Connections:
  -------------------------------------------------------------
  Module Pin            Arduino Pin      Description
  -------------------------------------------------------------
  Left IR Sensor (OUT)  Pin 2 (Digital)  Left Track Sensor
  Right IR Sensor (OUT) Pin 3 (Digital)  Right Track Sensor
  Sensor VCC / GND      5V / GND         5V Power Rail
  -------------------------------------------------------------
  Motor Driver Pin      Arduino Pin      Description
  -------------------------------------------------------------
  ENA                   Pin 5 (PWM)      Left Motor Speed
  IN1                   Pin 8            Left Motor Dir A
  IN2                   Pin 9            Left Motor Dir B
  IN3                   Pin 10           Right Motor Dir A
  IN4                   Pin 11           Right Motor Dir B
  ENB                   Pin 6 (PWM)      Right Motor Speed
  -------------------------------------------------------------
*/

// =====================================================
// IR SENSOR PINS
// =====================================================
#define LEFT_SENSOR   2
#define RIGHT_SENSOR  3

// =====================================================
// MOTOR DRIVER (L293D / L298N)
// =====================================================
// LEFT MOTOR
#define ENA 5
#define IN1 8
#define IN2 9

// RIGHT MOTOR
#define ENB 6
#define IN3 10
#define IN4 11

// =====================================================
// SPEED CONFIGURATION
// =====================================================
const int FORWARD_SPEED = 150;
const int TURN_SPEED    = 180;
const int SLOW_SPEED    = 50;

// true  = Black line on White surface (Standard)
// false = White line on Black surface
const bool BLACK_LINE_MODE = true;

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot();

  Serial.println(F("=============================================="));
  Serial.println(F(" Practical 3.7 - Line Following Robot         "));
  Serial.println(F("=============================================="));
  Serial.print(F("Mode: "));
  if (BLACK_LINE_MODE) {
    Serial.println(F("BLACK line / WHITE surface"));
  } else {
    Serial.println(F("WHITE line / BLACK surface"));
  }
  Serial.println(F("Starting in 3 seconds..."));
  delay(3000);
}

// =====================================================
// MAIN LOOP
// =====================================================
void loop() {
  int rawLeft  = digitalRead(LEFT_SENSOR);
  int rawRight = digitalRead(RIGHT_SENSOR);

  // Normalize sensor readings:
  // Default expectation: HIGH = BLACK surface, LOW = WHITE surface
  // If your TCRT5000 module behaves opposite, adjust polarity logic here.
  bool leftOnLine;
  bool rightOnLine;

  if (BLACK_LINE_MODE) {
    leftOnLine  = (rawLeft == HIGH);
    rightOnLine = (rawRight == HIGH);
  } else {
    leftOnLine  = (rawLeft == LOW);
    rightOnLine = (rawRight == LOW);
  }

  // ===================================================
  // REAL-TIME SENSOR TELEMETRY
  // ===================================================
  Serial.print(F("L="));
  Serial.print(rawLeft);
  Serial.print(F(" R="));
  Serial.print(rawRight);
  Serial.print(F(" | Line: L="));
  Serial.print(leftOnLine);
  Serial.print(F(" R="));
  Serial.println(rightOnLine);

  // ===================================================
  // SMOOTH DIFFERENTIAL LINE-FOLLOWING LOGIC
  // ===================================================
  // Condition 00: Neither sensor detects line (Robot is centered between sensors)
  if (!leftOnLine && !rightOnLine) {
    driveForward(FORWARD_SPEED);
  }
  // Condition 10: Left sensor hit the line (Robot veered right -> smooth correction left)
  else if (leftOnLine && !rightOnLine) {
    steerLeft();
  }
  // Condition 01: Right sensor hit the line (Robot veered left -> smooth correction right)
  else if (!leftOnLine && rightOnLine) {
    steerRight();
  }
  // Condition 11: Both sensors detect line (Crossroad / Stop Marker / T-Junction)
  else {
    stopRobot();
    Serial.println(F("[TRACK] Both sensors active - Junction/End marker detected"));
    delay(100);
  }

  delay(5);
}

// =====================================================
// MOTOR CONTROL PRIMITIVES (Smooth Differential Drive)
// =====================================================

void driveForward(int speed) {
  // LEFT MOTOR FORWARD
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // RIGHT MOTOR FORWARD
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void steerLeft() {
  // Smooth differential steer left: Left motor slower, Right motor faster
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SLOW_SPEED);
  analogWrite(ENB, TURN_SPEED);
}

void steerRight() {
  // Smooth differential steer right: Left motor faster, Right motor slower
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, SLOW_SPEED);
}

void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
