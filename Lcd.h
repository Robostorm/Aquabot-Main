#include <Keypad.h>


/* File Lcd.h  */
#ifndef LCD
#define LCD

#include "Screen.h"

#include "conf.h"

class Lcd{

private:

  static Screen curScreen;
  static Screen loadScreen;
  static DataScreen mainScreen;
  static MenuScreen menuScreen;

  static char keys[KEYROW][KEYCOL];
  static byte rowPins[KEYROW]; //connect to the row pinouts of the keypad
  static byte colPins[KEYCOL]; //connect to the column pinouts of the keypad

  static Keypad keypad;

  static char key;

public:
  static void init(unsigned long now);
  static void update(unsigned long now);
  static void setScreen(int i);
  static Screen getScreen(int i);
  static char getKey(){return key;}
  static void keypadEvent(KeypadEvent key);
};

#endif /* !LCD */
