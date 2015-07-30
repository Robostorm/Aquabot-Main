#include <Keypad.h>

#include <Arduino.h>

#include "Lcd.h"
#include "Screen.h"
#include "Dispenser.h"

#include "conf.h"
#include "pins.h"
#include "states.h"

int Lcd::curScreen;
Screen* Lcd::screens[SCREENS];

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

  DataScreen* mainScreen = new DataScreen(
    "      Aquabot",
    "Bottles Sold", &Dispenser::bottleSold, ' ',
    "Bottles Left", &Dispenser::bottles, ' ',
    "Temperature",&Dispenser::coolerTemp, 'F'
  );

  screens[MAINSCR] = new Screen("Hello");

  curScreen = MAINSCR;

  Serial1.print("?f");
}

void Lcd::update(unsigned long now){

  static unsigned long lcdMillis = 0UL;

  if(now - lcdMillis >= LCDDELAY){

    char nkey = keypad.getKey();

    if (nkey){
      key = nkey;
    }

    Serial.println(curScreen);
    //Screen d = Screen("Aquabot");
    //d.update(now);

    Serial.println("Updating");
    Serial1.print("?x00?y0");
    Serial1.print("title");
    Serial1.print("?x00?y1");
    Serial1.print({"lines[0]"});
    Serial1.print("?x00?y2");
    Serial1.print("lines[1]");
    Serial1.print("?x00?y3");
    Serial1.print("lines[2]");

    Serial1.print("?x19?y0");
    Serial1.print(key);

    lcdMillis = now;

    key = NOKEY;
  }
}

Screen Lcd::getScreen(int i){
  return *screens[curScreen];
}
