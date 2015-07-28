#include <Keypad.h>

#include <Arduino.h>

#include "Lcd.h"
#include "Screen.h"
#include "Dispenser.h"

#include "conf.h"
#include "pins.h"
#include "states.h"

Screen Lcd::curScreen = Screen();
Screen Lcd::loadScreen = Screen();
DataScreen Lcd::mainScreen = DataScreen();
MenuScreen Lcd::menuScreen = MenuScreen();

char Lcd::keys[KEYROW][KEYCOL] = {
  {'L','0','R','E'},
  {'7','8','9','^'},
  {'4','5','6','D'},
  {'1','2','3','U'},
  {'[',']','#','*'}
};
byte Lcd::rowPins[KEYROW] = {10, 11, 12, 13, 17}; //connect to the row pinouts of the keypad
byte Lcd::colPins[KEYCOL] = {5, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad Lcd::keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYROW, KEYCOL);

char Lcd::key = NOKEY;

void Lcd::init(unsigned long now){
  Serial.println("Initting Lcd");
  Serial1.print("?f");
  loadScreen = Screen();
  loadScreen.setTitle("                    ");
  loadScreen.setLine1("      Aquabot       ");
  loadScreen.setLine2("      Loading...    ");
  loadScreen.setLine3("====================");
  loadScreen.update(now);

  menuScreen = MenuScreen();
  menuScreen.setScreen1("Hello", Screen());
  menuScreen.setScreen1("ssss", Screen());
  menuScreen.setScreen1("Hedddllo", Screen());

  mainScreen = DataScreen();
  mainScreen.setTitle("      Aquabot");
  mainScreen.setKey(0, "Bottles Sold");
  mainScreen.setData(0, &Dispenser::bottleSold);
  mainScreen.setKey(1, "Bottles Left");
  mainScreen.setData(1, &Dispenser::bottles);
  mainScreen.setKey(2, "Temperature");
  mainScreen.setData(2, &Dispenser::coolerTemp);
  mainScreen.setUnit(2, 'F');
  mainScreen.setMainScreen(menuScreen);


  curScreen = mainScreen;

  Serial1.print("?f");
}

void Lcd::update(unsigned long now){

  static unsigned long lcdMillis = 0UL;

  if(now - lcdMillis >= LCDDELAY){

    char nkey = keypad.getKey();

    if (nkey){
      key = nkey;
    }

    mainScreen.update(now);

    Serial1.print("?x19?y0");
    Serial1.print(key);

    lcdMillis = now;

    key = NOKEY;
  }
}

Screen Lcd::getScreen(int i){
  return curScreen;
}
