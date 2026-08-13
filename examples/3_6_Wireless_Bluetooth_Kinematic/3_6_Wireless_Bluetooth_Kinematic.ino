/*
  Practical 3.6: Wireless Kinematic – Bluetooth Interfacing
  Course: NIELIT Robotics Practicals

  Description:
  Enables wireless teleoperation of the 2-wheel differential drive robot
  via Bluetooth (HC-05 or HC-06 module) using SoftwareSerial.
  Implements robust character protocol parsing (handling both uppercase and
  lowercase commands), speed preset scaling, and a fail-safe safety watchdog timer
  to automatically stop the vehicle upon connection interruption.

  Standard Bluetooth Robot Protocol:
  - 'F' / 'f': Forward
  - 'B' / 'b': Backward
  - 'L' / 'l': Spin Left (Point turn)
  - 'R' / 'r': Spin Right (Point turn)
  - 'G' / 'g': Forward Left Curve
  - 'I' / 'i': Forward Right Curve
  - 'H' / 'h': Backward Left Curve
  - 'J' / 'j': Backward Right Curve
  - 'S' / 's' / 'D' / 'd': STOP ALL MOTORS
  - '0'..'9' / 'q': Set Speed Presets (0 = Min, 9 / 'q' = Max 255)

  Hardware Connections:
  -------------------------------------------------------------
  HC-05 / HC-06 Pin     Arduino Pin      Notes
  -------------------------------------------------------------
  VCC                   5V               5V Power
  GND                   GND              Common Ground
  TXD                   Pin 12           Arduino RX (SoftwareSerial)
  RXD                   Pin 13           Arduino TX (via 1k/2k divider)
  -------------------------------------------------------------
  Motor Driver Pin      Arduino Pin
  -------------------------------------------------------------
  ENA                   Pin 5 (PWM)
  IN1                   Pin 2
  IN2                   Pin 3
  IN3                   Pin 4
  IN4                   Pin 7
  ENB                   Pin 6 (PWM)
  -------------------------------------------------------------
*/

#include <SoftwareSerial.h>

// Bluetooth SoftwareSerial interface
// Arduino Pin 12 (RX) <- HC-05 TXD
// Arduino Pin 13 (TX) -> HC-05 RXD (Use 1k/2k resistor voltage divider to 3.3V)
SoftwareSerial BTSerial(12, 13);

// Left Motor Driver Pins
#define ENA 5
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6
#define IN3 4
#define IN4 7

// Current operating speed (0 to 255)
int currentSpeed = 200;

// Watchdog safety timer (Auto-stop if no command received within timeout)
unsigned long lastCommandTime = 0;
const unsigned long WATCHDOG_TIMEOUT_MS = 1000;
bool isMoving = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600); // Standard default baud rate for HC-05/06

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.6: Wireless Bluetooth Robocar       "));
  Serial.println(F("=================================================="));
  Serial.println(F("Bluetooth ready! Pair with smartphone controller."));
  Serial.println(F("Commands: F=Forward, B=Back, L=Left, R=Right, S=Stop, 0-9=Speed\n"));

  BTSerial.println(F("NIELIT Bluetooth Robocar Ready!"));
  lastCommandTime = millis();
}

void loop() {
  // Read incoming command from Bluetooth module
  if (BTSerial.available()) {
    char cmd = BTSerial.read();
    handleCommand(cmd, "Bluetooth");
  }

  // Read incoming command from USB Serial Monitor (for bench testing)
  if (Serial.available()) {
    char cmd = Serial.read();
    handleCommand(cmd, "Serial");
  }

  // Watchdog Safety Check: Stop robot if communication drops while in motion
  if (isMoving && (millis() - lastCommandTime > WATCHDOG_TIMEOUT_MS)) {
    Serial.println(F("[Watchdog] Signal timeout reached. Auto-Stopping."));
    stopRobot();
  }
}

void handleCommand(char rawCmd, const char* source) {
  // Ignore newline or carriage return whitespace
  if (rawCmd == '\r' || rawCmd == '\n' || rawCmd == ' ') return;

  lastCommandTime = millis();

  Serial.print(F("["));
  Serial.print(source);
  Serial.print(F("] Received: '"));
  Serial.print(rawCmd);
  Serial.println(F("'"));

  // Check for numeric speed preset commands '0' through '9'
  if (rawCmd >= '0' && rawCmd <= '9') {
    int speedMap[] = {70, 90, 110, 130, 150, 170, 190, 210, 235, 255};
    setSpeedPreset(speedMap[rawCmd - '0']);
    return;
  }

  char cmd = toupper(rawCmd);

  switch (cmd) {
    case 'F': // Forward
      moveForward(currentSpeed);
      break;

    case 'B': // Backward
      moveBackward(currentSpeed);
      break;

    case 'L': // Spin Left (Point Turn)
      spinLeft(currentSpeed);
      break;

    case 'R': // Spin Right (Point Turn)
      spinRight(currentSpeed);
      break;

    case 'G': // Forward Left Curve (Differential drive)
      curveDrive(currentSpeed / 3, currentSpeed);
      break;

    case 'I': // Forward Right Curve (Differential drive)
      curveDrive(currentSpeed, currentSpeed / 3);
      break;

    case 'H': // Backward Left Curve
      curveReverse(currentSpeed / 3, currentSpeed);
      break;

    case 'J': // Backward Right Curve
      curveReverse(currentSpeed, currentSpeed / 3);
      break;

    case 'S': // STOP
    case 'D': // STOP ALL
      stopRobot();
      break;

    case 'Q': // Max Speed
      setSpeedPreset(255);
      break;

    default:
      Serial.print(F("Unrecognized command: "));
      Serial.println(cmd);
      break;
  }
}

void setSpeedPreset(int spd) {
  currentSpeed = constrain(spd, 0, 255);
  Serial.print(F("Speed updated to: "));
  Serial.println(currentSpeed);
  BTSerial.print(F("Speed: "));
  BTSerial.println(currentSpeed);
}

// -------------------------------------------------------------
// Motion Primitives
// -------------------------------------------------------------

void moveForward(int speed) {
  isMoving = true;
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moveBackward(int speed) {
  isMoving = true;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void spinLeft(int speed) {
  isMoving = true;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void spinRight(int speed) {
  isMoving = true;
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void curveDrive(int leftSpeed, int rightSpeed) {
  isMoving = true;
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void curveReverse(int leftSpeed, int rightSpeed) {
  isMoving = true;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void stopRobot() {
  isMoving = false;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
