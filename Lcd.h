
/* File Lcd.h  */
#ifndef LCD
#define LCD

#include "Screen.h"

class Lcd{

private:

  static Screen curScreen;
  static Screen loadScreen;
  static DataScreen mainScreen;

public:
  static void init(unsigned long now);
  static void update(unsigned long now);
  static void setScreen(int i);
  static Screen getScreen(int i);

};

#endif /* !LCD */
