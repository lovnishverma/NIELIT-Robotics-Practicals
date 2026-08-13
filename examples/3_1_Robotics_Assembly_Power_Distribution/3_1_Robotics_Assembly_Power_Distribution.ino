/*
  Practical 3.1: Robotics Assembly & Power Distribution Dynamics
  Course: NIELIT Robotics Practicals
  
  Description:
  Demonstrates robotic chassis power distribution dynamics, separate logic
  and motor power supplies, common ground bonding, and power rail diagnostics.
  Executes a comprehensive 4-phase diagnostic drill verifying forward and reverse
  rotation for both motor channels before deployment.

  Tinkercad Simulation:
  https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d

  Hardware Wiring:
  -------------------------------------------------------------
  L293D / Driver Pin    Arduino Pin      Description
  -------------------------------------------------------------
  ENABLE 1 (ENA)        Pin 5 (PWM)      Left Motor Speed Enable
  INPUT 1 (IN1)         Pin 2            Left Motor Direction A
  INPUT 2 (IN2)         Pin 3            Left Motor Direction B
  INPUT 3 (IN3)         Pin 4            Right Motor Direction A
  INPUT 4 (IN4)         Pin 7            Right Motor Direction B
  ENABLE 2 (ENB)        Pin 6 (PWM)      Right Motor Speed Enable
  VCC1 (Logic 5V)       5V               Arduino 5V Rail
  VCC2 / VM (Motor)     External Battery Positive (+6V to +12V)
  GND (All GND pins)    GND & Batt (-)   COMMON GROUND (Essential!)
  -------------------------------------------------------------
*/

// --- Left Motor Pin Definitions ---
#define ENA 5   // Enable Left Motor (PWM)
#define IN1 2   // Left Motor Input 1
#define IN2 3   // Left Motor Input 2

// --- Right Motor Pin Definitions ---
#define ENB 6   // Enable Right Motor (PWM)
#define IN3 4   // Right Motor Input 1
#define IN4 7   // Right Motor Input 2

// Built-in status indicator LED
#define STATUS_LED 13

void setup() {
  Serial.begin(9600);
  
  // Configure Motor Control Pins as Outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Initialize motors in stopped condition (safe state)
  stopAllMotors();

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.1: Robotics Assembly & Power Check   "));
  Serial.println(F("=================================================="));
  Serial.println(F("Power Distribution Architecture:"));
  Serial.println(F(" - Logic Rail: 5V regulated from Arduino"));
  Serial.println(F(" - Motor Rail: External Battery pack (Common GND)"));
  Serial.println(F("Starting 3-second power stabilization countdown...\n"));
  
  for (int i = 3; i > 0; i--) {
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);
    delay(500);
    Serial.print(F("Stabilizing: "));
    Serial.print(i);
    Serial.println(F("s"));
  }
  
  Serial.println(F("Power stabilized. Commencing 4-phase diagnostics...\n"));
}

void loop() {
  // ===================================================
  // Phase 1: Left Motor Polarity & Rotation Check
  // ===================================================
  Serial.println(F("[Phase 1] Left Motor -> FORWARD (1.5s)... (Verify wheel turns FORWARD)"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(true, 200);
  delay(1500);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(800);

  Serial.println(F("[Phase 1] Left Motor -> REVERSE (1.5s)... (Verify wheel turns REVERSE)"));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(false, 200);
  delay(1500);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(1000);

  // ===================================================
  // Phase 2: Right Motor Polarity & Rotation Check
  // ===================================================
  Serial.println(F("[Phase 2] Right Motor -> FORWARD (1.5s)... (Verify wheel turns FORWARD)"));
  digitalWrite(STATUS_LED, HIGH);
  setRightMotor(true, 200);
  delay(1500);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(800);

  Serial.println(F("[Phase 2] Right Motor -> REVERSE (1.5s)... (Verify wheel turns REVERSE)"));
  digitalWrite(STATUS_LED, HIGH);
  setRightMotor(false, 200);
  delay(1500);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(1000);

  // ===================================================
  // Phase 3: Dual Motor Synchronous Forward Thrust
  // ===================================================
  Serial.println(F("[Phase 3] Dual Channel Synchronized FORWARD (2.0s)..."));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(true, 220);
  setRightMotor(true, 220);
  delay(2000);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(1000);

  // ===================================================
  // Phase 4: Dual Motor Synchronous Reverse Pull
  // ===================================================
  Serial.println(F("[Phase 4] Dual Channel Synchronized REVERSE (2.0s)..."));
  digitalWrite(STATUS_LED, HIGH);
  setLeftMotor(false, 220);
  setRightMotor(false, 220);
  delay(2000);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  
  Serial.println(F("\n--- Diagnostic Cycle Completed ---"));
  Serial.println(F("Note: If any wheel rotates in reverse, swap its motor terminal leads.\n"));
  delay(4000);
}

// -------------------------------------------------------------
// Motor Control Helpers
// -------------------------------------------------------------

void setLeftMotor(bool forward, int speed) {
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  analogWrite(ENA, speed);
}

void setRightMotor(bool forward, int speed) {
  if (forward) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  analogWrite(ENB, speed);
}

void stopAllMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
