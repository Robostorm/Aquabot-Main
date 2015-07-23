
/* File Screen.h  */
#ifndef SCREEN
#define SCREEN

class Screen{

private:
  char* line1;
  char* line2;
  char* line3;
  char* title;

  void draw();

public:
  void update();
  void setLine1(char* chars);
  void setLine2(char* chars);
  void setLine3(char* chars);
  void setTitle(char* chars);
};

#endif /* !SCREEN */
