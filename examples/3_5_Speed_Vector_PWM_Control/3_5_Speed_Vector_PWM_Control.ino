/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.5: Speed Vector Regulation — PWM Motor Control
  =========================================================

  Objective:
  Control the speed of a 2-wheel mobile robot using Pulse Width Modulation (PWM),
  test discrete speed presets, perform smooth acceleration/deceleration ramps,
  and calibrate motor speed trim for straight driving.

  ---------------------------------------------------------
  How PWM (Pulse Width Modulation) Controls Motor Speed:
  ---------------------------------------------------------
  Arduino analogWrite(pin, value) generates a PWM signal with values from 0 to 255:
  - value = 0   (0% Duty Cycle)   -> Motor is OFF (0V average)
  - value = 100 (~40% Duty Cycle) -> Slow / Crawl speed
  - value = 180 (~70% Duty Cycle) -> Medium / Cruising speed
  - value = 255 (100% Duty Cycle) -> Full Maximum speed

  Smooth Acceleration Ramp:
  Gradually increasing PWM from 0 to 255 prevents sudden high battery current surges
  and keeps the Arduino from resetting due to brownout voltage drops.

  ---------------------------------------------------------
  Pin Connections (L298N / Standard Motor Driver):
  ---------------------------------------------------------
  Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  Motor Power:  6.0V - 7.4V Battery Pack (+ to VM/12V, - to GND)
  Arduino GND:  Common Ground connected to Battery (-)

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

#include <NIELIT_Robotics_Practicals.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

const int PIN_ENA = 5; // PWM pin for Left Motor speed
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;

const int PIN_ENB = 6; // PWM pin for Right Motor speed
const int PIN_IN3 = 4;
const int PIN_IN4 = 7;

// =====================================================
// SPEED TRIM CALIBRATION
// =====================================================

// Adjust if one motor spins faster than the other
// Example: If robot veers right, increase LEFT_TRIM (e.g. +15) or decrease RIGHT_TRIM (-15)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void drive(bool forward, int speed);
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  NIELIT::Robotics::printPracticalHeader(Serial, "3.5", "PWM Speed Control & Acceleration Ramps");
  Serial.println(F("[INFO] Place car on a stand or clear open floor."));
  Serial.println(F("[INFO] Starting speed tests in 2 seconds...\n"));
  delay(2000);
}

// =====================================================
// MAIN LOOP: Speed Presets & Smooth Acceleration Ramps
// =====================================================

void loop() {
  Serial.println(F("--- SECTION 1: TESTING FIXED SPEED PRESETS ---"));

  // 1. Slow Speed (PWM = 100)
  Serial.println(F("[SPEED 1] SLOW Speed (PWM = 100 / ~39%)"));
  drive(true, 100);
  delay(2500);
  stopRobot();
  delay(1000);

  // 2. Medium Cruise Speed (PWM = 180)
  Serial.println(F("[SPEED 2] MEDIUM Cruise Speed (PWM = 180 / ~70%)"));
  drive(true, 180);
  delay(2500);
  stopRobot();
  delay(1000);

  // 3. Fast Speed (PWM = 220)
  Serial.println(F("[SPEED 3] FAST Speed (PWM = 220 / ~86%)"));
  drive(true, 220);
  delay(2500);
  stopRobot();
  delay(1000);

  // 4. Maximum Speed (PWM = 255)
  Serial.println(F("[SPEED 4] MAXIMUM Full Speed (PWM = 255 / 100%)"));
  drive(true, 255);
  delay(2500);
  stopRobot();
  delay(1500);

  Serial.println(F("\n--- SECTION 2: SMOOTH ACCELERATION RAMP ---"));
  Serial.println(F("[RAMP UP] Gradually increasing speed from 60 to 255..."));

  // Ramp Up from 60 to 255
  for (int pwm = 60; pwm <= 255; pwm += 5) {
    drive(true, pwm);
    Serial.print(F("PWM: "));
    Serial.print(pwm);
    Serial.print(F(" ("));
    Serial.print((pwm * 100) / 255);
    Serial.println(F("%)"));
    delay(50);
  }
  delay(1500);

  Serial.println(F("\n--- SECTION 3: SMOOTH DECELERATION RAMP ---"));
  Serial.println(F("[RAMP DOWN] Gradually decreasing speed from 255 to 60..."));

  // Ramp Down from 255 to 60
  for (int pwm = 255; pwm >= 60; pwm -= 5) {
    drive(true, pwm);
    Serial.print(F("PWM: "));
    Serial.print(pwm);
    Serial.print(F(" ("));
    Serial.print((pwm * 100) / 255);
    Serial.println(F("%)"));
    delay(50);
  }

  stopRobot();
  Serial.println(F("[STOP] Motors stopped safely."));

  Serial.println(F("\n[INFO] Speed test cycle completed. Pausing 5 seconds before repeating...\n"));
  delay(5000);
}

// =====================================================
// MOTOR DRIVING FUNCTIONS
// =====================================================

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
