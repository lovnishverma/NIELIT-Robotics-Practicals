/*
  Practical 3.2: Inductive Load Control — Interfacing the L293D Motor Driver Shield
  Course: NIELIT Robotics Practicals

  Description:
  Demonstrates H-Bridge switching logic, inductive back-EMF suppression,
  and motor state control (Forward, Reverse, Active Dynamic Brake, Coasting Stop,
  and PWM Speed Regulation) using the L293D Quadruple Half-H Driver IC / Shield.

  L293D H-Bridge Truth Table:
  +-----+-----+-----+------------------------+------------------------------------+
  | ENA | IN1 | IN2 | Motor Operating State  | Electrical Condition               |
  +-----+-----+-----+------------------------+------------------------------------+
  |  H  |  H  |  L  | Forward (Clockwise)    | OUT1=VM, OUT2=GND (Current flows)  |
  |  H  |  L  |  H  | Reverse (Counter-CW)   | OUT1=GND, OUT2=VM (Current reversed)|
  |  H  |  H  |  H  | Active Dynamic Brake   | OUT1=VM, OUT2=VM (Braked to supply)|
  |  H  |  L  |  L  | Active Dynamic Brake   | OUT1=GND, OUT2=GND (Braked to GND) |
  |  L  |  X  |  X  | Coasting Stop (High-Z) | Half-H Drivers Disabled (Floating) |
  +-----+-----+-----+------------------------+------------------------------------+

  Hardware Connections:
  -------------------------------------------------------------
  L293D Pin             Arduino Pin      Function
  -------------------------------------------------------------
  Pin 1 (1,2EN)         Pin 5 (PWM)      Channel A Enable / Speed
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

  // Initialize motor in safe coasting state
  stopCoast();

  Serial.println(F("=================================================="));
  Serial.println(F(" Practical 3.2: L293D Inductive Load Control     "));
  Serial.println(F("=================================================="));
  Serial.println(F("Starting H-Bridge state machine verification in 2s...\n"));
  delay(2000);
}

void loop() {
  // -------------------------------------------------------------
  // State 1: Forward Rotation (Clockwise)
  // -------------------------------------------------------------
  Serial.println(F("[State 1] FORWARD / CLOCKWISE (IN1=HIGH, IN2=LOW, ENA=255)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255);
  delay(2500);

  // -------------------------------------------------------------
  // State 2: Active Dynamic Braking (Low-Side Clamp)
  // -------------------------------------------------------------
  Serial.println(F("[State 2] ACTIVE DYNAMIC BRAKE (IN1=LOW, IN2=LOW, ENA=255)"));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255); // Keep enable high to actively damp kinetic momentum
  delay(1000);

  // -------------------------------------------------------------
  // State 3: Reverse Rotation (Counter-Clockwise)
  // -------------------------------------------------------------
  Serial.println(F("[State 3] REVERSE / COUNTER-CLOCKWISE (IN1=LOW, IN2=HIGH, ENA=255)"));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);
  delay(2500);

  // -------------------------------------------------------------
  // State 4: Coasting Stop (High Impedance Floating)
  // -------------------------------------------------------------
  Serial.println(F("[State 4] COASTING STOP (ENA=0, Drivers disabled)"));
  stopCoast();
  delay(2000);

  // -------------------------------------------------------------
  // State 5: Pulse Width Modulation Speed Control
  // -------------------------------------------------------------
  Serial.println(F("[State 5] PWM SPEED REGULATION DEMO:"));
  
  Serial.println(F(" -> 40% PWM Duty Cycle (ENA=102)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 102);
  delay(1500);

  Serial.println(F(" -> 70% PWM Duty Cycle (ENA=178)"));
  analogWrite(ENA, 178);
  delay(1500);

  Serial.println(F(" -> 100% Full Power (ENA=255)"));
  analogWrite(ENA, 255);
  delay(1500);

  stopCoast();
  Serial.println(F("\n--- H-Bridge Test Cycle Complete. Restarting in 3s ---\n"));
  delay(3000);
}

void stopCoast() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}
