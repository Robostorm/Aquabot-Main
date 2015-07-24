
/* File Screen.h  */
#ifndef SCREEN
#define SCREEN

class Screen{

protected:
  char* line1;
  char* line2;
  char* line3;
  char* title;

public:
  void update(unsigned long now);
  void setLine1(char* chars);
  void setLine2(char* chars);
  void setLine3(char* chars);
  void setTitle(char* chars);
};

class DataScreen: public Screen{

protected:
  char* keys[3] = {"", "", ""};
  int* data[3] = {0, 0, 0};
  char units[3] = {' ', ' ', ' '};
  int oldData[3] = {-1, -1, -1};

public:
  void update(unsigned long now);
  void setKey(int index, char* key);
  void setData(int index, int* dat);
  void setUnit(int index, char unit);
};

#endif /* !SCREEN */
