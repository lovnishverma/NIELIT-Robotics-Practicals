/*
  =========================================================
  NIELIT Robotics Practicals Library
  =========================================================
  Core helper library for NIELIT Robotics practicals (3.1 to 3.8).
  Provides beginner-friendly motor control primitives, differential drive kinematics,
  speed trim calibration, and unified support for both L298N modules and L293D Motor Shields.

  National Institute of Electronics & Information Technology (NIELIT Ropar)
  =========================================================
*/

#ifndef NIELIT_ROBOTICS_PRACTICALS_H
#define NIELIT_ROBOTICS_PRACTICALS_H

#include <Arduino.h>

#define NIELIT_ROBOTICS_PRACTICALS_VERSION "1.0.0"

namespace NIELIT {
namespace Robotics {

/**
 * Returns current library version string.
 */
const char* version();

/**
 * Direction states for mobile robot movement.
 */
enum RobotDirection {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  SPIN_LEFT,
  SPIN_RIGHT
};

/**
 * Prints a clean, beginner-friendly banner to the Serial Monitor.
 */
inline void printPracticalHeader(Stream &out, const char *number, const char *title) {
  out.println();
  out.println(F("=================================================="));
  out.print(F(" NIELIT Robotics Practical "));
  out.println(number);
  out.print(F(" "));
  out.println(title);
  out.println(F("=================================================="));
}

/**
 * Prints the library title and version.
 */
inline void printLibraryInfo(Stream &out = Serial) {
  out.println(F("=================================================="));
  out.println(F(" NIELIT Robotics Practicals Library"));
  out.print(F(" Version: "));
  out.println(F(NIELIT_ROBOTICS_PRACTICALS_VERSION));
  out.println(F("=================================================="));
}

/**
 * Simple 2WD Differential Drive Robot Controller
 * Supports standard GPIO (L298N / discrete L293D IC) with built-in speed trim.
 */
class DifferentialRobot {
private:
  uint8_t _pinENA;
  uint8_t _pinIN1;
  uint8_t _pinIN2;
  uint8_t _pinENB;
  uint8_t _pinIN3;
  uint8_t _pinIN4;
  int _trimLeft;
  int _trimRight;

public:
  DifferentialRobot(uint8_t ena = 5, uint8_t in1 = 2, uint8_t in2 = 3,
                    uint8_t enb = 6, uint8_t in3 = 4, uint8_t in4 = 7)
    : _pinENA(ena), _pinIN1(in1), _pinIN2(in2),
      _pinENB(enb), _pinIN3(in3), _pinIN4(in4),
      _trimLeft(0), _trimRight(0) {}

  void begin() {
    pinMode(_pinENA, OUTPUT);
    pinMode(_pinIN1, OUTPUT);
    pinMode(_pinIN2, OUTPUT);
    pinMode(_pinENB, OUTPUT);
    pinMode(_pinIN3, OUTPUT);
    pinMode(_pinIN4, OUTPUT);
    stop();
  }

  void setTrim(int trimLeft, int trimRight) {
    _trimLeft = trimLeft;
    _trimRight = trimRight;
  }

  void setLeftMotor(bool forward, int speed) {
    int actualSpeed = constrain(speed + _trimLeft, 0, 255);
    if (speed <= 0 || actualSpeed == 0) {
      digitalWrite(_pinIN1, LOW);
      digitalWrite(_pinIN2, LOW);
      analogWrite(_pinENA, 0);
    } else {
      if (forward) {
        digitalWrite(_pinIN1, HIGH);
        digitalWrite(_pinIN2, LOW);
      } else {
        digitalWrite(_pinIN1, LOW);
        digitalWrite(_pinIN2, HIGH);
      }
      analogWrite(_pinENA, actualSpeed);
    }
  }

  void setRightMotor(bool forward, int speed) {
    int actualSpeed = constrain(speed + _trimRight, 0, 255);
    if (speed <= 0 || actualSpeed == 0) {
      digitalWrite(_pinIN3, LOW);
      digitalWrite(_pinIN4, LOW);
      analogWrite(_pinENB, 0);
    } else {
      if (forward) {
        digitalWrite(_pinIN3, HIGH);
        digitalWrite(_pinIN4, LOW);
      } else {
        digitalWrite(_pinIN3, LOW);
        digitalWrite(_pinIN4, HIGH);
      }
      analogWrite(_pinENB, actualSpeed);
    }
  }

  void forward(int speed = 200) {
    setLeftMotor(true, speed);
    setRightMotor(true, speed);
  }

  void backward(int speed = 200) {
    setLeftMotor(false, speed);
    setRightMotor(false, speed);
  }

  void turnLeft(int speed = 200) {
    setLeftMotor(true, 0);       // Left wheel stops
    setRightMotor(true, speed);  // Right wheel drives forward
  }

  void turnRight(int speed = 200) {
    setLeftMotor(true, speed);   // Left wheel drives forward
    setRightMotor(true, 0);      // Right wheel stops
  }

  void spinLeft(int speed = 180) {
    setLeftMotor(false, speed);  // Left wheel reverses
    setRightMotor(true, speed);  // Right wheel drives forward
  }

  void spinRight(int speed = 180) {
    setLeftMotor(true, speed);   // Left wheel drives forward
    setRightMotor(false, speed); // Right wheel reverses
  }

  void stop() {
    setLeftMotor(true, 0);
    setRightMotor(true, 0);
  }
};

} // namespace Robotics
} // namespace NIELIT

#endif // NIELIT_ROBOTICS_PRACTICALS_H
