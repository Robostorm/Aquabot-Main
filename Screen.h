
/* File Screen.h  */
#ifndef SCREEN
#define SCREEN

class Screen{

protected:
  char* line1;
  char* line2;
  char* line3;
  char* title;

  boolean next = false;

public:
  virtual int update(unsigned long now);
  virtual void setLine1(char* chars);
  virtual void setLine2(char* chars);
  virtual void setLine3(char* chars);
  void setTitle(char* chars);
  void setActive(){next = false;};
};

class DataScreen: public Screen{

protected:
  char* keys[3] = {"", "", ""};
  int* data[3] = {0, 0, 0};
  char units[3] = {' ', ' ', ' '};
  int oldData[3] = {-1, -1, -1};
  char* password = "    ";
  int passi = 0;
  Screen mainScreen;

public:
  int update(unsigned long now);
  void setKey(int index, char* key);
  void setData(int index, int* dat);
  void setUnit(int index, char unit);
  void setMainScreen(Screen s){mainScreen = s;};
};

class MenuScreen: public Screen{

protected:
  int index = 0;

  char* lines[3];

  Screen screens[3];

public:
  int update(unsigned long now);
  void setScreen1(char* l, Screen s){lines[0] = l, screens[0] = s;};
  void setScreen2(char* l, Screen s){lines[1] = l, screens[1] = s;};
  void setScreen3(char* l, Screen s){lines[2] = l, screens[2] = s;};
};

#endif /* !SCREEN */
