#include <Arduino.h>

#include "Dispenser.h"

int Dispenser::update(unsigned long now){
  
  switch(state){
  case DISPREADY:
    if(irState == LOW){
      state = GETTING;
    }
    servoState = 0;
    servoWait = 0;
    break;
  case DISPGETTING:
    motorPower = 255;
    if(photoState == HIGH) {
      state = DISPENSING;
    }
    break;
  case DISPDISPENSING:
    if(photoState == HIGH){
      break;
    }else{
      motorPower = 0;
      dispense(now);
    }
    break;
        
  case DISPDONE:
    state = READY;
    break;
  }
  return 1;
}

void Dispenser::setMotorPower(int power){
  motorPower = power;
}

void Dispenser::setServoPos(int pos){
  servoPos = pos;
}

void Dispenser::dispense(unsigned long now){
    
  static unsigned long servoMillis = 0UL;
  
  if(now - servoMillis >= 10UL){
        
    if(servoState == 0){
      if(servoPos < 135){
        servoPos++;
      }else{
        servoState = 1;
      }
    }
    
    if(servoState == 1){
      if(servoWait < 40){
        servoWait++;
      }else{
        servoState = 2;
      }
    }
    
    if(servoState == 2){
      if(servoPos > 45){
        servoPos--;
      }else{
        servoState = 3;
      }
    }
    
    if(servoState == 3){
      state = DONE;
    }
    servoMillis = now;
  }
}
