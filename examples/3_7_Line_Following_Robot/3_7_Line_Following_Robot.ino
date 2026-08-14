/*
  NIELIT Robotics Practicals
  Practical 3.7: Autonomous Navigation Capstone I — Line-Following Robot

  Objective:
  Build an autonomous line-following robot using two TCRT5000 Infrared (IR)
  sensors to track a black electrical tape line on a white floor.

  Wiring:
  - Left IR Sensor (OUT)  -> Pin 2 (Digital Input)
  - Right IR Sensor (OUT) -> Pin 3 (Digital Input)
  - Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 8, IN2 -> Pin 9
  - Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 10, IN4 -> Pin 11
  - Sensor Power: 5V -> Arduino 5V, GND -> Arduino GND
  - Motor Power:  6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
*/

// IR Sensor Pins
const int PIN_LEFT_SENSOR  = 2;
const int PIN_RIGHT_SENSOR = 3;

// Motor Driver Pins
const int PIN_ENA = 5;  // Left Speed (PWM)
const int PIN_IN1 = 8;  // Left Direction 1
const int PIN_IN2 = 9;  // Left Direction 2

const int PIN_ENB = 6;  // Right Speed (PWM)
const int PIN_IN3 = 10; // Right Direction 1
const int PIN_IN4 = 11; // Right Direction 2

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

  // Configure motor pins
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.7");
  Serial.println("Autonomous Line Following Robot");
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
