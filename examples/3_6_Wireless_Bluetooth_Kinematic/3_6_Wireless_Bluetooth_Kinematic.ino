/*
  Practical 3.6: Wireless Kinematic – Bluetooth Interfacing
  Course: NIELIT Robotics Practicals

  Description:
  Enables wireless teleoperation of the 2-wheel differential drive robot
  via Bluetooth (HC-05 or HC-06 module) using SoftwareSerial.
  Implements standard RC Bluetooth command protocol and includes a fail-safe
  watchdog timer to automatically stop the vehicle upon connection loss.

  Standard Bluetooth Robot Protocol:
  - 'F': Forward
  - 'B': Backward
  - 'L': Turn Left (Point spin)
  - 'R': Turn Right (Point spin)
  - 'G': Forward Left (Diagonal curve)
  - 'I': Forward Right (Diagonal curve)
  - 'H': Backward Left
  - 'J': Backward Right
  - 'S': STOP
  - '0'..'9': Set Speed (0 = min, 9 = max 255)

  Hardware Connections:
  -------------------------------------------------------------
  HC-05 / HC-06 Pin     Arduino Pin      Notes
  -------------------------------------------------------------
  VCC                   5V
  GND                   GND
  TXD                   Pin 12           Arduino RX (SoftwareSerial)
  RXD                   Pin 13           Arduino TX (via voltage divider)
  -------------------------------------------------------------
  Motor Driver Pin      Arduino Pin
  -------------------------------------------------------------
  ENA                   Pin 9 (PWM)
  IN1                   Pin 5
  IN2                   Pin 6
  IN3                   Pin 10
  IN4                   Pin 11
  ENB                   Pin 3 (PWM)
  -------------------------------------------------------------
*/

#include <SoftwareSerial.h>

// Bluetooth SoftwareSerial interface
// Arduino Pin 12 (RX) <- HC-05 TXD
// Arduino Pin 13 (TX) -> HC-05 RXD (Use 1k/2k resistor voltage divider to 3.3V)
SoftwareSerial BTSerial(12, 13);

// Motor Pins
const int ENA = 9;
const int IN1 = 5;
const int IN2 = 6;
const int ENB = 3;
const int IN3 = 10;
const int IN4 = 11;

// Current operating speed (0 to 255)
int currentSpeed = 200;

// Watchdog safety timer (Auto-stop if no packet received within timeout)
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
  // Read incoming command from Bluetooth
  if (BTSerial.available()) {
    char cmd = BTSerial.read();
    handleCommand(cmd, "Bluetooth");
  }

  // Read incoming command from USB Serial Monitor (for testing)
  if (Serial.available()) {
    char cmd = Serial.read();
    handleCommand(cmd, "Serial");
  }

  // Watchdog Safety Check: Stop robot if communication drops while moving
  if (isMoving && (millis() - lastCommandTime > WATCHDOG_TIMEOUT_MS)) {
    Serial.println(F("[Watchdog] Timeout: No signal received. Auto-Stopping."));
    stopRobot();
  }
}

void handleCommand(char cmd, const char* source) {
  // Ignore newline or carriage return characters
  if (cmd == '\r' || cmd == '\n') return;

  lastCommandTime = millis();

  Serial.print(F("["));
  Serial.print(source);
  Serial.print(F("] Command: '"));
  Serial.print(cmd);
  Serial.println(F("'"));

  switch (cmd) {
    case 'F': // Forward
      moveForward(currentSpeed);
      break;

    case 'B': // Backward
      moveBackward(currentSpeed);
      break;

    case 'L': // Spin Left
      spinLeft(currentSpeed);
      break;

    case 'R': // Spin Right
      spinRight(currentSpeed);
      break;

    case 'G': // Forward Left
      curveDrive(currentSpeed / 2, currentSpeed);
      break;

    case 'I': // Forward Right
      curveDrive(currentSpeed, currentSpeed / 2);
      break;

    case 'H': // Backward Left
      curveReverse(currentSpeed / 2, currentSpeed);
      break;

    case 'J': // Backward Right
      curveReverse(currentSpeed, currentSpeed / 2);
      break;

    case 'S': // STOP
      stopRobot();
      break;

    // Speed Presets '0' through '9'
    case '0': setSpeedPreset(70);  break;
    case '1': setSpeedPreset(90);  break;
    case '2': setSpeedPreset(110); break;
    case '3': setSpeedPreset(130); break;
    case '4': setSpeedPreset(150); break;
    case '5': setSpeedPreset(170); break;
    case '6': setSpeedPreset(190); break;
    case '7': setSpeedPreset(210); break;
    case '8': setSpeedPreset(235); break;
    case '9':
    case 'q': setSpeedPreset(255); break;

    default:
      Serial.println(F("Unknown command."));
      break;
  }
}

void setSpeedPreset(int spd) {
  currentSpeed = spd;
  Serial.print(F("Speed updated to: "));
  Serial.println(currentSpeed);
  BTSerial.print(F("Speed: "));
  BTSerial.println(currentSpeed);
}

// -------------------------------------------------------------
// Motion Functions
// -------------------------------------------------------------

void moveForward(int speed) {
  isMoving = true;
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward(int speed) {
  isMoving = true;
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void spinLeft(int speed) {
  isMoving = true;
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void spinRight(int speed) {
  isMoving = true;
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void curveDrive(int leftSpeed, int rightSpeed) {
  isMoving = true;
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void curveReverse(int leftSpeed, int rightSpeed) {
  isMoving = true;
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {
  isMoving = false;
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
