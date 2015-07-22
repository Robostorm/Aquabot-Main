
#include "Screen.hpp"

 class Lcd{

private:

  Screen curScreen;
  Screen rootScreen;

public:
  void update();
  void setRoot(Screen screen);
  void setScreen(Screen screen);
  Screen getScreen();

}
