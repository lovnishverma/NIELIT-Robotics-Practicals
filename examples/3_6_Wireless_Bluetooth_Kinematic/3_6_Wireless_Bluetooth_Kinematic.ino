/*
  NIELIT Robotics Practicals
  Practical 3.6: Wireless Kinematic – Bluetooth Interfacing

  Objective:
  Control your 2-wheel mobile robot wirelessly from an Android smartphone
  using an HC-05 / HC-06 Bluetooth module and the L293D Motor Shield.

  Wiring on L293D Motor Shield:
  - Left Motor  -> Screw Terminal M1
  - Right Motor -> Screw Terminal M2
  - Battery Pack -> EXT_PWR (+M and GND) on shield
  - Bluetooth Module (HC-05):
      * VCC -> 5V pin on shield
      * GND -> GND pin on shield
      * TXD -> Analog Pin A0 (SoftwareSerial RX)
      * RXD -> Analog Pin A1 (SoftwareSerial TX, via 1k/2k resistor divider)

  Commands:
  'F' = Forward, 'B' = Backward, 'L' = Left, 'R' = Right, 'S' = Stop, '0'..'9' = Speed
*/

#include <AFMotor.h>
#include <SoftwareSerial.h>

// Connect Left Motor to M1, Right Motor to M2
AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);

// Bluetooth Serial Pins on the Shield's Analog Header
const int BT_RX_PIN = A0; // Arduino receives from HC-05 TXD
const int BT_TX_PIN = A1; // Arduino transmits to HC-05 RXD

SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

int currentSpeed = 200; // Default driving speed (0 - 255)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// Safety Watchdog: Stops robot if no command received for 1.5 seconds
unsigned long lastCommandTime = 0;
const unsigned long TIMEOUT_MS = 1500;
bool isMoving = false;

void setup() {
  Serial.begin(9600);    // Serial Monitor (USB)
  btSerial.begin(9600);  // Bluetooth Module

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.6");
  Serial.println("L293D Shield - Wireless Bluetooth Robot Control");
  Serial.println("Connect HC-05 TXD to A0, RXD to A1.");
  Serial.println("Pair phone to HC-05 (Default PIN: 1234 or 0000).");
  Serial.println("Commands: F=Forward, B=Back, L=Left, R=Right, S=Stop, 0-9=Speed\n");
}

void loop() {
  // Read incoming command from Bluetooth module
  if (btSerial.available() > 0) {
    char cmd = btSerial.read();
    lastCommandTime = millis();
    processCommand(cmd);
  }

  // Read incoming command from USB Serial Monitor (bench testing)
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    lastCommandTime = millis();
    processCommand(cmd);
  }

  // Safety Timeout: Auto-stop if Bluetooth signal lost while moving
  if (isMoving && (millis() - lastCommandTime > TIMEOUT_MS)) {
    Serial.println("[Safety] Bluetooth timeout -> Stopping robot!");
    stopRobot();
  }
}

void processCommand(char cmd) {
  switch (cmd) {
    case 'F':
    case 'f':
      Serial.println("[Cmd] FORWARD");
      moveForward();
      break;

    case 'B':
    case 'b':
      Serial.println("[Cmd] BACKWARD");
      moveBackward();
      break;

    case 'L':
    case 'l':
      Serial.println("[Cmd] LEFT");
      spinLeft();
      break;

    case 'R':
    case 'r':
      Serial.println("[Cmd] RIGHT");
      spinRight();
      break;

    case 'S':
    case 's':
    case 'D':
    case 'd':
      Serial.println("[Cmd] STOP");
      stopRobot();
      break;

    // Speed Presets ('0' to '9')
    case '0': currentSpeed = 70;  break;
    case '1': currentSpeed = 90;  break;
    case '2': currentSpeed = 110; break;
    case '3': currentSpeed = 130; break;
    case '4': currentSpeed = 150; break;
    case '5': currentSpeed = 170; break;
    case '6': currentSpeed = 190; break;
    case '7': currentSpeed = 210; break;
    case '8': currentSpeed = 230; break;
    case '9': currentSpeed = 255; break;
    case 'q':
    case 'Q': currentSpeed = 255; break;

    default:
      break;
  }
}

// Movement Helper Functions

void moveForward() {
  motorLeft.setSpeed(constrain(currentSpeed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(currentSpeed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  isMoving = true;
}

void moveBackward() {
  motorLeft.setSpeed(constrain(currentSpeed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(currentSpeed + RIGHT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  isMoving = true;
}

void spinLeft() {
  motorLeft.setSpeed(constrain(currentSpeed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(currentSpeed + RIGHT_TRIM, 0, 255));
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  isMoving = true;
}

void spinRight() {
  motorLeft.setSpeed(constrain(currentSpeed + LEFT_TRIM, 0, 255));
  motorRight.setSpeed(constrain(currentSpeed + RIGHT_TRIM, 0, 255));
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  isMoving = true;
}

void stopRobot() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
  isMoving = false;
}
