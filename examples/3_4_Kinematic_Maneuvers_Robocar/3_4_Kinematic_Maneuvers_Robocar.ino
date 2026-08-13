/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.4: Kinematic Maneuvers — 2-Wheel Robocar Basic Movements
  =========================================================

  Objective:
  Implement modular kinematic motion primitives for a 2-wheel differential drive mobile robot
  and execute autonomous geometric path navigation drills.

  Description:
  Implements fundamental kinematic routines for mobile robotics: Forward, Backward,
  Pivot Turn Left/Right (wide curve), and Point Spin Left/Right (zero turning radius).
  Executes an autonomous 4-sided square navigation drill followed by an S-curve demonstration.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply (6V - 12V Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Driver / Component Pin   Arduino UNO Pin   Function
  -------------------------------------------------------------
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
  Differential drive kinematics relies on the relative velocities of the left and right wheels:
  - Straight Forward:     Left = +v, Right = +v (Equal forward speeds)
  - Straight Reverse:     Left = -v, Right = -v (Equal reverse speeds)
  - Pivot Turn Left:      Left = 0,  Right = +v (Left stationary, Right drives forward)
  - Pivot Turn Right:     Left = +v, Right = 0  (Left drives forward, Right stationary)
  - Point Spin Left:      Left = -v, Right = +v (Counter-rotating, zero radius turn)
  - Point Spin Right:     Left = +v, Right = -v (Counter-rotating, zero radius turn)

  Expected Behavior:
  1. Section 1 (Square Drill): Robot moves forward for 2.0s, pauses, executes a 90-degree point turn
     right (600ms), and repeats this 4 times to trace an enclosed rectangular path.
  2. Section 2 (Pivot & Reverse): Robot demonstrates wide pivot curves left and right, then reverses 2.0s.
  3. Telemetry is streamed to the Serial Monitor at 9600 baud.

  Notes:
  - 90-degree turn timing depends on wheel diameter, axle track width, and floor friction.
    The `TURN_TIME_MS` constant can be calibrated to achieve exact 90-degree corners.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6
#define IN3 4
#define IN4 7

// =====================================================
// SPEED & TIMING CONSTANTS
// =====================================================

const int CRUISE_SPEED = 200; // Base forward/reverse speed (PWM 0-255)
const int TURN_SPEED   = 180; // Turn/spin speed (PWM 0-255)

const int STRAIGHT_TIME_MS = 2000;
const int TURN_TIME_MS     = 600;  // Calibrate for exact 90-degree corner
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
  Serial.println(F("[INFO] Starting autonomous navigation drill in 3s...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  Serial.println(F("--- 1. Autonomous Square Navigation Drill ---"));

  // Traverse a 4-sided square trajectory
  for (int side = 1; side <= 4; side++) {
    Serial.print(F("[NAV] Traversing Side "));
    Serial.print(side);
    Serial.println(F(" (Straight Forward 2000ms)..."));
    
    // 1. Move straight
    moveForward(CRUISE_SPEED, STRAIGHT_TIME_MS);
    stopRobot(SETTLE_TIME_MS);

    // 2. 90-degree right point turn
    Serial.println(F("[NAV] Executing 90-degree Point Turn Right (600ms)..."));
    spinRight(TURN_SPEED, TURN_TIME_MS);
    stopRobot(SETTLE_TIME_MS);
  }

  Serial.println(F("[NAV] Square trajectory completed successfully!\n"));
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

  Serial.println(F("[NAV] Reversing back to starting position (2000ms)..."));
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
