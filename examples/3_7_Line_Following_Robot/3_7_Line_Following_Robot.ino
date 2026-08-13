/*
  Practical 3.7: Autonomous Navigation Capstone I — Line-Following Robot
  Course: NIELIT Robotics Practicals

  Description:
  Implements an autonomous line-following mobile robot utilizing dual infrared (IR)
  reflectance sensors (e.g., TCRT5000 modules). Features configurable line polarity
  (Black line on White surface vs White line on Black surface), adaptive differential
  steering corrections, and junction detection.

  Sensor Logic Table (Black Line on White Surface):
  +-------------+--------------+------------------+------------------------------+
  | Left Sensor | Right Sensor | Track Condition  | Robot Maneuver               |
  +-------------+--------------+------------------+------------------------------+
  |  0 (White)  |  0 (White)   | On Track (Center)| Forward (Both motors drive)  |
  |  1 (Black)  |  0 (White)   | Veering Right    | Steer Left (Left=0, Right=PWM)|
  |  0 (White)  |  1 (Black)   | Veering Left     | Steer Right (Left=PWM, Right=0)|
  |  1 (Black)  |  1 (Black)   | Intersection / End| Stop or Cross Junction       |
  +-------------+--------------+------------------+------------------------------+

  Hardware Connections:
  -------------------------------------------------------------
  Module Pin            Arduino Pin      Description
  -------------------------------------------------------------
  Left IR Sensor (OUT)  Pin 2 (Digital)  Left Track Sensor
  Right IR Sensor (OUT) Pin 3 (Digital)  Right Track Sensor
  Sensor VCC / GND      5V / GND         Power supply
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

// IR Sensor Pins (Digital output from IR modules with onboard comparators)
#define LEFT_SENSOR  2
#define RIGHT_SENSOR 3

// Motor Driver Pins
#define ENA 5
#define IN1 8
#define IN2 9

#define ENB 6
#define IN3 10
#define IN4 11

// Base cruising and turning speed (PWM 0-255)
const int FORWARD_SPEED = 160;
const int TURN_SPEED    = 180;

// Set to true if tracking a BLACK line on a WHITE floor (Standard)
// Set to false if tracking a WHITE line on a BLACK floor
const bool BLACK_LINE_MODE = true;

void setup() {
  Serial.begin(9600);

  // Initialize Sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  // Initialize Motor Control Pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.7: Autonomous Line Following Robot  "));
  Serial.println(F("=================================================="));
  Serial.print(F("Mode: "));
  Serial.println(BLACK_LINE_MODE ? F("Black line on White surface") : F("White line on Dark surface"));
  Serial.println(F("Place robot on track. Starting in 3 seconds...\n"));
  delay(3000);
}

void loop() {
  // Read Digital Values from IR Sensor Modules
  // Most modules output LOW (0) when reflecting white and HIGH (1) over dark/black surface
  int rawLeft  = digitalRead(LEFT_SENSOR);
  int rawRight = digitalRead(RIGHT_SENSOR);

  // Determine normalized line detection boolean
  bool leftOnLine  = BLACK_LINE_MODE ? (rawLeft == HIGH)  : (rawLeft == LOW);
  bool rightOnLine = BLACK_LINE_MODE ? (rawRight == HIGH) : (rawRight == LOW);

  // --- Decision Logic ---
  if (!leftOnLine && !rightOnLine) {
    // Condition 1: Both sensors on white floor (Line is perfectly centered)
    // Drive Forward
    driveForward(FORWARD_SPEED);
  }
  else if (leftOnLine && !rightOnLine) {
    // Condition 2: Left sensor hit black line (Vehicle drifted to the right)
    // Correct by turning Left
    pivotLeft(TURN_SPEED);
  }
  else if (!leftOnLine && rightOnLine) {
    // Condition 3: Right sensor hit black line (Vehicle drifted to the left)
    // Correct by turning Right
    pivotRight(TURN_SPEED);
  }
  else {
    // Condition 4: Both sensors on black line (Crossroad / Stop Mark / T-Junction)
    // Default action: Stop vehicle
    stopRobot();
    Serial.println(F("[Track] Both sensors active: Line intersection/End reached."));
  }

  // Small telemetry heartbeat (reduced delay for fast real-time response)
  delay(10);
}

// -------------------------------------------------------------
// Robot Movement Routines
// -------------------------------------------------------------

void driveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void pivotLeft(int speed) {
  // Left motor reversed slightly or stopped, Right motor drives forward
  analogWrite(ENA, speed / 3);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void pivotRight(int speed) {
  // Right motor reversed slightly or stopped, Left motor drives forward
  analogWrite(ENA, speed);
  analogWrite(ENB, speed / 3);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
