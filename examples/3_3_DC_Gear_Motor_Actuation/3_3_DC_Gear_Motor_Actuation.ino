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
  ENA (Pin 1)           Pin 5 (PWM)      Left Motor Speed
  IN1 (Pin 2)           Pin 2            Left Motor Dir A
  IN2 (Pin 7)           Pin 3            Left Motor Dir B
  IN3 (Pin 10)          Pin 4            Right Motor Dir A
  IN4 (Pin 15)          Pin 7            Right Motor Dir B
  ENB (Pin 9)           Pin 6 (PWM)      Right Motor Speed
  OUT1, OUT2            Terminals        Left DC Gear Motor
  OUT3, OUT4            Terminals        Right DC Gear Motor
  VCC1                  5V               Arduino 5V
  VCC2 (VM)             +6V to +12V      Motor Power Battery (+)
  GND                   GND              Common Ground
  -------------------------------------------------------------
*/

// Left Motor Driver Pins
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6
#define IN3 4
#define IN4 7

void setup() {
  Serial.begin(9600);

  // Setup motor pins as outputs
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
  Serial.println(F("Commencing dual-channel motor actuation test sequence in 2s:\n"));
  delay(2000);
}

void loop() {
  // 1. Both Motors Forward
  Serial.println(F("1. Both Motors Driving FORWARD (Thrust)"));
  setMotorLeft(true, 240);
  setMotorRight(true, 240);
  delay(2500);
  stopMotors();
  delay(800);

  // 2. Both Motors Reverse
  Serial.println(F("2. Both Motors Driving REVERSE (Pull)"));
  setMotorLeft(false, 240);
  setMotorRight(false, 240);
  delay(2500);
  stopMotors();
  delay(800);

  // 3. Left Motor Only (Forward)
  Serial.println(F("3. Left Motor ONLY (Forward) -> Vehicle pivots Right"));
  setMotorLeft(true, 220);
  setMotorRight(false, 0);
  delay(2000);
  stopMotors();
  delay(800);

  // 4. Right Motor Only (Forward)
  Serial.println(F("4. Right Motor ONLY (Forward) -> Vehicle pivots Left"));
  setMotorLeft(false, 0);
  setMotorRight(true, 220);
  delay(2000);
  stopMotors();
  delay(800);

  // 5. Axial Spin Left (Left reverse, Right forward)
  Serial.println(F("5. Axial SPIN LEFT (Zero turning radius)"));
  setMotorLeft(false, 200);
  setMotorRight(true, 200);
  delay(2000);
  stopMotors();
  delay(800);

  // 6. Axial Spin Right (Left forward, Right reverse)
  Serial.println(F("6. Axial SPIN RIGHT (Zero turning radius)"));
  setMotorLeft(true, 200);
  setMotorRight(false, 200);
  delay(2000);
  stopMotors();
  
  Serial.println(F("\n--- Sequence Completed. Pausing 4 seconds ---\n"));
  delay(4000);
}

// -------------------------------------------------------------
// Actuation Helper Routines
// -------------------------------------------------------------

void setMotorLeft(bool forward, int speed) {
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

void setMotorRight(bool forward, int speed) {
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

void stopMotors() {
  setMotorLeft(true, 0);
  setMotorRight(true, 0);
}
