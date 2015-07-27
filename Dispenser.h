//-*-C++-*-
/* File Dispenser.h  */
#ifndef DISPENSER
#define DISPENSER

#include <PWMServo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

class Dispenser{

private:
  static int state;
  static int ledState;
  static int irState;
  static int photoState;

  static PWMServo servo;

  static int motorPower;
  static int servoPos;

  static int servoState;
  static int servoWait;

  static void dispense(unsigned long now);

  static void sensorUpdate(unsigned long now);
  static void motorUpdate(unsigned long now);

public:
  static int bottles;
  static int bottleSold;
  static int coolerTemp;

  static void init();
  static int update(unsigned long now);
  static void setMotorPower(int power);
  static void setServoPos(int pos);
  static int getServoPos();
  static int getMotorPower();
  static int getLedState();
  static int getIrState();
  static int getPhotoState();
};

#endif /* !DISPENSER */
