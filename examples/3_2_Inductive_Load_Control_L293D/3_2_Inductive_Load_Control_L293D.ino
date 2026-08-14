/*
  NIELIT Robotics Practicals
  Practical 3.2: Motor Driver H-Bridge Control (L293D / L298N)

  Objective:
  Understand how an H-Bridge motor driver controls the direction and speed of a DC motor
  using control inputs (Forward, Reverse, Active Brake, Coasting Stop, and PWM Speed).

  Wiring:
  - ENA (Speed PWM) -> Pin 5
  - IN1 (Direction 1) -> Pin 2
  - IN2 (Direction 2) -> Pin 3
  - Logic Power: 5V -> Arduino 5V, GND -> Arduino GND
  - Motor Power: 6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
*/

// Pin Definitions
const int PIN_ENA = 5; // Speed control (PWM 0-255)
const int PIN_IN1 = 2; // Direction Pin 1
const int PIN_IN2 = 3; // Direction Pin 2

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  // Start with motor off
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 0);

  Serial.println("NIELIT Robotics Practical 3.2");
  Serial.println("Motor Driver H-Bridge Control Demonstration");
  Serial.println("Starting in 2 seconds...\n");
  delay(2000);
}

void loop() {
  // State 1: Forward Rotation
  Serial.println("[State 1] FORWARD (IN1 = HIGH, IN2 = LOW, ENA = 255)");
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 255);
  delay(3000);

  // State 2: Active Brake
  Serial.println("[State 2] ACTIVE BRAKE (IN1 = LOW, IN2 = LOW, ENA = 255)");
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 255);
  delay(1500);

  // State 3: Reverse Rotation
  Serial.println("[State 3] REVERSE (IN1 = LOW, IN2 = HIGH, ENA = 255)");
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENA, 255);
  delay(3000);

  // State 4: Coasting Stop
  Serial.println("[State 4] COASTING STOP (ENA = 0)");
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 0);
  delay(2000);

  // State 5: PWM Half Speed
  Serial.println("[State 5] PWM SPEED CONTROL (Forward at 50% Speed: ENA = 128)");
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 128);
  delay(3000);

  // Stop motor
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);

  Serial.println("\nDemonstration complete. Repeating in 4 seconds...\n");
  delay(4000);
}
