
#include "Screen.hpp"

class Lcd{

private:
  
  Screen curScreen;
  
public:
  
  void update();
  void setScreen(Screen screen);
  Screen getScreen();

}
