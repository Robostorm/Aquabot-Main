//-*-C++-*-

#include <Servo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

class Dispenser{
  
private:
  
  int state = DISPREADY;
  int ledState = 0;
  int irState = 0;
  int photoState = 0;
  
  Servo servo;
  
  int motorPower = 0;
  int servoPos = 45;
  
  int servoState = 0;
  int servoWait = 0;
  
  void dispense(unsigned long now);
  
public:
  int update(unsigned long now);
  void setMotorPower(int power);
  void setServoPos(int pos);
  int getServoPos();
  int getMotorPower();
  int getLedState();
  int getIrState();
  int getPhotoState();
};
