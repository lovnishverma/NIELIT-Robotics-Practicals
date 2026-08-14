/*
  NIELIT Robotics Practicals
  Practical 3.2: Motor Driver Control — Interfacing the L293D Motor Driver Shield

  Objective:
  Understand how the L293D Motor Shield controls a DC motor through
  different states: Forward, Reverse, Release (Stop), and PWM Speed Control.

  Hardware:
  - Arduino UNO with Blue L293D Motor Driver Shield
  - 1x DC Yellow BO Gear Motor -> Connected to Screw Terminal M1
  - 6V - 7.4V Battery Pack -> Connected to EXT_PWR (+M and GND) on shield
*/

#include <AFMotor.h>

// Connect DC Motor to screw terminal M1
AF_DCMotor motor(1);

void setup() {
  Serial.begin(9600);

  // Initialize motor in stopped state
  motor.setSpeed(255);
  motor.run(RELEASE);

  Serial.println("NIELIT Robotics Practical 3.2");
  Serial.println("L293D Motor Driver Shield Demonstration");
  Serial.println("Starting in 2 seconds...\n");
  delay(2000);
}

void loop() {
  // State 1: Forward Rotation at Full Speed
  Serial.println("[State 1] FORWARD at Full Speed (PWM = 255)");
  motor.setSpeed(255);
  motor.run(FORWARD);
  delay(3000);

  // State 2: Stop / Release
  Serial.println("[State 2] STOP / RELEASE (Motor power disconnected)");
  motor.run(RELEASE);
  delay(1500);

  // State 3: Reverse Rotation at Full Speed
  Serial.println("[State 3] REVERSE at Full Speed (PWM = 255)");
  motor.setSpeed(255);
  motor.run(BACKWARD);
  delay(3000);

  // State 4: Stop / Release
  Serial.println("[State 4] STOP / RELEASE");
  motor.run(RELEASE);
  delay(1500);

  // State 5: Speed Control (50% Half Speed)
  Serial.println("[State 5] PWM SPEED CONTROL (Forward at 50% Speed: PWM = 128)");
  motor.setSpeed(128);
  motor.run(FORWARD);
  delay(3000);

  // Stop motor
  motor.run(RELEASE);

  Serial.println("\nDemonstration complete. Repeating in 4 seconds...\n");
  delay(4000);
}
