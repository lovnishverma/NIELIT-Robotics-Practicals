/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.1: Robotics Assembly & Power Distribution Dynamics
  =========================================================

  Objective:
  Understand mobile robot chassis assembly, 2-wheel differential drive kinematics,
  power wiring (separate motor battery & common ground), and verify motor rotation polarity.

  ---------------------------------------------------------
  Understanding Your 2WD + Caster Wheel Chassis:
  ---------------------------------------------------------
  This robot uses a 2-Wheel Differential Drive system:
  - 2 Powered Yellow BO Motors (Left and Right wheels).
  - 1 Free-Wheeling Ball Caster (Passive balance wheel - has NO motor!).
  
  Steering is achieved by controlling the relative speed/direction of the 2 drive motors.
  The caster wheel simply rolls in any direction to keep the car balanced.

  Leveling Tip: Ensure both yellow rubber wheels and the caster wheel touch the floor
  firmly with equal pressure. If the caster is too tall, the drive wheels will lose grip!

  ---------------------------------------------------------
  Wiring & Pin Connections (L298N / Standard Motor Driver):
  ---------------------------------------------------------
  Arduino Pin   Driver Pin      Function
  ---------------------------------------------------------
  Pin 5 (PWM)   ENA             Left Motor Speed Enable
  Pin 2         IN1             Left Motor Direction A
  Pin 3         IN2             Left Motor Direction B
  Pin 4         IN3             Right Motor Direction A
  Pin 7         IN4             Right Motor Direction B
  Pin 6 (PWM)   ENB             Right Motor Speed Enable
  5V            VCC / 5V Logic  Arduino Regulated 5V Rail
  GND           GND & Batt (-)  Common Ground (MANDATORY!)
  Pin 13        Status LED      Visual Diagnostic Indicator
  ---------------------------------------------------------
  Motor Power: Connect 6.0V - 7.4V Battery Pack (+ to 12V/VM, - to GND).
  NEVER power DC motors directly from Arduino 5V!

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins
const int PIN_ENA = 5; // Left Speed PWM
const int PIN_IN1 = 2; // Left Direction 1
const int PIN_IN2 = 3; // Left Direction 2

// Right Motor Driver Pins
const int PIN_ENB = 6; // Right Speed PWM
const int PIN_IN3 = 4; // Right Direction 1
const int PIN_IN4 = 7; // Right Direction 2

// Visual Indicator
const int STATUS_LED = 13;

// =====================================================
// SPEED & CALIBRATION SETTINGS
// =====================================================

// Base testing speed (0 to 255)
const int TEST_SPEED = 200;

// Motor Speed Trim: Adjust if one motor spins faster than the other
// Example: If robot drifts right, increase LEFT_TRIM or decrease RIGHT_TRIM
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void setLeftMotor(bool forward, int speed);
void setRightMotor(bool forward, int speed);
void stopMotors();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  // Configure Motor Pins as Outputs
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Ensure motors start in STOP state
  stopMotors();

  // Print Welcome Banner
  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Robotics Practical 3.1"));
  Serial.println(F(" Chassis Assembly & Power Pre-Flight Test"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] Place car on a stand (wheels off table) for initial testing."));
  Serial.println(F("[INFO] Starting 3-second power countdown...\n"));

  // Blink LED during 3-second safety delay
  for (int i = 3; i > 0; i--) {
    Serial.print(F("Starting in "));
    Serial.print(i);
    Serial.println(F("..."));
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);
    delay(500);
  }
  Serial.println(F("\n--- COMMENCING MOTOR POLARITY DIAGNOSTICS ---"));
}

// =====================================================
// MAIN LOOP: Step-by-Step Diagnostic Test Sequence
// =====================================================

void loop() {
  // TEST 1: Left Wheel Forward
  Serial.println(F("\n[TEST 1] Testing LEFT Wheel FORWARD"));
  Serial.println(F(" -> Check: Left wheel should rotate forward."));
  Serial.println(F(" -> If it spins backward, swap its two terminal wires at the driver."));
  setLeftMotor(true, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // TEST 2: Left Wheel Reverse
  Serial.println(F("\n[TEST 2] Testing LEFT Wheel REVERSE"));
  Serial.println(F(" -> Check: Left wheel should rotate backward."));
  setLeftMotor(false, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // TEST 3: Right Wheel Forward
  Serial.println(F("\n[TEST 3] Testing RIGHT Wheel FORWARD"));
  Serial.println(F(" -> Check: Right wheel should rotate forward."));
  Serial.println(F(" -> If it spins backward, swap its two terminal wires at the driver."));
  setRightMotor(true, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // TEST 4: Right Wheel Reverse
  Serial.println(F("\n[TEST 4] Testing RIGHT Wheel REVERSE"));
  Serial.println(F(" -> Check: Right wheel should rotate backward."));
  setRightMotor(false, TEST_SPEED);
  delay(2000);
  stopMotors();
  delay(1000);

  // TEST 5: Both Wheels Forward (Forward Driving)
  Serial.println(F("\n[TEST 5] BOTH Wheels FORWARD"));
  Serial.println(F(" -> Both wheels rotating synchronously forward."));
  setLeftMotor(true, TEST_SPEED);
  setRightMotor(true, TEST_SPEED);
  delay(2500);
  stopMotors();
  delay(1000);

  // TEST 6: Both Wheels Reverse (Reverse Driving)
  Serial.println(F("\n[TEST 6] BOTH Wheels REVERSE"));
  Serial.println(F(" -> Both wheels rotating synchronously backward."));
  setLeftMotor(false, TEST_SPEED);
  setRightMotor(false, TEST_SPEED);
  delay(2500);
  stopMotors();

  Serial.println(F("\n[INFO] Diagnostics cycle complete. Pausing 5 seconds before repeating...\n"));
  delay(5000);
}

// =====================================================
// HELPER FUNCTIONS
// =====================================================

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
