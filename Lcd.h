
/* File Lcd.h  */
#ifndef LCD
#define LCD

#include "Screen.h"

class Lcd{

private:

  Screen curScreen;
  Screen rootScreen;
  Screen loadScreen;

public:
  void init();
  void update(unsigned long now);
  void setScreen(Screen screen);
  Screen getScreen();

};

#endif /* !LCD */
