/*
  Practical 3.2: Inductive Load Control — Interfacing the L293D Motor Driver Shield
  Course: NIELIT Robotics Practicals

  Description:
  Demonstrates H-Bridge switching logic, inductive back-EMF suppression,
  and motor state control (Forward, Reverse, Active Brake, and Coasting Stop)
  using the L293D Quadruple Half-H Driver IC / Motor Driver Shield.

  L293D H-Bridge Truth Table:
  +-----+-----+-----+--------------------+
  | ENA | IN1 | IN2 | Motor Action       |
  +-----+-----+-----+--------------------+
  |  H  |  H  |  L  | Turn Clockwise     |
  |  H  |  L  |  H  | Turn Anti-Clockwise|
  |  H  |  H  |  H  | Fast Brake         |
  |  H  |  L  |  L  | Coasting Stop      |
  |  L  |  X  |  X  | Motor Disabled     |
  +-----+-----+-----+--------------------+

  Hardware Connections:
  -------------------------------------------------------------
  L293D Pin             Arduino Pin      Function
  -------------------------------------------------------------
  Pin 1 (1,2EN)         Pin 5 (PWM)      Channel A Enable
  Pin 2 (1A / IN1)      Pin 2            Channel A Input 1
  Pin 7 (2A / IN2)      Pin 3            Channel A Input 2
  Pin 3, 6 (1Y, 2Y)     Motor Terminals  DC Motor Output
  Pin 4, 5, 12, 13      GND              Common Ground & Heat Sink
  Pin 8 (VCC2 / VM)     +6V to +12V      Motor Power Supply
  Pin 16 (VCC1 / VSS)   5V               Logic Power Supply
  -------------------------------------------------------------
*/

// Driver Control Pins
#define ENA 5   // Speed / Enable Pin (PWM)
#define IN1 2   // Direction Control 1
#define IN2 3   // Direction Control 2

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.2: L293D Inductive Load Control     "));
  Serial.println(F("=================================================="));
  Serial.println(F("Starting H-Bridge state machine verification...\n"));
}

void loop() {
  // 1. Clockwise (Forward) Rotation
  Serial.println(F("State 1: CLOCKWISE (IN1=HIGH, IN2=LOW, ENA=255)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255);
  delay(2500);

  // 2. Active Braking
  Serial.println(F("State 2: ACTIVE BRAKE (IN1=HIGH, IN2=HIGH)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  delay(1000);

  // 3. Counter-Clockwise (Reverse) Rotation
  Serial.println(F("State 3: COUNTER-CLOCKWISE (IN1=LOW, IN2=HIGH, ENA=255)"));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);
  delay(2500);

  // 4. Coasting Stop (High-Z Floating)
  Serial.println(F("State 4: COASTING STOP (IN1=LOW, IN2=LOW, ENA=0)"));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  delay(2000);

  // 5. Half Power / PWM test
  Serial.println(F("State 5: 50% PWM CLOCKWISE (ENA=128)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 128);
  delay(2000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  Serial.println(F("\nCycle completed. Restarting in 3 seconds...\n"));
  delay(3000);
}
