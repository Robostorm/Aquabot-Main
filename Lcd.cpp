#include <Arduino.h>

#include "Lcd.h"

void Lcd::init(){
  Serial.println("Initting Lcd");
  Serial1.print("?f");
  loadScreen = Screen();
  loadScreen.setTitle("");
  loadScreen.setLine1("   Loading...   ");
  loadScreen.setLine2("");
  loadScreen.setLine3("");
  loadScreen.update();
}

void Lcd::update(unsigned long now){

}

Screen Lcd::getScreen(){
  return curScreen;
}
