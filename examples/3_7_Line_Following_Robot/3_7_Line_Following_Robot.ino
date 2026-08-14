/*
  NIELIT Robotics Practicals
  Practical 3.7: Autonomous Navigation Capstone I — Line-Following Robot

  Objective:
  Build an autonomous line-following robot using two TCRT5000 Infrared (IR)
  sensors and the L293D Motor Shield.

  Wiring on L293D Motor Shield:
  - Left IR Sensor (OUT)  -> Analog Pin A0
  - Right IR Sensor (OUT) -> Analog Pin A1
  - Sensors VCC / GND     -> 5V / GND on the shield's analog breakout row
  - Left Motor            -> Screw Terminal M1
  - Right Motor           -> Screw Terminal M2
  - Battery Pack          -> EXT_PWR (+M and GND) on shield
*/

#include <AFMotor.h>

// Connect Left Motor to M1, Right Motor to M2
AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

// IR Sensor Pins (connected to Analog header on shield)
const int PIN_LEFT_SENSOR  = A0;
const int PIN_RIGHT_SENSOR = A1;

// Speed Settings
const int FORWARD_SPEED = 160; // Straight line speed
const int TURN_FAST     = 180; // Outside wheel speed when steering
const int TURN_SLOW     = 40;  // Inside wheel speed when steering

// Set true for standard black tape on white floor (sensor HIGH on black)
const bool BLACK_LINE_MODE = true;

// Motor Speed Trim
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  // Configure sensor pins
  pinMode(PIN_LEFT_SENSOR, INPUT);
  pinMode(PIN_RIGHT_SENSOR, INPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.7");
  Serial.println("L293D Shield - Autonomous Line Following Robot");
  Serial.println("Connect Left IR to A0, Right IR to A1.");
  Serial.println("Calibrate sensors: LED turns ON over black tape.");
  Serial.println("Starting in 3 seconds...\n");
  delay(3000);
}

void loop() {
  // Read both IR sensors
  int leftRaw  = digitalRead(PIN_LEFT_SENSOR);
  int rightRaw = digitalRead(PIN_RIGHT_SENSOR);

  bool leftOnBlack  = BLACK_LINE_MODE ? (leftRaw == HIGH)  : (leftRaw == LOW);
  bool rightOnBlack = BLACK_LINE_MODE ? (rightRaw == HIGH) : (rightRaw == LOW);

  // Decision 1: Both on white -> Centered -> Go straight
  if (!leftOnBlack && !rightOnBlack) {
    driveStraight(FORWARD_SPEED);
  }
  // Decision 2: Left on black -> Veering right -> Steer Left
  else if (leftOnBlack && !rightOnBlack) {
    steerLeft(TURN_FAST, TURN_SLOW);
  }
  // Decision 3: Right on black -> Veering left -> Steer Right
  else if (!leftOnBlack && rightOnBlack) {
    steerRight(TURN_FAST, TURN_SLOW);
  }
  // Decision 4: Both on black -> T-junction or Stop line -> Stop
  else if (leftOnBlack && rightOnBlack) {
    stopRobot();
  }

  delay(10); // Loop stability delay
}

// Motor Control Helper Functions

void driveStraight(int speed) {
  motorLeft.setSpeed(constrain(speed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(speed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}

void steerLeft(int fastSpeed, int slowSpeed) {
  motorLeft.setSpeed(constrain(slowSpeed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(fastSpeed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}

void steerRight(int fastSpeed, int slowSpeed) {
  motorLeft.setSpeed(constrain(fastSpeed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(slowSpeed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}

void stopRobot() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}
