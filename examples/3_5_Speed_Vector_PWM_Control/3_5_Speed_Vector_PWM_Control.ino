/*
  NIELIT Robotics Practicals
  Practical 3.5: Speed Vector Regulation — PWM Motor Control

  Objective:
  Control the speed of a 2-wheel mobile robot using Pulse Width Modulation (PWM),
  test discrete speed presets, perform smooth acceleration/deceleration ramps,
  and calibrate motor speed trim for straight driving.

  Wiring (Arduino to L298N Motor Driver):
  - Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  - Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  - Motor Power:  6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
  - Common GND:   Arduino GND connected to Battery (-)
*/

// Pin Definitions
const int PIN_ENA = 5; // Left Motor Speed (PWM)
const int PIN_IN1 = 2; // Left Motor Direction 1
const int PIN_IN2 = 3; // Left Motor Direction 2

const int PIN_ENB = 6; // Right Motor Speed (PWM)
const int PIN_IN3 = 4; // Right Motor Direction 1
const int PIN_IN4 = 7; // Right Motor Direction 2

// Motor Speed Trim (Adjust if robot veers right or left)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.5");
  Serial.println("PWM Speed Control & Acceleration Ramps");
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

  if (forward) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  }

  analogWrite(PIN_ENA, actualLeft);
  analogWrite(PIN_ENB, actualRight);
}

void stopRobot() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}
