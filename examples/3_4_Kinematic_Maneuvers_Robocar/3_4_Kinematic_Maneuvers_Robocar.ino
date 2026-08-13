/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.4: Kinematic Maneuvers — 2-Wheel Robocar Basic Movements
  =========================================================

  Objective:
  Implement modular kinematic motion primitives for a 2-wheel differential drive mobile robot
  and execute an open-loop timed geometric navigation demonstration.

  Description:
  Implements fundamental kinematic routines for mobile robotics: Forward, Backward,
  Pivot Turn Left/Right (wide curve), and Point Spin Left/Right (zero turning radius).
  Executes a timed 4-sided square demonstration followed by wide curve maneuvers.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors (Nominal: 3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply: 6.0V - 7.4V (e.g. 4x AA Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Driver / Component Pin   Arduino UNO Pin   Function
  -------------------------------------------------------------
  ENA                      Pin 5 (PWM)       Left Motor Speed Enable (Timer0)
  IN1                      Pin 2             Left Motor Direction Input 1
  IN2                      Pin 3             Left Motor Direction Input 2
  IN3                      Pin 4             Right Motor Direction Input 1
  IN4                      Pin 7             Right Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor Speed Enable (Timer0)
  VCC2 / VM                Battery (+)       Motor Power (6.0V - 7.4V Recommended)
  GND                      GND & Batt (-)    Common Ground Busbar (Mandatory)
  -------------------------------------------------------------

  Kinematic Working Principle:
  Differential drive kinematics relies on the relative velocities of the left (v_L) and right (v_R) wheels:
  - Linear Velocity:    v = (v_R + v_L) / 2
  - Angular Velocity:   omega = (v_R - v_L) / L  (where L is the wheelbase axle track width)
  - Straight Forward:   v_L = +v, v_R = +v  (omega = 0)
  - Straight Reverse:   v_L = -v, v_R = -v  (omega = 0)
  - Pivot Turn Left:    v_L = 0,  v_R = +v  (Instantaneous center of rotation on Left wheel)
  - Pivot Turn Right:   v_L = +v, v_R = 0   (Instantaneous center of rotation on Right wheel)
  - Point Spin Left:    v_L = -v, v_R = +v  (Zero turning radius, rotation about robot midpoint)
  - Point Spin Right:   v_L = +v, v_R = -v  (Zero turning radius, rotation about robot midpoint)

  Expected Behavior:
  1. Section 1 (Open-Loop Square Demonstration): Robot executes 4 legs of timed forward driving (2000ms)
     and timed right point turns (600ms).
  2. Section 2 (Pivot & Reverse): Robot demonstrates wide pivot curves and a straight reverse pull.
  3. Telemetry is streamed to the Serial Monitor at 9600 baud.

  Kinematic & Calibration Limitations:
  - This program demonstrates OPEN-LOOP timed motion primitives.
  - A mathematically perfect closed square CANNOT be guaranteed without closed-loop feedback (e.g. optical
    wheel encoders or an IMU), as real trajectories drift due to surface friction, battery voltage sag,
    and slight mechanical gearbox speed variances.
  - `TURN_TIME_MS` is an empirical value requiring manual physical calibration for each floor surface.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins
#define ENA 5   // Timer0 PWM
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6   // Timer0 PWM
#define IN3 4
#define IN4 7

// =====================================================
// SPEED & TIMING CONSTANTS
// =====================================================

const int CRUISE_SPEED = 200; // Base forward/reverse speed (PWM 0-255)
const int TURN_SPEED   = 180; // Turn/spin speed (PWM 0-255)

const int STRAIGHT_TIME_MS = 2000;
const int TURN_TIME_MS     = 600;  // Empirical 90-degree turn estimate (Requires physical calibration)
const int PIVOT_TIME_MS    = 1500;
const int SETTLE_TIME_MS   = 400;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void moveForward(int speed, int durationMs);
void moveBackward(int speed, int durationMs);
void turnLeft(int speed, int durationMs);
void turnRight(int speed, int durationMs);
void spinLeft(int speed, int durationMs);
void spinRight(int speed, int durationMs);
void stopRobot(int durationMs);

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Safe initialization
  stopRobot(500);

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.4: Robocar Kinematic Maneuvers"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.println(F("[INFO] Open-Loop Kinematic Demonstration starting in 3s...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  Serial.println(F("--- 1. Open-Loop Square Demonstration Drill ---"));

  // Traverse a 4-sided square trajectory (Open-loop timed approximation)
  for (int side = 1; side <= 4; side++) {
    Serial.print(F("[NAV] Traversing Leg "));
    Serial.print(side);
    Serial.println(F(" (Timed Forward 2000ms)..."));
    
    // 1. Move straight (blocking delay for demonstration)
    moveForward(CRUISE_SPEED, STRAIGHT_TIME_MS);
    stopRobot(SETTLE_TIME_MS);

    // 2. 90-degree right point turn (empirical calibration)
    Serial.println(F("[NAV] Executing Point Turn Right (~90 deg, 600ms)..."));
    spinRight(TURN_SPEED, TURN_TIME_MS);
    stopRobot(SETTLE_TIME_MS);
  }

  Serial.println(F("[NAV] Four-sided timed pattern cycle completed.\n"));
  delay(2000);

  // ---------------------------------------------------
  // Section 2: Wide Pivot Curve & Reversal Demonstration
  // ---------------------------------------------------
  Serial.println(F("--- 2. Demonstrating Wide Pivot Turns & Reversal ---"));

  Serial.println(F("[NAV] Pivot Turn Left (Right wheel forward, Left wheel stopped 1500ms)..."));
  turnLeft(CRUISE_SPEED, PIVOT_TIME_MS);
  stopRobot(SETTLE_TIME_MS);

  Serial.println(F("[NAV] Pivot Turn Right (Left wheel forward, Right wheel stopped 1500ms)..."));
  turnRight(CRUISE_SPEED, PIVOT_TIME_MS);
  stopRobot(SETTLE_TIME_MS);

  Serial.println(F("[NAV] Reversing back along path (2000ms)..."));
  moveBackward(CRUISE_SPEED, STRAIGHT_TIME_MS);
  stopRobot(3000);
}

// =====================================================
// KINEMATIC MOVEMENT PRIMITIVES
// =====================================================

void moveForward(int speed, int durationMs) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  if (durationMs > 0) delay(durationMs);
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

void turnLeft(int speed, int durationMs) {
  // Left motor stopped, right motor drives forward (Pivot Turn)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, speed);
  if (durationMs > 0) delay(durationMs);
}

void turnRight(int speed, int durationMs) {
  // Left motor drives forward, right motor stopped (Pivot Turn)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, 0);
  if (durationMs > 0) delay(durationMs);
}

void spinLeft(int speed, int durationMs) {
  // Zero-radius point spin: Left reverses, Right drives forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  if (durationMs > 0) delay(durationMs);
}

void spinRight(int speed, int durationMs) {
  // Zero-radius point spin: Left drives forward, Right reverses
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
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
