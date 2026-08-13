/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.1: Robotics Assembly & Power Distribution Dynamics
  =========================================================

  Objective:
  Understand mobile robot power architecture, separate logic and motor power rails,
  common ground dynamics, and verify dual-channel motor wiring polarity.

  Description:
  Demonstrates robotic chassis power distribution by isolating the microcontroller 5V
  logic rail from the high-current DC motor battery supply while maintaining a shared
  common ground reference. Executes a structured pre-flight diagnostic routine to verify
  forward and reverse rotation on both channels.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D Dual H-Bridge Motor Driver IC / Shield (or L298N Module)
  - 2x DC Yellow BO Gear Motors (3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply (6V - 12V Battery Pack)
  - Onboard Status LED (Pin 13)

  Pin Configuration:
  -------------------------------------------------------------
  Driver / Component Pin   Arduino UNO Pin   Function
  -------------------------------------------------------------
  ENA (1,2EN)              Pin 5 (PWM)       Left Motor Speed Enable
  IN1 (1A)                 Pin 2             Left Motor Direction A
  IN2 (2A)                 Pin 3             Left Motor Direction B
  IN3 (3A)                 Pin 4             Right Motor Direction A
  IN4 (4A)                 Pin 7             Right Motor Direction B
  ENB (3,4EN)              Pin 6 (PWM)       Right Motor Speed Enable
  VCC1 (Logic)             5V                Arduino 5V Regulated Supply
  VCC2 / VM (Motor)        Battery (+)       External Battery (+6V to +12V)
  GND (All GND pins)       GND & Batt (-)    Common Ground Busbar
  STATUS_LED               Pin 13            Visual Diagnostic Indicator
  -------------------------------------------------------------

  Working Principle:
  DC motors generate inductive current spikes and substantial stall current (up to 1A+).
  Drawing motor power from the Arduino 5V pin causes severe voltage sag and brownout resets.
  By powering motors directly from a dedicated battery pack and tying all ground rails together,
  clean logic levels are maintained across the H-bridge inputs.

  Expected Behavior:
  1. Startup: Status LED blinks during a 3-second power stabilization countdown.
  2. Phase 1: Left wheel rotates Forward for 1.5s, pauses, then rotates Reverse for 1.5s.
  3. Phase 2: Right wheel rotates Forward for 1.5s, pauses, then rotates Reverse for 1.5s.
  4. Phase 3: Both wheels drive Forward synchronously for 2.0s.
  5. Phase 4: Both wheels drive Reverse synchronously for 2.0s.
  6. Telemetry is logged to Serial Monitor at 9600 baud.

  Notes:
  - If a wheel rotates in reverse during a forward test phase, swap the two wire leads
    connected to that motor's driver terminals (OUT1/OUT2 or OUT3/OUT4).
  - Common ground connection between Arduino GND and Battery (-) is strictly mandatory.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins
#define ENA 5   // PWM Speed Enable
#define IN1 2   // Direction Pin A
#define IN2 3   // Direction Pin B

// Right Motor Driver Pins
#define ENB 6   // PWM Speed Enable
#define IN3 4   // Direction Pin A
#define IN4 7   // Direction Pin B

// Visual Diagnostic Indicator
#define STATUS_LED 13

// =====================================================
// SPEED & TIMING CONSTANTS
// =====================================================

const int TEST_SPEED_SINGLE = 200; // PWM duty cycle (~78%) for single-wheel verification
const int TEST_SPEED_DUAL   = 220; // PWM duty cycle (~86%) for dual-wheel thrust
const int PULSE_DELAY_MS    = 1500;
const int DUAL_DELAY_MS     = 2000;
const int PAUSE_DELAY_MS    = 1000;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void setLeftMotor(bool forward, int speed);
void setRightMotor(bool forward, int speed);
void stopAllMotors();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  // Configure control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Safe initialization: Ensure all motor outputs are pulled low
  stopAllMotors();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.1: Assembly & Power Check     "));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.println(F("[INFO] Logic Rail: 5V regulated from Arduino"));
  Serial.println(F("[INFO] Motor Rail: External Battery (Common GND mandatory)"));
  Serial.println(F("[INFO] Starting 3-second power stabilization countdown..."));

  for (int i = 3; i > 0; i--) {
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);
    delay(500);
    Serial.print(F("[INFO] Ready in: "));
    Serial.print(i);
    Serial.println(F("s"));
  }

  Serial.println(F("[INFO] Power stabilized. Commencing pre-flight diagnostics...\n"));
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // ---------------------------------------------------
  // Phase 1: Left Motor Channel Polarity Check
  // ---------------------------------------------------
  Serial.println(F("[DIAGNOSTIC] Phase 1A: Left Motor FORWARD (Verify CW rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(true, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  Serial.println(F("[DIAGNOSTIC] Phase 1B: Left Motor REVERSE (Verify CCW rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(false, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  // ---------------------------------------------------
  // Phase 2: Right Motor Channel Polarity Check
  // ---------------------------------------------------
  Serial.println(F("[DIAGNOSTIC] Phase 2A: Right Motor FORWARD (Verify CW rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setRightMotor(true, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  Serial.println(F("[DIAGNOSTIC] Phase 2B: Right Motor REVERSE (Verify CCW rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setRightMotor(false, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  // ---------------------------------------------------
  // Phase 3: Dual Motor Synchronized Forward Thrust
  // ---------------------------------------------------
  Serial.println(F("[DIAGNOSTIC] Phase 3: Dual Motor Synchronized FORWARD"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(true, TEST_SPEED_DUAL);
  setRightMotor(true, TEST_SPEED_DUAL);
  delay(DUAL_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  // ---------------------------------------------------
  // Phase 4: Dual Motor Synchronized Reverse Pull
  // ---------------------------------------------------
  Serial.println(F("[DIAGNOSTIC] Phase 4: Dual Motor Synchronized REVERSE"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(false, TEST_SPEED_DUAL);
  setRightMotor(false, TEST_SPEED_DUAL);
  delay(DUAL_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);

  Serial.println(F("\n[INFO] Diagnostic cycle complete. Pausing 5 seconds."));
  Serial.println(F("[NOTE] If any motor rotated backwards, swap its physical terminal leads.\n"));
  delay(5000);
}

// =====================================================
// MOTOR CONTROL PRIMITIVES
// =====================================================

void setLeftMotor(bool forward, int speed) {
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  analogWrite(ENA, constrain(speed, 0, 255));
}

void setRightMotor(bool forward, int speed) {
  if (forward) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  analogWrite(ENB, constrain(speed, 0, 255));
}

void stopAllMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
