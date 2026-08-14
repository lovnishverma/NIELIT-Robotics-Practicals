/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.7: Autonomous Navigation Capstone I — Line-Following Robot
  =========================================================

  Objective:
  Build an autonomous line-following robot using two TCRT5000 Infrared (IR)
  reflective sensors to track a black electrical tape line on a white floor.

  ---------------------------------------------------------
  How Line Following Works:
  ---------------------------------------------------------
  Two IR sensors are mounted side-by-side on the front underside of the chassis,
  spaced ~25mm apart so they straddle (sit on either side of) a 19mm black line.

        [Front of Robot]
        (Left IR)   (Right IR)
           │            │
      ═════╪════════════╪═════  (White Floor)
           │  ████████  │       (Black Line in Middle)

  IR Sensor Logic:
  - Over White Floor: IR light is reflected back -> Sensor Output = LOW (0)
  - Over Black Tape:  IR light is absorbed        -> Sensor Output = HIGH (1)

  Steering Decision Table:
  +-------------+--------------+--------------------------+-----------------------+
  | Left Sensor | Right Sensor | Robot Position           | Robot Action          |
  +-------------+--------------+--------------------------+-----------------------+
  | 0 (White)   | 0 (White)    | Centered on line         | Drive Straight Forward|
  | 1 (Black)   | 0 (White)    | Veered right (line left) | Steer Left            |
  | 0 (White)   | 1 (Black)    | Veered left (line right) | Steer Right           |
  | 1 (Black)   | 1 (Black)    | T-junction or Stop Line  | Full Stop             |
  +-------------+--------------+--------------------------+-----------------------+

  ---------------------------------------------------------
  Pin Connections (Line Follower Configuration):
  ---------------------------------------------------------
  Left IR Sensor (OUT)  -> Pin 2 (Digital Input)
  Right IR Sensor (OUT) -> Pin 3 (Digital Input)
  Left Motor Speed ENA  -> Pin 5 (PWM)
  Left Motor Dir IN1/2  -> Pin 8, Pin 9
  Right Motor Speed ENB -> Pin 6 (PWM)
  Right Motor Dir IN3/4 -> Pin 10, Pin 11
  Sensor VCC / GND      -> Arduino 5V / GND
  Motor Power           -> 6.0V - 7.4V Battery Pack

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// IR Sensor Pins
const int PIN_LEFT_SENSOR  = 2;
const int PIN_RIGHT_SENSOR = 3;

// Motor Driver Pins
const int PIN_ENA = 5;  // Left Speed PWM
const int PIN_IN1 = 8;  // Left Direction 1
const int PIN_IN2 = 9;  // Left Direction 2

const int PIN_ENB = 6;  // Right Speed PWM
const int PIN_IN3 = 10; // Right Direction 1
const int PIN_IN4 = 11; // Right Direction 2

// =====================================================
// SPEED & SENSOR CONFIGURATION
// =====================================================

// Speed constants (0 - 255)
const int FORWARD_SPEED = 160; // Normal straight speed
const int TURN_FAST     = 180; // Speed of outside wheel during a turn
const int TURN_SLOW     = 40;  // Speed of inside wheel during a turn

// Set true if sensor output is HIGH on Black line (Standard TCRT5000)
// Set false if your sensor module outputs LOW on Black line
const bool BLACK_LINE_MODE = true;

// Motor Speed Trim (Adjust if robot veers off-center on straight track)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void driveStraight(int speed);
void steerLeft(int fastSpeed, int slowSpeed);
void steerRight(int fastSpeed, int slowSpeed);
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  // Configure Sensor Pins as Inputs
  pinMode(PIN_LEFT_SENSOR, INPUT);
  pinMode(PIN_RIGHT_SENSOR, INPUT);

  // Configure Motor Pins as Outputs
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Robotics Practical 3.7"));
  Serial.println(F(" Autonomous Line Following Robot"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] Calibrate sensor potentiometers: LED should turn ON over black tape."));
  Serial.println(F("[INFO] Place robot on black line track. Starting in 3 seconds...\n"));
  delay(3000);
}

// =====================================================
// MAIN LOOP: Continuous IR Sensing & Differential Steering
// =====================================================

void loop() {
  // Read digital state of both IR sensors
  int leftRaw  = digitalRead(PIN_LEFT_SENSOR);
  int rightRaw = digitalRead(PIN_RIGHT_SENSOR);

  // Convert to boolean: true = on Black Line, false = on White Floor
  bool leftOnBlack  = BLACK_LINE_MODE ? (leftRaw == HIGH)  : (leftRaw == LOW);
  bool rightOnBlack = BLACK_LINE_MODE ? (rightRaw == HIGH) : (rightRaw == LOW);

  // Decision 1: Both sensors on White -> Robot centered -> Drive straight
  if (!leftOnBlack && !rightOnBlack) {
    driveStraight(FORWARD_SPEED);
  }
  // Decision 2: Left sensor on Black -> Robot veering right -> Steer Left
  else if (leftOnBlack && !rightOnBlack) {
    steerLeft(TURN_FAST, TURN_SLOW);
  }
  // Decision 3: Right sensor on Black -> Robot veering left -> Steer Right
  else if (!leftOnBlack && rightOnBlack) {
    steerRight(TURN_FAST, TURN_SLOW);
  }
  // Decision 4: Both sensors on Black -> T-Junction / End of track -> Stop
  else if (leftOnBlack && rightOnBlack) {
    stopRobot();
  }

  delay(10); // Small loop stability delay (10ms)
}

// =====================================================
// MOTOR CONTROL PRIMITIVES
// =====================================================

void driveStraight(int speed) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
}

void steerLeft(int fastSpeed, int slowSpeed) {
  int leftSpd  = constrain(slowSpeed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(fastSpeed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
}

void steerRight(int fastSpeed, int slowSpeed) {
  int leftSpd  = constrain(fastSpeed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(slowSpeed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
}

void stopRobot() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}
