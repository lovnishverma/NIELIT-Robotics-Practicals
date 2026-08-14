/*
  NIELIT Robotics Practicals
  Practical 3.8: Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle

  Objective:
  Build an autonomous collision-avoiding robot using an HC-SR04 ultrasonic
  distance sensor to detect obstacles ahead and automatically steer around them.

  Wiring (HC-SR04 & L298N to Arduino):
  - HC-SR04: VCC -> 5V, GND -> GND, TRIG -> Pin 9, ECHO -> Pin 10
  - Left Motor:   ENA -> Pin 5 (PWM), IN1 -> Pin 2, IN2 -> Pin 3
  - Right Motor:  ENB -> Pin 6 (PWM), IN3 -> Pin 4, IN4 -> Pin 7
  - Motor Power:  6V - 7.4V Battery Pack (+ to 12V/VM, - to GND)
*/

// Ultrasonic Sensor Pins
const int PIN_TRIG = 9;
const int PIN_ECHO = 10;

// Motor Driver Pins
const int PIN_ENA = 5;
const int PIN_IN1 = 2;
const int PIN_IN2 = 3;
const int PIN_ENB = 6;
const int PIN_IN3 = 4;
const int PIN_IN4 = 7;

// Distance and Speed Settings
const int OBSTACLE_DISTANCE_CM = 25;  // Obstacle detection threshold (cm)
const int CRUISE_SPEED         = 180; // Forward driving speed (0 - 255)
const int TURN_SPEED           = 180; // Avoidance turn speed (0 - 255)

// Motor Speed Trim
const int LEFT_TRIM  = 0;
const int RIGHT_TRIM = 0;

void setup() {
  Serial.begin(9600);

  // Configure sensor pins
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Configure motor driver pins
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  stopRobot();

  Serial.println("NIELIT Robotics Practical 3.8");
  Serial.println("Autonomous Obstacle-Avoiding Robot");
  Serial.println("Starting in 3 seconds...\n");
  delay(3000);
}

void loop() {
  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If path is clear, drive forward
  if (distance > OBSTACLE_DISTANCE_CM || distance == 0) {
    moveForward(CRUISE_SPEED);
  } else {
    // Obstacle detected within 25 cm!
    Serial.println("[Alert] Obstacle detected! Backing up and turning...");

    // Step 1: Stop
    stopRobot();
    delay(200);

    // Step 2: Reverse slightly
    moveBackward(CRUISE_SPEED, 400);
    stopRobot();
    delay(200);

    // Step 3: Spin turn right
    spinTurn(TURN_SPEED, 500, true);
    stopRobot();
    delay(200);
  }

  delay(60); // Small interval between readings
}

// Ultrasonic Sensor Reading Helper

long readDistanceCM() {
  // Clear trigger pin
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  // Send 10us trigger pulse
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read return echo duration (timeout after 25ms)
  long duration = pulseIn(PIN_ECHO, HIGH, 25000);

  if (duration == 0) {
    return 999; // Out of range or no echo received
  }

  // Convert travel time to distance in centimeters (speed of sound = 343 m/s)
  long distanceCm = duration / 58;
  return distanceCm;
}

// Motor Control Helper Functions

void moveForward(int speed) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);
}

void moveBackward(int speed, int durationMs) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);

  delay(durationMs);
}

void spinTurn(int speed, int durationMs, bool turnRight) {
  int leftSpd  = constrain(speed + LEFT_TRIM, 0, 255);
  int rightSpd = constrain(speed + RIGHT_TRIM, 0, 255);

  if (turnRight) {
    // Left Forward, Right Reverse
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  } else {
    // Left Reverse, Right Forward
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  }

  analogWrite(PIN_ENA, leftSpd);
  analogWrite(PIN_ENB, rightSpd);

  delay(durationMs);
}

void stopRobot() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}
