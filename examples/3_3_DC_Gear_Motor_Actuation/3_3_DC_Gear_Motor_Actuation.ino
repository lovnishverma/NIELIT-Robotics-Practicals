/*
  NIELIT Robotics Practicals
  Practical 3.3: Direct Current Actuation — Interfacing DC Gear Motors

  Objective:
  Interface and independently control dual DC yellow BO gear motors on a 2WD chassis,
  testing forward drive, reverse drive, single-wheel turns, and in-place spins.

  Wiring (Arduino to L298N Motor Driver):
  - Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  - Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  - Motor Power:  6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
  - Common GND:   Arduino GND connected to Battery (-)
*/

// Left Motor Pins
const int PIN_ENA = 5; // Left Speed (PWM)
const int PIN_IN1 = 2; // Left Dir 1
const int PIN_IN2 = 3; // Left Dir 2

// Right Motor Pins
const int PIN_ENB = 6; // Right Speed (PWM)
const int PIN_IN3 = 4; // Right Dir 1
const int PIN_IN4 = 7; // Right Dir 2

// Speed and Timing Settings
const int DRIVE_SPEED = 220; // Driving speed (0 - 255)
const int TURN_SPEED  = 200; // Turning speed (0 - 255)

// Motor Speed Trim (Adjust if one motor spins faster)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

const int MOVE_TIME_MS  = 2000; // Duration for each move
const int PAUSE_TIME_MS = 800;  // Pause to protect plastic gears

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopMotors();

  Serial.println("NIELIT Robotics Practical 3.3");
  Serial.println("DC Gear Motor Movement Tests");
  Serial.println("Starting in 2 seconds...\n");
  delay(2000);
}

void loop() {
  // 1. Both Motors Forward
  Serial.println("[Move 1] Both Motors FORWARD -> Robot drives forward");
  setLeftMotor(true, DRIVE_SPEED);
  setRightMotor(true, DRIVE_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 2. Both Motors Reverse
  Serial.println("[Move 2] Both Motors REVERSE -> Robot drives backward");
  setLeftMotor(false, DRIVE_SPEED);
  setRightMotor(false, DRIVE_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 3. Left Motor Only (Forward)
  Serial.println("[Move 3] Left Motor ONLY Forward -> Robot turns RIGHT");
  setLeftMotor(true, TURN_SPEED);
  setRightMotor(true, 0);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 4. Right Motor Only (Forward)
  Serial.println("[Move 4] Right Motor ONLY Forward -> Robot turns LEFT");
  setLeftMotor(true, 0);
  setRightMotor(true, TURN_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 5. In-Place Spin Left
  Serial.println("[Move 5] In-Place SPIN LEFT (Left Reverse, Right Forward)");
  setLeftMotor(false, TURN_SPEED);
  setRightMotor(true, TURN_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // 6. In-Place Spin Right
  Serial.println("[Move 6] In-Place SPIN RIGHT (Left Forward, Right Reverse)");
  setLeftMotor(true, TURN_SPEED);
  setRightMotor(false, TURN_SPEED);
  delay(MOVE_TIME_MS);
  stopMotors();

  Serial.println("\nMovement sequence complete. Repeating in 4 seconds...\n");
  delay(4000);
}

// Motor Control Helper Functions

void setLeftMotor(bool forward, int speed) {
  int actualSpeed = constrain(speed + LEFT_TRIM, 0, 255);
  if (actualSpeed == 0) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, 0);
  } else {
    digitalWrite(PIN_IN1, forward ? HIGH : LOW);
    digitalWrite(PIN_IN2, forward ? LOW : HIGH);
    analogWrite(PIN_ENA, actualSpeed);
  }
}

void setRightMotor(bool forward, int speed) {
  int actualSpeed = constrain(speed + RIGHT_TRIM, 0, 255);
  if (actualSpeed == 0) {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, 0);
  } else {
    digitalWrite(PIN_IN3, forward ? HIGH : LOW);
    digitalWrite(PIN_IN4, forward ? LOW : HIGH);
    analogWrite(PIN_ENB, actualSpeed);
  }
}

void stopMotors() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}
