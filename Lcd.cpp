#include <Arduino.h>

#include "Lcd.h"
#include "Screen.h"
#include "Dispenser.h"
#include "common.h"
#include "conf.h"
#include "pins.h"
#include "states.h"

Screen Lcd::curScreen = Screen();
Screen Lcd::loadScreen = Screen();
DataScreen Lcd::mainScreen = DataScreen();

void Lcd::init(unsigned long now){
  Serial.println("Initting Lcd");
  Serial1.print("?f");
  loadScreen = Screen();
  loadScreen.setTitle("                    ");
  loadScreen.setLine1("      Aquabot       ");
  loadScreen.setLine2("      Loading...    ");
  loadScreen.setLine3("====================");
  loadScreen.update(now);

  mainScreen = DataScreen();
  mainScreen.setTitle("      Aquabot       ");
  mainScreen.setKey(0, "Bottles Sold");
  mainScreen.setData(0, &Dispenser::bottleSold);
  mainScreen.setKey(1, "Bottles Left");
  mainScreen.setData(1, &Dispenser::bottles);
  mainScreen.setKey(2, "Temperature");
  mainScreen.setData(2, &Dispenser::coolerTemp);
  mainScreen.setUnit(2, 'F');

  curScreen = mainScreen;

  Serial1.print("?f");
}

void Lcd::update(unsigned long now){
  static unsigned long lcdMillis = 0UL;

  if(now - lcdMillis >= LCDDELAY){
    mainScreen.update(now);
    lcdMillis = now;
  }
}

Screen Lcd::getScreen(int i){
  return curScreen;
}
