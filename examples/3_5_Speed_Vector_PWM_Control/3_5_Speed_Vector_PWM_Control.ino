/*
  NIELIT Robotics Practicals
  Practical 3.5: Speed Vector Regulation — PWM Motor Control

  Objective:
  Control the speed of a 2-wheel mobile robot on an L293D shield using PWM,
  test discrete speed presets, and perform smooth acceleration/deceleration ramps.

  Hardware:
  - Arduino UNO with Blue L293D Motor Shield
  - Left Motor  -> M1
  - Right Motor -> M2
  - Battery Pack -> EXT_PWR (+M and GND)
*/

#include <AFMotor.h>

AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

// Motor Speed Trim (Adjust if robot veers right or left)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.5");
  Serial.println("L293D Shield - PWM Speed Control & Acceleration Ramps");
  Serial.println("Starting in 2 seconds...\n");
  delay(2000);
}

void loop() {
  Serial.println("--- Section 1: Testing Fixed Speed Presets ---");

  // 1. Slow Speed (PWM = 100)
  Serial.println("[Speed 1] SLOW Speed (PWM = 100 / ~39%)");
  drive(true, 100);
  delay(2500);
  stopRobot();
  delay(1000);

  // 2. Medium Cruise Speed (PWM = 180)
  Serial.println("[Speed 2] MEDIUM Cruise Speed (PWM = 180 / ~70%)");
  drive(true, 180);
  delay(2500);
  stopRobot();
  delay(1000);

  // 3. Fast Speed (PWM = 220)
  Serial.println("[Speed 3] FAST Speed (PWM = 220 / ~86%)");
  drive(true, 220);
  delay(2500);
  stopRobot();
  delay(1000);

  // 4. Maximum Speed (PWM = 255)
  Serial.println("[Speed 4] MAXIMUM Full Speed (PWM = 255 / 100%)");
  drive(true, 255);
  delay(2500);
  stopRobot();
  delay(1500);

  Serial.println("\n--- Section 2: Smooth Acceleration Ramp ---");
  Serial.println("Gradually increasing speed from 60 to 255...");

  for (int pwm = 60; pwm <= 255; pwm += 5) {
    drive(true, pwm);
    Serial.print("PWM: ");
    Serial.print(pwm);
    Serial.print(" (");
    Serial.print((pwm * 100) / 255);
    Serial.println("%)");
    delay(50);
  }
  delay(1500);

  Serial.println("\n--- Section 3: Smooth Deceleration Ramp ---");
  Serial.println("Gradually decreasing speed from 255 to 60...");

  for (int pwm = 255; pwm >= 60; pwm -= 5) {
    drive(true, pwm);
    Serial.print("PWM: ");
    Serial.print(pwm);
    Serial.print(" (");
    Serial.print((pwm * 100) / 255);
    Serial.println("%)");
    delay(50);
  }

  stopRobot();
  Serial.println("[Stop] Motors stopped safely.");

  Serial.println("\nSpeed tests complete. Repeating in 5 seconds...\n");
  delay(5000);
}

// Motor Driving Helper Functions

void drive(bool forward, int speed) {
  int actualLeft  = constrain(speed + LEFT_TRIM, 0, 255);
  int actualRight = constrain(speed + RIGHT_TRIM, 0, 255);

  motorLeft.setSpeed(actualLeft);
  motorRight.setSpeed(actualRight);

  if (forward) {
    motorLeft.run(FORWARD);
    motorRight.run(FORWARD);
  } else {
    motorLeft.run(BACKWARD);
    motorRight.run(BACKWARD);
  }
}

void stopRobot() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}
