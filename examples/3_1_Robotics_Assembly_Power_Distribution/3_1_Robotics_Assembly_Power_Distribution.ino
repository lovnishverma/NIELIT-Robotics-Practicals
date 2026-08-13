/*
  Practical 3.1: Robotics Assembly & Power Distribution Dynamics
  Course: NIELIT Robotics Practicals
  
  Description:
  Demonstrates robotic chassis power distribution dynamics, separate logic
  and motor power supplies, common ground bonding, and power rail diagnostics.
  Performs a startup self-test sequence verifying dual motor channels.

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

// Built-in status indicator
#define STATUS_LED 13

void setup() {
  // Initialize Serial Monitor for telemetry
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
  Serial.println(F("Checking Power Rails:"));
  Serial.println(F(" - Logic VCC: 5V regulated from Arduino"));
  Serial.println(F(" - Motor VCC: External Battery pack (Common GND)"));
  Serial.println(F("Starting 3-second power stabilization delay..."));
  
  for (int i = 3; i > 0; i--) {
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);
    delay(500);
    Serial.print(F("Ready in: "));
    Serial.print(i);
    Serial.println(F("s"));
  }
  
  Serial.println(F("Power stabilized. Commencing motor diagnostics...\n"));
}

void loop() {
  // --- Test 1: Left Motor Forward Pulse ---
  Serial.println(F("[Diagnostic 1] Testing Left Motor Channel (Forward 1.5s)..."));
  digitalWrite(STATUS_LED, HIGH);
  analogWrite(ENA, 200);   // Set 78% PWM power
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(1500);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(1000);

  // --- Test 2: Right Motor Forward Pulse ---
  Serial.println(F("[Diagnostic 2] Testing Right Motor Channel (Forward 1.5s)..."));
  digitalWrite(STATUS_LED, HIGH);
  analogWrite(ENB, 200);   // Set 78% PWM power
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1500);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  delay(1000);

  // --- Test 3: Dual Motor Synchronized Forward Check ---
  Serial.println(F("[Diagnostic 3] Testing Dual Channel Power Distribution (2.0s)..."));
  digitalWrite(STATUS_LED, HIGH);
  analogWrite(ENA, 220);
  analogWrite(ENB, 220);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2000);
  stopAllMotors();
  digitalWrite(STATUS_LED, LOW);
  
  Serial.println(F("\nPower distribution test cycle complete. Pausing 5 seconds.\n"));
  delay(5000);
}

void stopAllMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
