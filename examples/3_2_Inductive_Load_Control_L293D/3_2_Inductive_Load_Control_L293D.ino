/*
  =========================================================
  NIELIT Robotics Practicals
  Practical 3.2: Inductive Load Control — Interfacing the L293D Motor Driver Shield
  =========================================================

  Objective:
  Understand the operating principles of an H-Bridge motor driver circuit,
  inductive back-EMF suppression, and state control (Forward, Reverse, Dynamic Braking, Coasting).

  Description:
  Demonstrates inductive load switching using the L293D Quadruple Half-H Driver IC.
  Executes a sequential state machine cycling through all fundamental H-bridge operating modes:
  Forward Rotation, Active Dynamic Braking, Reverse Rotation, Coasting Stop, and PWM Speed Modulation.

  Hardware:
  - Arduino UNO R3 (or compatible AVR development board)
  - L293D Dual H-Bridge Driver IC / Motor Shield
  - DC BO Gear Motor (Nominal: 3V - 6V)
  - External Motor Power Supply: 6.0V - 7.4V (e.g. 4x AA Battery Pack)

  Pin Configuration:
  -------------------------------------------------------------
  L293D Pin                Arduino UNO Pin   Function
  -------------------------------------------------------------
  Pin 1 (1,2EN / ENA)      Pin 5 (PWM)       Channel A Speed Enable
  Pin 2 (1A / IN1)         Pin 2             Channel A Direction Input 1
  Pin 7 (2A / IN2)         Pin 3             Channel A Direction Input 2
  Pin 3, 6 (1Y, 2Y)        Motor Terminals   Channel A Motor Outputs
  Pin 4, 5, 12, 13 (GND)   GND               Common Ground & Heat Sink
  Pin 8 (VCC2 / VM)        Battery (+)       Motor Power Supply (6.0V - 7.4V Recommended)
  Pin 16 (VCC1 / VSS)      5V                Logic Power Supply (Arduino 5V Regulated)
  -------------------------------------------------------------

  Electrical & Device Characteristics:
  - The L293D incorporates internal flyback clamping diodes across each half-H driver to safely
    dissipate inductive back-EMF when current is switched off.
    (Note: If replacing L293D with a bare L298N IC, external fast-recovery diodes are required).
  - Maximum continuous current per channel on L293D is 600mA (1.2A non-repetitive peak).
  - Operating logic levels: High level input V_IH >= 2.3V, Low level input V_IL <= 1.5V.

  H-Bridge Truth Table:
  +-----+-----+-----+------------------------+----------------------------------------------------+
  | ENA | IN1 | IN2 | Motor Operating State  | Electrical & Physical Condition                    |
  +-----+-----+-----+------------------------+----------------------------------------------------+
  |  H  |  H  |  L  | Forward (Clockwise)    | OUT1 = VM - V_sat, OUT2 = GND + V_sat (Current A->B)
  |  H  |  L  |  H  | Reverse (Counter-CW)   | OUT1 = GND + V_sat, OUT2 = VM - V_sat (Current B->A)
  |  H  |  L  |  L  | Active Dynamic Brake   | OUT1 & OUT2 clamped to Low Rail (Back-EMF damped)   |
  |  H  |  H  |  H  | Active Dynamic Brake   | OUT1 & OUT2 clamped to High Rail (Back-EMF damped)  |
  |  L  |  X  |  X  | Coasting Stop (High-Z) | Output transistors disabled (Inertial spin-down)   |
  +-----+-----+-----+------------------------+----------------------------------------------------+

  Expected Behavior:
  1. State 1: Motor rotates Forward at full speed for 2.5 seconds (blocking delay for sequence demonstration).
  2. State 2: Motor executes an active dynamic brake for 1.0 second (rapid deceleration).
  3. State 3: Motor rotates Reverse at full speed for 2.5 seconds.
  4. State 4: Motor coasts to a gradual stop for 2.0 seconds with driver outputs in high impedance (High-Z).
  5. State 5: Motor demonstrates PWM speed regulation across 40%, 70%, and 100% duty cycles.
  6. Real-time state descriptions are streamed to the Serial Monitor at 9600 baud.

  Notes:
  - Dynamic braking works by shorting both motor terminals together through the low-side or high-side
    transistors. The rotor's kinetic energy drives a generator current that creates opposing electromagnetic
    counter-torque (Lenz's Law).

  Author/Organization:
  National Institute of Electronics & Information Technology
  NIELIT Ropar

  =========================================================
*/

// =====================================================
// PIN DEFINITIONS
// =====================================================

#define ENA 5   // PWM Speed Enable (Timer0, ~976 Hz)
#define IN1 2   // Direction Input 1
#define IN2 3   // Direction Input 2

// =====================================================
// SPEED & TIMING CONSTANTS
// =====================================================

const int FULL_SPEED_PWM = 255;
const int RUN_TIME_MS    = 2500;
const int BRAKE_TIME_MS  = 1000;
const int COAST_TIME_MS  = 2000;

// =====================================================
// FUNCTION PROTOTYPES
// =====================================================

void stopCoast();
void dynamicBrake();

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Safe initialization: start with motor in coasting stop
  stopCoast();

  Serial.println(F("=================================================="));
  Serial.println(F(" NIELIT Practical 3.2: L293D Inductive Load Control"));
  Serial.println(F("=================================================="));
  Serial.println(F("[INFO] System initialized"));
  Serial.println(F("[INFO] Starting H-Bridge state machine verification in 2s...\n"));
  delay(2000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // ---------------------------------------------------
  // State 1: Forward Rotation (Clockwise)
  // ---------------------------------------------------
  Serial.println(F("[STATE 1] FORWARD / CLOCKWISE (IN1=HIGH, IN2=LOW, ENA=255)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, FULL_SPEED_PWM);
  delay(RUN_TIME_MS);

  // ---------------------------------------------------
  // State 2: Active Dynamic Braking (Low-Side Clamp)
  // ---------------------------------------------------
  Serial.println(F("[STATE 2] ACTIVE DYNAMIC BRAKE (IN1=LOW, IN2=LOW, ENA=255)"));
  dynamicBrake();
  delay(BRAKE_TIME_MS);

  // ---------------------------------------------------
  // State 3: Reverse Rotation (Counter-Clockwise)
  // ---------------------------------------------------
  Serial.println(F("[STATE 3] REVERSE / COUNTER-CLOCKWISE (IN1=LOW, IN2=HIGH, ENA=255)"));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, FULL_SPEED_PWM);
  delay(RUN_TIME_MS);

  // ---------------------------------------------------
  // State 4: Coasting Stop (High-Impedance Floating)
  // ---------------------------------------------------
  Serial.println(F("[STATE 4] COASTING STOP (ENA=0, Driver High-Z)"));
  stopCoast();
  delay(COAST_TIME_MS);

  // ---------------------------------------------------
  // State 5: Pulse Width Modulation Speed Control
  // ---------------------------------------------------
  Serial.println(F("[STATE 5] PWM SPEED REGULATION DEMO:"));

  Serial.println(F(" -> ~40% Duty Cycle (ENA=102)"));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 102);
  delay(1500);

  Serial.println(F(" -> ~70% Duty Cycle (ENA=178)"));
  analogWrite(ENA, 178);
  delay(1500);

  Serial.println(F(" -> 100% Full Power (ENA=255)"));
  analogWrite(ENA, 255);
  delay(1500);

  stopCoast();
  Serial.println(F("\n[INFO] State machine test cycle complete. Pausing 3 seconds...\n"));
  delay(3000);
}

// =====================================================
// HELPER FUNCTIONS
// =====================================================

void stopCoast() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void dynamicBrake() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255); // Keep enable asserted to damp back-EMF through ground
}
