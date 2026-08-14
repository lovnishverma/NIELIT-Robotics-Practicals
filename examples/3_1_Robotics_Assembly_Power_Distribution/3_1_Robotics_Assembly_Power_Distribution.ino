/*
  NIELIT Robotics Practicals
  Practical 3.1: Robotics Assembly & Power Distribution Dynamics

  Objective:
  Understand 2WD chassis assembly, differential drive kinematics,
  power wiring with the L293D Motor Shield, and verify motor rotation direction.

  Hardware:
  - Arduino UNO with Blue L293D Motor Driver Shield plugged on top
  - 2x DC Yellow BO Gear Motors:
      * Left Motor  -> Screw Terminal M1
      * Right Motor -> Screw Terminal M2
  - 6V - 7.4V Battery Pack -> EXT_PWR (+M and GND) on shield
  - Onboard Status LED (Pin 13)
*/

#include <AFMotor.h>

// Connect Left Motor to M1, Right Motor to M2
AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

const int STATUS_LED = 13;

// Base testing speed (0 to 255)
const int TEST_SPEED = 200;

// Motor Speed Trim (Adjust if one motor spins faster than the other)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  pinMode(STATUS_LED, OUTPUT);

  // Initialize motors in stopped state
  motorLeft.setSpeed(TEST_SPEED);
  motorRight.setSpeed(TEST_SPEED);
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);

  Serial.println("NIELIT Robotics Practical 3.1");
  Serial.println("L293D Motor Shield - Chassis Assembly & Pre-Flight Test");
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
  Serial.println("If it turns backward, swap its two wires at screw terminal M1.");
  motorLeft.setSpeed(constrain(TEST_SPEED + LEFT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  delay(2000);
  motorLeft.run(RELEASE);
  delay(1000);

  // Test 2: Left Wheel Reverse
  Serial.println("\n[Test 2] Left Wheel REVERSE");
  Serial.println("Check: Left wheel should turn backward.");
  motorLeft.setSpeed(constrain(TEST_SPEED + LEFT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  delay(2000);
  motorLeft.run(RELEASE);
  delay(1000);

  // Test 3: Right Wheel Forward
  Serial.println("\n[Test 3] Right Wheel FORWARD");
  Serial.println("Check: Right wheel should turn forward.");
  Serial.println("If it turns backward, swap its two wires at screw terminal M2.");
  motorRight.setSpeed(constrain(TEST_SPEED + RIGHT_TRIM, 0, 255));
  motorRight.run(FORWARD);
  delay(2000);
  motorRight.run(RELEASE);
  delay(1000);

  // Test 4: Right Wheel Reverse
  Serial.println("\n[Test 4] Right Wheel REVERSE");
  Serial.println("Check: Right wheel should turn backward.");
  motorRight.setSpeed(constrain(TEST_SPEED + RIGHT_TRIM, 0, 255));
  motorRight.run(BACKWARD);
  delay(2000);
  motorRight.run(RELEASE);
  delay(1000);

  // Test 5: Both Wheels Forward
  Serial.println("\n[Test 5] BOTH Wheels FORWARD");
  Serial.println("Both wheels should rotate together forward.");
  motorLeft.setSpeed(constrain(TEST_SPEED + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(TEST_SPEED + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  delay(2500);
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
  delay(1000);

  // Test 6: Both Wheels Reverse
  Serial.println("\n[Test 6] BOTH Wheels REVERSE");
  Serial.println("Both wheels should rotate together backward.");
  motorLeft.setSpeed(constrain(TEST_SPEED + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(TEST_SPEED + RIGHT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  delay(2500);
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);

  Serial.println("\nDiagnostics complete. Repeating in 5 seconds...\n");
  delay(5000);
}
