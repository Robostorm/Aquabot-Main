//-*-C++-*-
//https://github.com/Robostorm/Aquabot-Bill-Feed-Dispencer/tree/2015

#include <Servo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

#include "Dispenser.h"
#include "Lcd.h"

Dispenser disp = Dispenser();
Lcd lcd = Lcd();

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
  disp.update(now);
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

  return ledState;
}
