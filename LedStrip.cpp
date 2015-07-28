#include <Arduino.h>

#include "LedStrip.h"
#include "conf.h"
#include "pins.h"
#include "states.h"

LedStrip::LedStrip(int redP, int greenP, int blueP, char* name):
    redPin(redP), greenPin(greenP), bluePin(blueP), ledName(name){



}

void LedStrip::init(){

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.print("Inited ");
  Serial.println(ledName);

}

void LedStrip::update(unsigned long now){

  //Serial.print("Updating ");
  //Serial.print(ledName);

  if(now - ledsMillis >= LEDSDELAY){

    int redl = 0;
    int greenl = 0;
    int bluel = 0;

    for(patternIndex = 0; patternIndex < LEDPATTERNS; patternIndex++){
      if(ledPatterns[patternIndex]->isReady()){
        ledPatterns[patternIndex]->update(redl, bluel, greenl);
      }
    }

    constrain(redl, 0, 255);
    constrain(greenl, 0, 255);
    constrain(bluel, 0, 255);

    // Pin 27 cannot be PWMed, see README.md
    if(redPin != 27)
      analogWrite(redPin, redl);
    else
      digitalWrite(redPin, (redl <= 0 ? LOW : HIGH));

    if(greenPin != 27)
      analogWrite(greenPin, greenl);
    else
      digitalWrite(greenPin, (greenl <= 0 ? LOW : HIGH));

    if(bluePin != 27)
      analogWrite(bluePin, bluel);
    else
      digitalWrite(bluePin, (bluel <= 0 ? LOW : HIGH));

    ledsMillis = now;
  }
}

void LedStrip::setPattern(LedPattern* pattern, int index){
  if(index >= 0 && index < LEDPATTERNS){
    ledPatterns[index] = pattern;
  }
}
