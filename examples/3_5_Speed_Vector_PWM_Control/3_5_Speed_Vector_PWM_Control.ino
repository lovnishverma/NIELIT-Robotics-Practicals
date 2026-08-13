/*
  Practical 3.5: Speed Vector Regulation — PWM Motor Control
  Course: NIELIT Robotics Practicals

  Description:
  Demonstrates continuous velocity regulation of a differential drive robot
  using Pulse Width Modulation (PWM). Implements smooth acceleration/deceleration
  ramps, multi-tier speed presets (Low, Medium, High, Turbo), and differential
  trim calibration to ensure precise straight-line trajectory.

  PWM Duty Cycle Equation:
  Duty Cycle (%) = (AnalogWrite_Value / 255) * 100

  Hardware Connections:
  -------------------------------------------------------------
  L293D / Driver Pin    Arduino Pin      Description
  -------------------------------------------------------------
  ENA                   Pin 9 (PWM)      Left Motor Speed
  IN1                   Pin 5            Left Motor Input 1
  IN2                   Pin 6            Left Motor Input 2
  IN3                   Pin 10           Right Motor Input 1
  IN4                   Pin 11           Right Motor Input 2
  ENB                   Pin 3 (PWM)      Right Motor Speed
  VCC2 (VM)             +6V to +12V      Battery Pack Positive (+)
  GND                   GND              Common Ground
  -------------------------------------------------------------
*/

// Motor Driver Pins (ENA and ENB MUST be connected to PWM ~ pins)
const int ENA = 9;   // Left Motor PWM Speed
const int IN1 = 5;
const int IN2 = 6;

const int ENB = 3;   // Right Motor PWM Speed
const int IN3 = 10;
const int IN4 = 11;

// Motor Trim Offsets (Adjust if one motor runs faster than the other)
// Range: -50 to +50
const int LEFT_MOTOR_TRIM  = 0;
const int RIGHT_MOTOR_TRIM = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.5: PWM Speed Vector Regulation      "));
  Serial.println(F("=================================================="));
  Serial.println(F("Starting PWM speed vector test in 2 seconds...\n"));
  delay(2000);
}

void loop() {
  // --- Demonstration 1: Multi-tier Speed Steps ---
  Serial.println(F("--- 1. Testing Speed Presets ---"));
  
  Serial.println(F("Preset 1: Creep Speed (PWM: 90 / ~35% Duty Cycle)"));
  setRobotSpeedVector(true, 90);
  delay(2000);

  Serial.println(F("Preset 2: Cruise Speed (PWM: 160 / ~63% Duty Cycle)"));
  setRobotSpeedVector(true, 160);
  delay(2000);

  Serial.println(F("Preset 3: High Speed (PWM: 220 / ~86% Duty Cycle)"));
  setRobotSpeedVector(true, 220);
  delay(2000);

  Serial.println(F("Preset 4: Maximum Velocity (PWM: 255 / 100% Duty Cycle)"));
  setRobotSpeedVector(true, 255);
  delay(2000);

  stopRobot();
  delay(1500);

  // --- Demonstration 2: Smooth Acceleration Ramp (Soft Start) ---
  Serial.println(F("\n--- 2. Smooth Acceleration Ramp (Soft Start) ---"));
  setDirection(true); // Set Forward
  
  for (int pwm = 60; pwm <= 255; pwm += 5) {
    applyPWM(pwm, pwm);
    Serial.print(F("Accelerating -> PWM: "));
    Serial.print(pwm);
    Serial.print(F(" ("));
    Serial.print((pwm * 100) / 255);
    Serial.println(F("%)"));
    delay(40);
  }
  delay(1000);

  // --- Demonstration 3: Smooth Deceleration Ramp (Soft Stop) ---
  Serial.println(F("\n--- 3. Smooth Deceleration Ramp (Soft Stop) ---"));
  for (int pwm = 255; pwm >= 60; pwm -= 5) {
    applyPWM(pwm, pwm);
    Serial.print(F("Decelerating -> PWM: "));
    Serial.print(pwm);
    Serial.println(F(""));
    delay(40);
  }
  
  stopRobot();
  Serial.println(F("Vehicle brought to smooth stop.\n"));
  delay(3000);
}

// Applies PWM with hardware trim offsets
void applyPWM(int leftPWM, int rightPWM) {
  int adjustedLeft  = constrain(leftPWM + LEFT_MOTOR_TRIM, 0, 255);
  int adjustedRight = constrain(rightPWM + RIGHT_MOTOR_TRIM, 0, 255);
  
  analogWrite(ENA, adjustedLeft);
  analogWrite(ENB, adjustedRight);
}

// Configures H-bridge directions
void setDirection(bool forward) {
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

// Sets full speed vector (Direction + PWM magnitude)
void setRobotSpeedVector(bool forward, int pwmMagnitude) {
  setDirection(forward);
  applyPWM(pwmMagnitude, pwmMagnitude);
}

// Safe stop
void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
