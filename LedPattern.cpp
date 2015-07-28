
#include <Arduino.h>

#include "LedStrip.h"
#include "LedPattern.h"
#include "Dispenser.h"

#include "conf.h"
#include "pins.h"
#include "states.h"

//********************
// Led Pattern
//********************
boolean LedPattern::isReady(){
  return true;
}

void LedPattern::update(int &red, int &blue, int &green){

  red = 255;
  blue = 255;
  green = 255;

}

char* LedPattern::getName(){
  return "LedPattern";
}

//********************
// Red Fade
//********************
boolean RedFade::isReady(){
  return true;
}

void RedFade::update(int &red, int &blue, int &green){

  if(oldRed <= 255){
    red = oldRed;
    oldRed++;
  }else{
    oldRed = 0;
  }

}

char* RedFade::getName(){
  return "RedFade";
}

//********************
// Bill Flash
//********************
boolean BillFlash::isReady(){
  return (Dispenser::getState() == DISPGETTING) ||
  (Dispenser::getState() == DISPDISPENSING) || (state != 0);
}

void BillFlash::update(int &red, int &blue, int &green){

  Serial.print(state);

  unsigned long now = millis();

  //Serial.print(now);
  //Serial.print(":");
  //Serial.print(billMillis);
  //Serial.print(":");
  //Serial.println(now-billMillis);

  switch(state){
    case 0:
      green = 255;
      blue = 0;
      red = 0;
      if(now-billMillis >= BLNKDELAY){
        billMillis = now;
        state = 1;
      }
      break;
    case 1:
      green = 0;
      blue = 255;
      red = 0;
      if(now-billMillis >= BLNKDELAY){
        billMillis = now;
        state = 0;
      }
      break;


  }

}

char* BillFlash::getName(){
  return "BillFlash";
}

//********************
// FadeBG
//********************
boolean FadeBG::isReady(){
  return true;
}

void FadeBG::update(int &red, int &blue, int &green){


  switch(state){
    case 0:
      if(old < 254){
        old+=2;
      }else{
        state = 1;
      }
      break;
    case 1:
      if(old > 0){
        old-=2;
      }else{
        state = 0;
      }
      break;
  }

  blue = old;
  green = 255 - old;

  //Serial.print(state);
  //Serial.print(":");
  //Serial.print(blue);
  //Serial.print(":");
  //Serial.println(green);

}

char* FadeBG::getName(){
  return "FadeBG";
}

//********************
// Bill Blue
//********************
boolean BillBlue::isReady(){
  return Dispenser::getState() == DISPGETTING;
}

void BillBlue::update(int &red, int &blue, int &green){
  blue = 255;
  red = 0;
  green  = 0;
}

char* BillBlue::getName(){
  return "BillBlue";
}

//********************
// Dispenser Green
//********************
boolean DispGreen::isReady(){
  return Dispenser::getState() == DISPDISPENSING;
}

void DispGreen::update(int &red, int &blue, int &green){
  green = 255;
  red = 0;
  blue = 0;
}

char* DispGreen::getName(){
  return "Dispenser Green";
}
