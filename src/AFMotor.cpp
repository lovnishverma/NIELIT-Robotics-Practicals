// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <Arduino.h>
#include <avr/io.h>
#include "AFMotor.h"

static uint8_t latch_state;

AFMotorController::AFMotorController(void) {
  TimerInitalized = 0;
}

void AFMotorController::enable(void) {
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);

  latch_state = 0;
  latch_tx();
  digitalWrite(MOTORENABLE, LOW);
}

void AFMotorController::latch_tx(void) {
  uint8_t i;
  digitalWrite(MOTORLATCH, LOW);
  digitalWrite(MOTORDATA, LOW);

  for (i = 0; i < 8; i++) {
    digitalWrite(MOTORCLK, LOW);
    if (latch_state & _BV(7 - i)) {
      digitalWrite(MOTORDATA, HIGH);
    } else {
      digitalWrite(MOTORDATA, LOW);
    }
    digitalWrite(MOTORCLK, HIGH);
  }
  digitalWrite(MOTORLATCH, HIGH);
}

static AFMotorController MC;

static void initPWM1(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  TCCR2A |= _BV(COM2A1) | _BV(WGM20) | _BV(WGM21); // fast PWM
  TCCR2B = freq & 0x7;
  OCR2A = 0;
  pinMode(11, OUTPUT);
#endif
}

static void setPWM1(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  OCR2A = s;
#endif
}

static void initPWM2(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  TCCR2A |= _BV(COM2B1) | _BV(WGM20) | _BV(WGM21); // fast PWM
  TCCR2B = freq & 0x7;
  OCR2B = 0;
  pinMode(3, OUTPUT);
#endif
}

static void setPWM2(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  OCR2B = s;
#endif
}

static void initPWM3(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01); // fast PWM
  OCR0A = 0;
  pinMode(6, OUTPUT);
#endif
}

static void setPWM3(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  OCR0A = s;
#endif
}

static void initPWM4(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  TCCR0A |= _BV(COM0B1) | _BV(WGM00) | _BV(WGM01); // fast PWM
  OCR0B = 0;
  pinMode(5, OUTPUT);
#endif
}

static void setPWM4(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
  OCR0B = s;
#endif
}

AF_DCMotor::AF_DCMotor(uint8_t num, uint8_t freq) {
  motornum = num;
  pwmfreq = freq;

  MC.enable();

  switch (num) {
    case 1:
      latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B);
      MC.latch_tx();
      initPWM1(freq);
      break;
    case 2:
      latch_state &= ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B);
      MC.latch_tx();
      initPWM2(freq);
      break;
    case 3:
      latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B);
      MC.latch_tx();
      initPWM3(freq);
      break;
    case 4:
      latch_state &= ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B);
      MC.latch_tx();
      initPWM4(freq);
      break;
  }
}

void AF_DCMotor::run(uint8_t cmd) {
  uint8_t a, b;
  switch (motornum) {
    case 1:
      a = MOTOR1_A; b = MOTOR1_B; break;
    case 2:
      a = MOTOR2_A; b = MOTOR2_B; break;
    case 3:
      a = MOTOR3_A; b = MOTOR3_B; break;
    case 4:
      a = MOTOR4_A; b = MOTOR4_B; break;
    default:
      return;
  }

  switch (cmd) {
    case FORWARD:
      latch_state |= _BV(a);
      latch_state &= ~_BV(b);
      MC.latch_tx();
      break;
    case BACKWARD:
      latch_state &= ~_BV(a);
      latch_state |= _BV(b);
      MC.latch_tx();
      break;
    case RELEASE:
      latch_state &= ~_BV(a);
      latch_state &= ~_BV(b);
      MC.latch_tx();
      break;
  }
}

void AF_DCMotor::setSpeed(uint8_t speed) {
  switch (motornum) {
    case 1:
      setPWM1(speed); break;
    case 2:
      setPWM2(speed); break;
    case 3:
      setPWM3(speed); break;
    case 4:
      setPWM4(speed); break;
  }
}
