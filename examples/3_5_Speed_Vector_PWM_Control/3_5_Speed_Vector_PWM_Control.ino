/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.5: Speed Vector Regulation — PWM Motor Control
  =========================================================

  Objective:
  Regulate mobile robot velocity using Pulse Width Modulation (PWM), implement soft acceleration
  and deceleration ramps to mitigate inrush currents, and calibrate differential motor trim for straight-line tracking.

  Description:
  Demonstrates continuous velocity vector regulation using Arduino timer PWM pins (D5 and D6).
  Tests multi-tier speed presets (Creep, Cruise, Fast, Maximum), performs smooth acceleration/deceleration
  ramps, and provides hardware trim adjustment offsets to balance physical differences between gearboxes.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply (6V - 12V Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  Driver / Component Pin   Arduino UNO Pin   Function
  -------------------------------------------------------------
  ENA                      Pin 5 (PWM)       Left Motor PWM Speed
  IN1                      Pin 2             Left Motor Direction Input 1
  IN2                      Pin 3             Left Motor Direction Input 2
  IN3                      Pin 4             Right Motor Direction Input 1
  IN4                      Pin 7             Right Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor PWM Speed
  VCC2 / VM                Battery (+)       Motor Power Supply (+6V to +12V)
  GND                      GND & Batt (-)    Common Ground Busbar
  -------------------------------------------------------------

  Working Principle:
  DC motor rotational speed is proportional to the average voltage across its terminals.
  Pulse Width Modulation (PWM) rapidly chops the DC supply voltage at a fixed frequency (~980 Hz on Timer0).
  By adjusting the duty cycle ($D = \frac{T_{on}}{T_{total}} \times 100\%$), the effective motor voltage
  is precisely controlled from 0V ($D=0\%$) to VM ($D=100\%$).
  Soft starting ramps avoid sudden current surges that cause battery voltage collapse and brownout resets.

  Expected Behavior:
  1. Section 1 (Presets): Robot tests 4 discrete speed tiers (PWM: 90, 160, 220, 255) for 2.0s each.
  2. Section 2 (Soft Start): Robot smoothly accelerates from PWM 60 to 255 in 5-unit increments.
  3. Section 3 (Soft Stop): Robot smoothly decelerates from PWM 255 down to 60, then halts cleanly.
  4. Real-time PWM percentages are streamed to Serial Monitor at 9600 baud.

  Notes:
  - If the robot veers to one side during straight forward drive due to motor manufacturing tolerances,
    adjust `LEFT_MOTOR_TRIM` or `RIGHT_MOTOR_TRIM` (range: -50 to +50) to achieve balanced trajectory.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins (ENA must be a hardware PWM pin: D5)
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Driver Pins (ENB must be a hardware PWM pin: D6)
#define ENB 6
#define IN3 4
#define IN4 7

// =====================================================
// MOTOR TRIM CALIBRATION
// =====================================================

// Adjust if one motor rotates faster than the other
// Example: If robot veers slightly right, increase LEFT_MOTOR_TRIM or decrease RIGHT_MOTOR_TRIM
const int LEFT_MOTOR_TRIM  = 0;
const int RIGHT_MOTOR_TRIM = 0;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void applyPWM(int leftPWM, int rightPWM);
void setDirection(bool forward);
void setRobotSpeedVector(bool forward, int pwmMagnitude);
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Safe initialization
  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.5: PWM Speed Vector Regulation"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.println(F("[INFO] Hardware PWM Channels: ENA=Pin5, ENB=Pin6 (Timer0 ~980Hz)"));
  Serial.println(F("[INFO] Starting PWM speed vector test in 2s...\n"));
  delay(2000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // ---------------------------------------------------
  // Section 1: Multi-Tier Speed Presets
  // ---------------------------------------------------
  Serial.println(F("--- 1. Testing Discrete Speed Presets ---"));

  Serial.println(F("[PWM] Tier 1: Creep Velocity (PWM: 90 / ~35% Duty Cycle)"));
  setRobotSpeedVector(true, 90);
  delay(2000);

  Serial.println(F("[PWM] Tier 2: Cruise Velocity (PWM: 160 / ~63% Duty Cycle)"));
  setRobotSpeedVector(true, 160);
  delay(2000);

  Serial.println(F("[PWM] Tier 3: High Velocity (PWM: 220 / ~86% Duty Cycle)"));
  setRobotSpeedVector(true, 220);
  delay(2000);

  Serial.println(F("[PWM] Tier 4: Maximum Velocity (PWM: 255 / 100% Duty Cycle)"));
  setRobotSpeedVector(true, 255);
  delay(2000);

  stopRobot();
  delay(1500);

  // ---------------------------------------------------
  // Section 2: Smooth Acceleration Ramp (Soft Start)
  // ---------------------------------------------------
  Serial.println(F("\n--- 2. Smooth Acceleration Ramp (Soft Start) ---"));
  setDirection(true); // Establish forward H-bridge state

  for (int pwm = 60; pwm <= 255; pwm += 5) {
    applyPWM(pwm, pwm);
    Serial.print(F("[RAMP UP] PWM: "));
    Serial.print(pwm);
    Serial.print(F(" ("));
    Serial.print((pwm * 100) / 255);
    Serial.println(F("%)"));
    delay(40);
  }
  delay(1000);

  // ---------------------------------------------------
  // Section 3: Smooth Deceleration Ramp (Soft Stop)
  // ---------------------------------------------------
  Serial.println(F("\n--- 3. Smooth Deceleration Ramp (Soft Stop) ---"));
  for (int pwm = 255; pwm >= 60; pwm -= 5) {
    applyPWM(pwm, pwm);
    Serial.print(F("[RAMP DOWN] PWM: "));
    Serial.print(pwm);
    Serial.println(F(""));
    delay(40);
  }

  stopRobot();
  Serial.println(F("[INFO] Vehicle brought to smooth stop.\n"));
  delay(3000);
}

// =====================================================
// PWM & VECTOR CONTROL HELPERS
// =====================================================

void applyPWM(int leftPWM, int rightPWM) {
  int adjustedLeft  = constrain(leftPWM + LEFT_MOTOR_TRIM, 0, 255);
  int adjustedRight = constrain(rightPWM + RIGHT_MOTOR_TRIM, 0, 255);

  analogWrite(ENA, adjustedLeft);
  analogWrite(ENB, adjustedRight);
}

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

void setRobotSpeedVector(bool forward, int pwmMagnitude) {
  setDirection(forward);
  applyPWM(pwmMagnitude, pwmMagnitude);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
