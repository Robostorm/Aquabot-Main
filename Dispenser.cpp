#include <Arduino.h>

#include "Dispenser.h"

#include "common.h"
#include "states.h"
#include "conf.h"
#include "pins.h"

int Dispenser::bottles = 10;
int Dispenser::bottleSold = 0;
int Dispenser::coolerTemp = 0;

int Dispenser::state = DISPREADY;
int Dispenser::ledState = 0;
int Dispenser::irState = 0;
int Dispenser::photoState = 0;

PWMServo Dispenser::servo = PWMServo();

int Dispenser::motorPower = 0;
int Dispenser::servoPos = 45;
int Dispenser::servoState = 0;
int Dispenser::servoWait = 0;

void Dispenser::init(){

  servo.attach(SERVOPIN);
  pinMode(IRPIN, INPUT);
  pinMode(PHOTOPIN, INPUT);
  pinMode(BILLPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
}

int Dispenser::update(unsigned long now){
  sensorUpdate(now);
  motorUpdate(now);

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
    bottles--;
    bottleSold++;
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

  Serial.println(now-servoMillis);

  if(now - servoMillis >= SERVODELAY){

    if(servoState == 0){
      if(servoPos < SERVOMAX){
        servoPos += SERVOSTEP;
      }else{
        servoState = 1;
      }
    }

    if(servoState == 1){
      if(servoWait < SERVOSTOP/SERVODELAY){
        servoWait++;
      }else{
        servoState = 2;
      }
    }

    if(servoState == 2){
      if(servoPos > SERVOMIN){
        servoPos -= SERVOSTEP;
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

void Dispenser::sensorUpdate(unsigned long now){
  static unsigned long sensorMillis = 0UL;

  if(now - sensorMillis >= SENSEDELAY){
    irState = digitalRead(IRPIN);
    photoState = digitalRead(PHOTOPIN);
    sensorMillis = now;
  }
}

void Dispenser::motorUpdate(unsigned long now){
  static unsigned long motorMillis = 0UL;

  if(now - motorMillis >= MOTORDELAY){
    analogWrite(BILLPIN, motorPower);
    servo.write(servoPos);
    motorMillis = now;
  }
}
