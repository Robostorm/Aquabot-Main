#include <Keypad.h>

#include <PWMServo.h>

//-*-C++-*-
//https://github.com/Robostorm/Aquabot-Bill-Feed-Dispencer/tree/2015
/* File Aquabot-Main-2015.ino  */
#ifndef AQUABOT
#define AQUABOT

#include <PWMServo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

#include "Dispenser.h"
#include "Lcd.h"
#include "LedStrip.h"
#include "LedPattern.h"

LedStrip topLeds = LedStrip(RED2, GREEN2, BLUE2, "Top Leds");
LedStrip frontLeds = LedStrip(RED1, GREEN1, BLUE1, "Front Leds");

void setup(){
  Serial.begin(9800);
  Serial1.begin(9800);
  Serial.println("Setting up");
  Dispenser::init();
  Lcd::init(millis());

  topLeds.init();
  topLeds.setPattern(new BillFlash(), 2);
  topLeds.setPattern(new Blue(), 1);
  topLeds.setPattern(new Blue(), 0);

  frontLeds.init();
  frontLeds.setPattern(new BillBlue(), 2);
  frontLeds.setPattern(new DispGreen(), 1);
  frontLeds.setPattern(new FadeBG(), 0);

  Serial.println("Finnished setting up");
}

void loop(){
  //Serial.println("Looping");
  unsigned long now = millis();
  ledUpdate(now);
  Dispenser::update(now);
  Lcd::update(now);

  topLeds.update(now);
  frontLeds.update(now);
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
