//-*-C++-*-
//https://github.com/Robostorm/Aquabot-Bill-Feed-Dispencer/tree/2015
/* File Aquabot-Main-2015.ino  */
#ifndef AQUABOT
#define AQUABOT

#include <Servo.h>

#include "common.h"
#include "pins.h"
#include "conf.h"
#include "states.h"

#include "Dispenser.h"
#include "Lcd.h"

Dispenser disp = Dispenser();
Lcd lcd = Lcd();

void setup(){
  Serial.begin(9800);
  Serial1.begin(9800);
  Serial.println("Setting up");
  disp.init();
  lcd.init(millis());
  Serial.println("Finnished setting up");
}

void loop(){
  //Serial.println("Looping");
  unsigned long now = millis();
  ledUpdate(now);
  disp.update(now);
  lcd.update(now);
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

#endif /* !AQUABOT */
