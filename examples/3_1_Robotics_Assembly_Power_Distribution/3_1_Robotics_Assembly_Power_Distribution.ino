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
  logical forward and reverse rotation on both channels.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D Dual H-Bridge Motor Driver IC / Shield (or L298N Module with flyback diodes)
  - 2x DC Yellow BO Gear Motors (Nominal: 3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply: 6.0V - 7.4V (e.g. 4x AA 1.5V batteries or 2S Li-Ion with buck regulator)
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
  VCC1 / VSS (Logic)       5V                Arduino 5V Regulated Supply
  VCC2 / VM (Motor)        Battery (+)       Motor Supply Rail (6.0V - 7.4V Recommended)
  GND (All GND pins)       GND & Batt (-)    Common Ground Busbar (Mandatory)
  STATUS_LED               Pin 13            Visual Diagnostic Indicator
  -------------------------------------------------------------

  Electrical & Power Constraints:
  - DC BO gear motors are rated for 3V to 6V DC.
  - The L293D driver IC introduces an internal bipolar saturation voltage drop (V_CE,sat ~1.4V to 1.8V).
    A 6.0V to 7.4V battery pack delivers approximately 4.5V to 5.6V directly across the motor terminals.
  - Drawing motor current from the Arduino 5V header will cause voltage collapse and microcontroller resets.
  - A common ground between the Arduino GND and the external battery negative terminal is mandatory
    to establish a shared reference voltage for TTL logic signals (IN1-IN4, ENA, ENB).

  Expected Behavior:
  1. Startup: Status LED blinks during a 3-second power stabilization countdown (blocking delay used for sequence).
  2. Phase 1: Left wheel drives in Logical Forward for 1.5s, pauses, then drives in Logical Reverse for 1.5s.
  3. Phase 2: Right wheel drives in Logical Forward for 1.5s, pauses, then drives in Logical Reverse for 1.5s.
  4. Phase 3: Both wheels drive in Logical Forward synchronously for 2.0s.
  5. Phase 4: Both wheels drive in Logical Reverse synchronously for 2.0s.
  6. Diagnostic status messages are streamed to the Serial Monitor at 9600 baud.

  Hardware Notes:
  - Because left and right motors are mounted in mirror symmetry on opposite sides of the chassis,
    their output shafts rotate in physically opposite rotational directions (one clockwise, one counter-clockwise)
    to drive the vehicle chassis in the same linear direction.
  - If a wheel rotates backwards during its forward diagnostic phase, swap the two physical terminal
    leads of that motor at the driver output terminals (OUT1/OUT2 or OUT3/OUT4).

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Left Motor Driver Pins
#define ENA 5   // PWM Speed Enable (Timer0, ~976 Hz)
#define IN1 2   // Direction Pin A
#define IN2 3   // Direction Pin B

// Right Motor Driver Pins
#define ENB 6   // PWM Speed Enable (Timer0, ~976 Hz)
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
  Serial.println(F("[DIAGNOSTIC] Phase 1A: Left Motor -> LOGICAL FORWARD (Verify forward rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(true, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  Serial.println(F("[DIAGNOSTIC] Phase 1B: Left Motor -> LOGICAL REVERSE (Verify reverse rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(false, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  // ---------------------------------------------------
  // Phase 2: Right Motor Channel Polarity Check
  // ---------------------------------------------------
  Serial.println(F("[DIAGNOSTIC] Phase 2A: Right Motor -> LOGICAL FORWARD (Verify forward rotation)"));
  digitalWrite(STATUS_LED, HIGH);
  setRightMotor(true, TEST_SPEED_SINGLE);
  delay(PULSE_DELAY_MS);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(PAUSE_DELAY_MS);

  Serial.println(F("[DIAGNOSTIC] Phase 2B: Right Motor -> LOGICAL REVERSE (Verify reverse rotation)"));
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
  Serial.println(F("[NOTE] If any wheel rotated backwards, swap its physical terminal leads.\n"));
  delay(5000);
}

// =====================================================
// MOTOR CONTROL PRIMITIVES
// =====================================================

void setLeftMotor(bool forward, int speed) {
  if (speed <= 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  } else {
    if (forward) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
    analogWrite(ENA, constrain(speed, 0, 255));
  }
}

void setRightMotor(bool forward, int speed) {
  if (speed <= 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  } else {
    if (forward) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    analogWrite(ENB, constrain(speed, 0, 255));
  }
}

void stopAllMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
