/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.6: Wireless Kinematic – Bluetooth Interfacing
  =========================================================

  Objective:
  Control your 2-wheel mobile robot wirelessly from an Android smartphone
  using an HC-05 / HC-06 Bluetooth module and simple single-character commands.

  ---------------------------------------------------------
  Bluetooth Module (HC-05 / HC-06) Wiring:
  ---------------------------------------------------------
  HC-05 Pin     Arduino Pin   Notes
  ---------------------------------------------------------
  VCC           5V            Power supply from Arduino 5V
  GND           GND           Common Ground
  TXD           Pin 12 (RX)   Connects directly to Arduino Pin 12
  RXD           Pin 13 (TX)   Connects to Arduino Pin 13
                              (Recommended: Use 1k/2k resistor divider to 3.3V)
  ---------------------------------------------------------

  Resistor Divider for HC-05 RX Pin (3.3V Logic Protection):
    Arduino Pin 13 ---[ 1k Ohm ]---+--- HC-05 RXD
                                   |
                                [ 2k Ohm ]
                                   |
                              Arduino GND

  ---------------------------------------------------------
  Motor Driver Pin Connections (L298N):
  ---------------------------------------------------------
  Left Motor:   ENA -> Pin 5, IN1 -> Pin 2, IN2 -> Pin 3
  Right Motor:  ENB -> Pin 6, IN3 -> Pin 4, IN4 -> Pin 7
  Motor Power:  6.0V - 7.4V Battery Pack (+ to VM/12V, - to GND)

  ---------------------------------------------------------
  Bluetooth Control Commands:
  ---------------------------------------------------------
  'F' or 'f' -> Move Forward
  'B' or 'b' -> Move Backward
  'L' or 'l' -> Spin Left
  'R' or 'r' -> Spin Right
  'S' or 's' -> Stop
  '0' .. '9' -> Adjust Speed (0 = Slowest, 9 = Fastest)

  Author: National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

#include <SoftwareSerial.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Bluetooth Serial Pins
const int BT_RX_PIN = 12; // Arduino receives from HC-05 TXD
const int BT_TX_PIN = 13; // Arduino transmits to HC-05 RXD

// Motor Driver Pins
const int PIN_ENA = 5;
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;
const int PIN_ENB = 6;
const int PIN_IN3 = 4;
const int PIN_IN4 = 7;

// =====================================================
// BLUETOOTH & MOTOR VARIABLES
// =====================================================

SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

int currentSpeed = 200; // Default driving speed (0 - 255)
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

// Safety Watchdog: Auto-stops robot if no command received for 1.5 seconds
unsigned long lastCommandTime = 0;
const unsigned long TIMEOUT_MS = 1500;
bool isMoving = false;

// =====================================================
// FUNCTION DECLARATIONS
// =====================================================

void processCommand(char cmd);
void moveForward();
void moveBackward();
void spinLeft();
void spinRight();
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);    // Hardware Serial for USB Computer Monitor
  btSerial.begin(9600);  // Software Serial for HC-05 Bluetooth Module

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Robotics Practical 3.6"));
  Serial.println(F(" Wireless Bluetooth Robot Control"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] Pair your phone to 'HC-05' (Default PIN: 1234 or 0000)."));
  Serial.println(F("[INFO] Use any Bluetooth RC Car App or Bluetooth Serial Terminal."));
  Serial.println(F("[INFO] Commands: F=Forward, B=Back, L=Left, R=Right, S=Stop, 0-9=Speed\n"));
}

// =====================================================
// MAIN LOOP: Listens for Bluetooth commands & handles safety
// =====================================================

void loop() {
  // Check if character received from Bluetooth
  if (btSerial.available() > 0) {
    char cmd = btSerial.read();
    lastCommandTime = millis(); // Refresh watchdog timer
    processCommand(cmd);
  }

  // Safety Timeout: If car is moving and Bluetooth signal is lost, stop car!
  if (isMoving && (millis() - lastCommandTime > TIMEOUT_MS)) {
    Serial.println(F("[SAFETY] Bluetooth signal timeout -> Auto-stopping robot!"));
    stopRobot();
  }
}

// =====================================================
// COMMAND PROCESSOR
// =====================================================

void processCommand(char cmd) {
  switch (cmd) {
    case 'F':
    case 'f':
      Serial.println(F("[CMD] Moving FORWARD"));
      moveForward();
      break;

    case 'B':
    case 'b':
      Serial.println(F("[CMD] Moving BACKWARD"));
      moveBackward();
      break;

    case 'L':
    case 'l':
      Serial.println(F("[CMD] Spinning LEFT"));
      spinLeft();
      break;

    case 'R':
    case 'r':
      Serial.println(F("[CMD] Spinning RIGHT"));
      spinRight();
      break;

    case 'S':
    case 's':
    case 'D':
    case 'd':
      Serial.println(F("[CMD] STOPPING"));
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
      // Ignore newlines or unmapped characters
      break;
  }
}

// =====================================================
// MOVEMENT PRIMITIVES
// =====================================================

void moveForward() {
  int leftSpd  = constrain(currentSpeed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(currentSpeed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
  isMoving = true;
}

void moveBackward() {
  int leftSpd  = constrain(currentSpeed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(currentSpeed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
  isMoving = true;
}

void spinLeft() {
  int leftSpd  = constrain(currentSpeed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(currentSpeed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
  isMoving = true;
}

void spinRight() {
  int leftSpd  = constrain(currentSpeed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(currentSpeed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
  isMoving = true;
}

void stopRobot() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
  isMoving = false;
}
