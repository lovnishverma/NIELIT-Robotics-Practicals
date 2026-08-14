/*
  NIELIT Robotics Practicals
  Practical 3.1: Robotics Assembly & Power Distribution Dynamics

  Objective:
  Understand mobile robot chassis assembly, 2-wheel differential drive,
  power wiring (battery pack & common ground), and verify motor rotation direction.

  Wiring (Arduino to L298N Motor Driver):
  - Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  - Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  - Logic Power:  5V -> Arduino 5V, GND -> Arduino GND
  - Motor Power:  6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
  - Diagnostic:   Status LED -> Pin 13
*/

// Pin Definitions
const int PIN_ENA = 5; // Left Motor Speed (PWM)
const int PIN_IN1 = 2; // Left Motor Direction 1
const int PIN_IN2 = 3; // Left Motor Direction 2

const int PIN_ENB = 6; // Right Motor Speed (PWM)
const int PIN_IN3 = 4; // Right Motor Direction 1
const int PIN_IN4 = 7; // Right Motor Direction 2

const int STATUS_LED = 13;

// Speed and Calibration Settings
const int TEST_SPEED = 200; // Testing speed (0 to 255)

// Adjust if one motor spins faster than the other
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  // Configure motor driver and LED pins
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Start with motors stopped
  stopMotors();

  Serial.println("NIELIT Robotics Practical 3.1");
  Serial.println("Chassis Assembly & Power Pre-Flight Test");
  Serial.println("Place car on a stand with wheels in the air for testing.");
  Serial.println("Starting in 3 seconds...\n");

  // 3-second countdown with blinking LED
  for (int i = 3; i > 0; i--) {
    Serial.print("Starting in ");
    Serial.print(i);
    Serial.println("...");
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);
    delay(500);
  }
  Serial.println("\nStarting motor direction diagnostics...");
}

void loop() {
  // Test 1: Left Wheel Forward
  Serial.println("\n[Test 1] Left Wheel FORWARD");
  Serial.println("Check: Left wheel should turn forward.");
  Serial.println("If it turns backward, swap its two wires at the driver terminal.");
  setLeftMotor(true, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // Test 2: Left Wheel Reverse
  Serial.println("\n[Test 2] Left Wheel REVERSE");
  Serial.println("Check: Left wheel should turn backward.");
  setLeftMotor(false, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // Test 3: Right Wheel Forward
  Serial.println("\n[Test 3] Right Wheel FORWARD");
  Serial.println("Check: Right wheel should turn forward.");
  Serial.println("If it turns backward, swap its two wires at the driver terminal.");
  setRightMotor(true, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // Test 4: Right Wheel Reverse
  Serial.println("\n[Test 4] Right Wheel REVERSE");
  Serial.println("Check: Right wheel should turn backward.");
  setRightMotor(false, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // Test 5: Both Wheels Forward
  Serial.println("\n[Test 5] BOTH Wheels FORWARD");
  Serial.println("Both wheels should rotate together forward.");
  setLeftMotor(true, TEST_SPEED);
  setRightMotor(true, TEST_SPEED);
  delay(2500);
  stopMotors();
  delay(1000);

  // Test 6: Both Wheels Reverse
  Serial.println("\n[Test 6] BOTH Wheels REVERSE");
  Serial.println("Both wheels should rotate together backward.");
  setLeftMotor(false, TEST_SPEED);
  setRightMotor(false, TEST_SPEED);
  delay(2500);
  stopMotors();

  Serial.println("\nDiagnostics complete. Repeating in 5 seconds...\n");
  delay(5000);
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
