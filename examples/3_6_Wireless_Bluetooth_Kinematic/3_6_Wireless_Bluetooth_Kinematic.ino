/*
  NIELIT Robotics Practicals
  Practical 3.6: Wireless Kinematic – Bluetooth Interfacing

  Objective:
  Control your 2-wheel mobile robot wirelessly from an Android smartphone
  using an HC-05 / HC-06 Bluetooth module and simple single-character commands.

  Bluetooth Wiring (HC-05 to Arduino UNO):
  - VCC -> 5V
  - GND -> GND
  - TXD -> Pin 12 (RX)
  - RXD -> Pin 13 (TX) via 1k/2k resistor voltage divider

  Commands:
  'F' = Forward, 'B' = Backward, 'L' = Left, 'R' = Right, 'S' = Stop, '0'..'9' = Speed
*/

#include <SoftwareSerial.h>

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

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.6");
  Serial.println("Wireless Bluetooth Robot Control");
  Serial.println("Pair phone to HC-05 (Default PIN: 1234 or 0000).");
  Serial.println("Commands: F=Forward, B=Back, L=Left, R=Right, S=Stop, 0-9=Speed\n");
}

void loop() {
  // Read incoming Bluetooth command
  if (btSerial.available() > 0) {
    char cmd = btSerial.read();
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
