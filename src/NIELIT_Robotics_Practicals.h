/*
  NIELIT Robotics Practicals Library
  ----------------------------------
  Course helper library for NIELIT Robotics practicals (3.1 - 3.8).

  This library provides installable Arduino examples covering:
  - 3.1 Robotics Assembly & Power Distribution Dynamics
  - 3.2 Inductive Load Control — Interfacing the L293D Motor Driver Shield
  - 3.3 Direct Current Actuation — Interfacing DC Gear Motors
  - 3.4 Kinematic Maneuvers — 2-Wheel Robocar Basic Movements
  - 3.5 Speed Vector Regulation — PWM Motor Control
  - 3.6 Wireless Kinematic – Bluetooth Interfacing
  - 3.7 Autonomous Navigation Capstone I — Line-Following Robot
  - 3.8 Autonomous Navigation Capstone II — Obstacle-Avoiding Vehicle

  After installation in Arduino IDE:
    File -> Examples -> NIELIT Robotics Practicals
*/

#ifndef NIELIT_ROBOTICS_PRACTICALS_H
#define NIELIT_ROBOTICS_PRACTICALS_H

#define NIELIT_ROBOTICS_PRACTICALS_VERSION "1.0.3"

#include <Arduino.h>

namespace NIELIT {
namespace Robotics {

/**
 * Returns the current version string of the NIELIT Robotics Practicals library.
 */
const char* version();

/**
 * Prints library title and version information to the specified Stream.
 */
inline void printLibraryInfo(Stream &out = Serial) {
  out.println(F("========================================"));
  out.println(F("NIELIT Robotics Practicals Library"));
  out.print(F("Version: "));
  out.println(F(NIELIT_ROBOTICS_PRACTICALS_VERSION));
  out.println(F("========================================"));
}

/**
 * Prints a standardized banner header for a practical session.
 */
inline void printPracticalHeader(Stream &out, const char *number, const char *title) {
  out.println();
  out.println(F("========================================"));
  out.print(F("NIELIT Robotics Practical "));
  out.println(number);
  out.println(title);
  out.println(F("========================================"));
}

/**
 * Direction enum for differential drive motor states.
 */
enum Direction {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  SPIN_LEFT,
  SPIN_RIGHT
};

} // namespace Robotics
} // namespace NIELIT

#endif // NIELIT_ROBOTICS_PRACTICALS_H
