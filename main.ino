//-*-C++-*-
//https://github.com/Robostorm/Aquabot-Bill-Feed-Dispencer/tree/2015

#include <Servo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

Servo servo;

int ledState = 0;
int irState = 0;
int photoState = 0;

int billPower = 0;
int servoPos = 45;

int servoState = 0;
int servoWait = 0;

void setup(){
  pinMode(IRPIN, OUTPUT);
  pinMode(PHOTOPIN, OUTPUT);
  pinMode(BILLPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
}

void loop(){
  unsigned long now = millis();
  ledUpdate(now);
  billUpdate(now);
}

int billUpdate(unsigned long now){
  static int state = DISPREADY;
  
  switch(state){
    case DISPREADY:
      if(irState == LOW){
        state = GETTING;
      }
      actionText = "Ready.";
      servoState = 0;
      servoWait = 0;
      return 1;
    case DISPGETTING:
      billPower = 255;
      if(photoState == HIGH) {
        state = DISPENSING;
      }
      actionText = "Bill Detected";
      return 1;
    case DISPDISPENSING:
      if(photoState == HIGH){
        actionText = "Waiting for Bill";
        return 1;
      }else{
        billPower = 0;
        dispense(now);
      }
      return 1;
        
    case DISPDONE:
      state = READY;
      return 1;
  }
}

int ledUpdate(unsigned long now){
  static unsigned long ledMillis = 0UL;
  static int ledState = LOW;

  if(now - ledMillis >= LEDDELAY){
    if(ledState == HIGH){
      ledState = LOW;
    }else if(ledState == LOW) {
      ledState = HIGH;
    }
    digitalWrite(LEDPIN, ledState);
    ledMillis = now;
  }
}

int lcdUpdate(){
  
}
