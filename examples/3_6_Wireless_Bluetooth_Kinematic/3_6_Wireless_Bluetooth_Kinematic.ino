/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.6: Wireless Kinematic – Bluetooth Interfacing
  =========================================================

  Objective:
  Interface an HC-05/HC-06 Bluetooth module via SoftwareSerial, implement a wireless teleoperation
  protocol with multi-tier speed scaling, and integrate a communication timeout safety auto-stop.

  Description:
  Enables wireless remote control of a 2-wheel differential drive robot from a smartphone or PC Bluetooth terminal.
  Implements case-insensitive character parsing for directional movements (Forward, Backward, Point Turns, Differential Curves),
  discrete speed presets ('0' - '9'), and an autonomous communication timeout monitor that halts the vehicle
  if communication packets cease.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - HC-05 or HC-06 Bluetooth Serial Module (Default Baud: 9600)
  - L293D / L298N Dual H-Bridge Motor Driver
  - 2x DC Yellow BO Gear Motors (Nominal: 3V - 6V, 1:48 gear ratio)
  - 2WD Robotic Chassis with caster wheel
  - External Motor Power Supply: 6.0V - 7.4V (e.g. 4x AA Battery Pack)
  - Voltage Divider Resistors for 3.3V RX protection: 1k Ohm (D13 -> RXD) and 2k Ohm (RXD -> GND)

  Pin Configuration:
  -------------------------------------------------------------
  Module / Driver Pin      Arduino UNO Pin   Notes
  -------------------------------------------------------------
  HC-05 VCC                5V                5V Regulated Supply
  HC-05 GND                GND               Common Ground
  HC-05 TXD                Pin 12 (RX)       SoftwareSerial Receive from Bluetooth
  HC-05 RXD                Pin 13 (TX)       SoftwareSerial Transmit (via 3.3V divider)
  ENA                      Pin 5 (PWM)       Left Motor Speed Enable (Timer0)
  IN1                      Pin 2             Left Motor Direction Input 1
  IN2                      Pin 3             Left Motor Direction Input 2
  IN3                      Pin 4             Right Motor Direction Input 1
  IN4                      Pin 7             Right Motor Direction Input 2
  ENB                      Pin 6 (PWM)       Right Motor Speed Enable (Timer0)
  VCC2 / VM                Battery (+)       Motor Power Supply (6.0V - 7.4V Recommended)
  GND                      GND & Batt (-)    Common Ground Busbar (Mandatory)
  -------------------------------------------------------------

  Working Principle:
  The Bluetooth module functions as a transparent wireless UART serial bridge at 9600 baud.
  When the remote controller transmits single-byte ASCII characters, the Arduino reads them via
  `SoftwareSerial`, decodes the instruction, sets the motor state, and resets the watchdog timer.
  If no packet arrives within 1000ms while the vehicle is in motion, the communication watchdog
  automatically commands a full stop to prevent runaway vehicle motion upon signal loss.

  Bluetooth Command Protocol:
  +----------------------+----------------------------------------------------+
  | Command Characters   | Action Executed                                    |
  +----------------------+----------------------------------------------------+
  | 'F' / 'f'            | Move Forward                                       |
  | 'B' / 'b'            | Move Backward                                      |
  | 'L' / 'l'            | Point Spin Left (Zero radius)                      |
  | 'R' / 'r'            | Point Spin Right (Zero radius)                     |
  | 'G' / 'g'            | Forward-Left Differential Curve                    |
  | 'I' / 'i'            | Forward-Right Differential Curve                   |
  | 'H' / 'h'            | Backward-Left Differential Curve                   |
  | 'J' / 'j'            | Backward-Right Differential Curve                  |
  | 'S' / 's' / 'D' / 'd'| Full Stop (All motor drivers disabled)             |
  | '0' .. '9'           | Speed Presets (0 = 70 PWM ... 9 = 255 PWM)         |
  | 'Q' / 'q'            | Maximum Speed (255 PWM)                            |
  +----------------------+----------------------------------------------------+

  Expected Behavior:
  1. Pairing: HC-05 LED blinks rapidly until paired with smartphone (default PIN: 1234 or 0000).
  2. Teleoperation: Robot responds immediately to incoming touch / D-pad commands.
  3. Safety Timeout: If out of range or disconnected, the robot halts within 1000ms.
  4. Diagnostics are displayed on the Arduino Serial Monitor at 9600 baud.

  Electrical Safety Notes:
  - The HC-05 RXD pin is rated for 3.3V logic. Connecting Arduino D13 (5V output) directly without
    the 1k/2k resistor voltage divider may degrade or damage the Bluetooth baseband IC over time.

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

#include <SoftwareSerial.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Bluetooth SoftwareSerial interface
#define BT_RX_PIN 12   // Arduino RX <- HC-05 TXD
#define BT_TX_PIN 13   // Arduino TX -> HC-05 RXD (via 3.3V divider)

// Left Motor Driver Pins
#define ENA 5   // Timer0 PWM
#define IN1 2
#define IN2 3

// Right Motor Driver Pins
#define ENB 6   // Timer0 PWM
#define IN3 4
#define IN4 7

// =====================================================
// OBJECT INSTANTIATION & STATE VARIABLES
// =====================================================

SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);

int currentSpeed = 200; // Default cruise PWM speed (0 - 255)

// Communication timeout watchdog timer
unsigned long lastCommandTime = 0;
const unsigned long WATCHDOG_TIMEOUT_MS = 1000;
bool isMoving = false;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void handleCommand(char rawCmd, const char* source);
void setSpeedPreset(int spd);
void moveForward(int speed);
void moveBackward(int speed);
void spinLeft(int speed);
void spinRight(int speed);
void curveDrive(int leftSpeed, int rightSpeed);
void curveReverse(int leftSpeed, int rightSpeed);
void stopRobot();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600); // Standard factory baud rate for HC-05/06

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Safe initialization
  stopRobot();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.6: Wireless Bluetooth Robocar "));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.println(F("[INFO] Bluetooth interface ready on Pins 12(RX) and 13(TX)"));
  Serial.println(F("[INFO] Commands: F=Forward, B=Back, L=Left, R=Right, S=Stop, 0-9=Speed"));
  Serial.println(F("[INFO] Communication watchdog timeout: 1000ms\n"));

  BTSerial.println(F("NIELIT Bluetooth Robocar Ready!"));
  lastCommandTime = millis();
}

// =====================================================
// MAIN LOOP
// =====================================================

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

  // Communication Timeout Check: Stop robot if command stream ceases while in motion
  if (isMoving && (millis() - lastCommandTime > WATCHDOG_TIMEOUT_MS)) {
    Serial.println(F("[WATCHDOG] Communication timeout reached. Auto-Stopping."));
    stopRobot();
  }
}

// =====================================================
// PROTOCOL PARSER
// =====================================================

void handleCommand(char rawCmd, const char* source) {
  // Ignore whitespace, carriage return, and newline characters
  if (rawCmd == '\r' || rawCmd == '\n' || rawCmd == ' ') return;

  lastCommandTime = millis();

  Serial.print(F("["));
  Serial.print(source);
  Serial.print(F("] Command: '"));
  Serial.print(rawCmd);
  Serial.println(F("'"));

  // Check for numeric speed preset commands '0' through '9'
  if (rawCmd >= '0' && rawCmd <= '9') {
    const int speedMap[] = {70, 90, 110, 130, 150, 170, 190, 210, 235, 255};
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

    case 'G': // Forward Left Curve (Differential)
      curveDrive(currentSpeed / 3, currentSpeed);
      break;

    case 'I': // Forward Right Curve (Differential)
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

    case 'Q': // Maximum Velocity
      setSpeedPreset(255);
      break;

    default:
      Serial.print(F("[WARN] Unrecognized command: "));
      Serial.println(cmd);
      break;
  }
}

void setSpeedPreset(int spd) {
  currentSpeed = constrain(spd, 0, 255);
  Serial.print(F("[INFO] Speed updated to: "));
  Serial.println(currentSpeed);
  BTSerial.print(F("Speed: "));
  BTSerial.println(currentSpeed);
}

// =====================================================
// MOTION PRIMITIVES
// =====================================================

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
