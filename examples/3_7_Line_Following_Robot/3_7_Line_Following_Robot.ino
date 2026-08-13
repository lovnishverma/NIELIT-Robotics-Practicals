/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.7: Autonomous Navigation Capstone I — Line-Following Robot
  =========================================================

  Objective:
  Implement an autonomous closed-loop trajectory tracking system using dual infrared (IR)
  reflectance sensors and smooth differential forward velocity steering.

  Description:
  Demonstrates autonomous optical navigation over a contrasting line track (e.g. black electrical
  tape on white floor). Reads left and right digital IR reflectance sensor signals, evaluates
  tracking deviation, and applies smooth differential forward velocity steering to maintain alignment.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - 2x TCRT5000 Infrared Reflectance Sensor Modules
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors (Nominal: 3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply: 6.0V - 7.4V (e.g. 4x AA Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Module / Driver Pin      Arduino UNO Pin   Description
  -------------------------------------------------------------
  Left IR Sensor (OUT)     Pin 2             Digital Line Detection (Left)
  Right IR Sensor (OUT)    Pin 3             Digital Line Detection (Right)
  Sensor VCC / GND         5V / GND          5V Regulated Supply Rail
  ENA                      Pin 5 (PWM)       Left Motor Speed Enable (Timer0)
  IN1                      Pin 8             Left Motor Direction Input 1
  IN2                      Pin 9             Left Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor Speed Enable (Timer0)
  IN3                      Pin 10            Right Motor Direction Input 1
  IN4                      Pin 11            Right Motor Direction Input 2
  VCC2 / VM                Battery (+)       Motor Power (6.0V - 7.4V Recommended)
  GND                      GND & Batt (-)    Common Ground Busbar (Mandatory)
  -------------------------------------------------------------

  Sensor & Track Geometry Assumptions:
  - Track Setup: 15mm to 19mm wide black electrical tape applied to a white reflective surface.
  - Sensor Spacing: The optical axes of the Left and Right TCRT5000 sensors must be mounted
    approximately 22mm to 28mm apart so that they straddle the black line.
  - When centered, both sensors view the white background (00 state).
  - TCRT5000 Active Logic:
      * Standard modules: High reflection (white floor) = LOW (0), Low reflection (black tape) = HIGH (1).
      * If your module outputs the inverted signal, set `BLACK_LINE_MODE = false`.

  Line Following State Table (Standard Mode: Black Line on White Surface):
  +-------------+--------------+-----------------------+------------------------------------+
  | Left Sensor | Right Sensor | Physical Condition    | Robot Maneuver                     |
  +-------------+--------------+-----------------------+------------------------------------+
  |  0 (White)  |  0 (White)   | Robot Centered on Line| Drive Forward (Both motors cruise) |
  |  1 (Black)  |  0 (White)   | Robot Veered Right    | Steer Left (Left=50, Right=180)    |
  |  0 (White)  |  1 (Black)   | Robot Veered Left     | Steer Right (Left=180, Right=50)   |
  |  1 (Black)  |  1 (Black)   | Intersection / End    | Full Stop (Drivers disabled)       |
  +-------------+--------------+-----------------------+------------------------------------+

  Expected Behavior:
  1. Startup: Telemetry announces active tracking mode after a 3-second delay.
  2. Bench Test: When lifted off the floor, placing black tape under the left sensor causes the right
     wheel to accelerate and the left wheel to slow down (steering left toward the line).
  3. Track Navigation: The vehicle smoothly tracks along curved trajectories without aggressive oscillating pivots.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// IR Sensor Digital Input Pins
#define LEFT_SENSOR  2
#define RIGHT_SENSOR 3

// Left Motor Driver Pins
#define ENA 5   // Timer0 PWM
#define IN1 8
#define IN2 9

// Right Motor Driver Pins
#define ENB 6   // Timer0 PWM
#define IN3 10
#define IN4 11

// =====================================================
// CONFIGURATION & SPEED CONSTANTS
// =====================================================

const int FORWARD_SPEED = 150; // Base forward speed (PWM 0-255)
const int TURN_SPEED    = 180; // High speed for outside steering wheel
const int SLOW_SPEED    = 50;  // Low speed for inside steering wheel

// Set true for standard BLACK line on WHITE surface (TCRT5000 HIGH on black)
// Set false if module outputs LOW on black
const bool BLACK_LINE_MODE = true;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void driveForward(int speed);
void steerLeft();
void steerRight();
void stopRobot();

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

  // Safe initialization
  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.7: Line Following Robot       "));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.print(F("[INFO] Tracking Mode: "));
  if (BLACK_LINE_MODE) {
    Serial.println(F("BLACK line on WHITE surface (Active HIGH)"));
  } else {
    Serial.println(F("WHITE line on BLACK surface (Active LOW)"));
  }
  Serial.println(F("[INFO] Starting in 3 seconds...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  int rawLeft  = digitalRead(LEFT_SENSOR);
  int rawRight = digitalRead(RIGHT_SENSOR);

  // Normalize sensor readings based on line polarity mode
  bool leftOnLine;
  bool rightOnLine;

  if (BLACK_LINE_MODE) {
    leftOnLine  = (rawLeft == HIGH);
    rightOnLine = (rawRight == HIGH);
  } else {
    leftOnLine  = (rawLeft == LOW);
    rightOnLine = (rawRight == LOW);
  }

  // ---------------------------------------------------
  // Real-Time Sensor Telemetry
  // ---------------------------------------------------
  Serial.print(F("L="));
  Serial.print(rawLeft);
  Serial.print(F(" R="));
  Serial.print(rawRight);
  Serial.print(F(" | Line: L="));
  Serial.print(leftOnLine);
  Serial.print(F(" R="));
  Serial.println(rightOnLine);

  // ---------------------------------------------------
  // Smooth Differential Steering State Machine
  // ---------------------------------------------------
  // State 00: Both sensors detect white floor (Robot centered straddling the line)
  if (!leftOnLine && !rightOnLine) {
    driveForward(FORWARD_SPEED);
  }
  // State 10: Left sensor hit black line (Robot veered right -> gentle steer left)
  else if (leftOnLine && !rightOnLine) {
    steerLeft();
  }
  // State 01: Right sensor hit black line (Robot veered left -> gentle steer right)
  else if (!leftOnLine && rightOnLine) {
    steerRight();
  }
  // State 11: Both sensors detect line (Crossroad / Stop Mark / T-Junction)
  else {
    stopRobot();
    Serial.println(F("[TRACK] Both sensors active - Junction or End marker detected"));
    delay(100);
  }

  delay(5); // Rapid reactive control loop
}

// =====================================================
// MOTOR CONTROL PRIMITIVES (Smooth Differential Drive)
// =====================================================

void driveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
