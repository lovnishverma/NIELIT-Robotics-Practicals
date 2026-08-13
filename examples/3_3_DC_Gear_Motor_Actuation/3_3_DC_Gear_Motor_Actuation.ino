/*
  Practical 3.3: Direct Current Actuation — Interfacing DC Gear Motors
  Course: NIELIT Robotics Practicals

  Description:
  Controls dual DC gear motors (BO Motors) using the L293D motor driver.
  Demonstrates independent bi-directional actuation for Left and Right channels,
  verifying forward thrust, reverse pull, and axial spinning motions.

  Tinkercad Simulation:
  https://www.tinkercad.com/things/itgeuX95VvZ-two-dc-motor-l293d

  Hardware Connections:
  -------------------------------------------------------------
  L293D Pin             Arduino Pin      Connected To
  -------------------------------------------------------------
  ENA (Pin 1)           Pin 9 (PWM)      Left Motor Speed
  IN1 (Pin 2)           Pin 5            Left Motor Dir A
  IN2 (Pin 7)           Pin 6            Left Motor Dir B
  IN3 (Pin 10)          Pin 10           Right Motor Dir A
  IN4 (Pin 15)          Pin 11           Right Motor Dir B
  ENB (Pin 9)           Pin 3 (PWM)      Right Motor Speed
  OUT1, OUT2            Terminals        Left DC Gear Motor
  OUT3, OUT4            Terminals        Right DC Gear Motor
  VCC1                  5V               Arduino 5V
  VCC2 (VM)             +6V to +12V      Motor Power Battery (+)
  GND                   GND              Common Ground
  -------------------------------------------------------------
*/

// Left Motor Driver Pins
const int ENA = 9;
const int IN1 = 5;
const int IN2 = 6;

// Right Motor Driver Pins
const int ENB = 3;
const int IN3 = 10;
const int IN4 = 11;

void setup() {
  Serial.begin(9600);

  // Setup motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.3: DC Gear Motor Actuation           "));
  Serial.println(F("=================================================="));
  Serial.println(F("Demonstrating dual-channel motor actuation sequence:\n"));
}

void loop() {
  // 1. Both Motors Forward
  Serial.println(F("1. Both Motors Driving FORWARD"));
  setMotorLeft(true, 255);
  setMotorRight(true, 255);
  delay(3000);
  stopMotors();
  delay(1000);

  // 2. Both Motors Reverse
  Serial.println(F("2. Both Motors Driving REVERSE"));
  setMotorLeft(false, 255);
  setMotorRight(false, 255);
  delay(3000);
  stopMotors();
  delay(1000);

  // 3. Left Motor Only (Forward)
  Serial.println(F("3. Left Motor ONLY (Forward)"));
  setMotorLeft(true, 220);
  setMotorRight(false, 0);
  delay(2000);
  stopMotors();
  delay(1000);

  // 4. Right Motor Only (Forward)
  Serial.println(F("4. Right Motor ONLY (Forward)"));
  setMotorLeft(false, 0);
  setMotorRight(true, 220);
  delay(2000);
  stopMotors();
  delay(1000);

  // 5. Axial Spin Left (Left reverse, Right forward)
  Serial.println(F("5. Axial SPIN LEFT"));
  setMotorLeft(false, 200);
  setMotorRight(true, 200);
  delay(2000);
  stopMotors();
  delay(1000);

  // 6. Axial Spin Right (Left forward, Right reverse)
  Serial.println(F("6. Axial SPIN RIGHT"));
  setMotorLeft(true, 200);
  setMotorRight(false, 200);
  delay(2000);
  stopMotors();
  delay(3000);
}

// Helper to set Left Motor direction and speed
void setMotorLeft(bool forward, int speed) {
  analogWrite(ENA, speed);
  if (speed == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  } else if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

// Helper to set Right Motor direction and speed
void setMotorRight(bool forward, int speed) {
  analogWrite(ENB, speed);
  if (speed == 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  } else if (forward) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

// Safe stop for both motors
void stopMotors() {
  setMotorLeft(true, 0);
  setMotorRight(true, 0);
}
