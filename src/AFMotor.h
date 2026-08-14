// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#ifndef _AFMotor_h_
#define _AFMotor_h_

#include <inttypes.h>
#include <avr/io.h>

#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// Constants that the user passes in to the motor calls
#define MOTOR12_64KHZ _BV(CS20)  // no prescale
#define MOTOR12_8KHZ  _BV(CS21)  // divide by 8
#define MOTOR12_2KHZ  (_BV(CS21) | _BV(CS20)) // divide by 32
#define MOTOR12_1KHZ  _BV(CS22)  // divide by 64

#define MOTOR34_64KHZ _BV(CS00)  // no prescale
#define MOTOR34_8KHZ  _BV(CS01)  // divide by 8
#define MOTOR34_1KHZ  (_BV(CS01) | _BV(CS00)) // divide by 64

#define DC_MOTOR_PWM_RATE   MOTOR12_8KHZ // 8KHz default for DC motors

// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

class AFMotorController {
  public:
    AFMotorController(void);
    void enable(void);
    void latch_tx(void);
    uint8_t TimerInitalized;
};

class AF_DCMotor {
 public:
  AF_DCMotor(uint8_t motornum, uint8_t freq = DC_MOTOR_PWM_RATE);
  void run(uint8_t);
  void setSpeed(uint8_t);

 private:
  uint8_t motornum, pwmfreq;
};

#endif
