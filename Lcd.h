
/* File Lcd.h  */
#ifndef LCD
#define LCD

#include "Screen.h"

class Lcd{

private:

  Screen curScreen;

  Screen loadScreen;
  DataScreen mainScreen;

public:
  void init(unsigned long now);
  void update(unsigned long now);
  void setScreen(int i);
  Screen getScreen(int i);

};

#endif /* !LCD */
