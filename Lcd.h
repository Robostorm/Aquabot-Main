
/* File Lcd.h  */
#ifndef LCD
#define LCD

#include "Screen.h"

class Lcd{

private:

  Screen curScreen;
  Screen rootScreen;

public:
  void update();
  void setRoot(Screen screen);
  void setScreen(Screen screen);
  Screen getScreen();

};

#endif /* !LCD */
