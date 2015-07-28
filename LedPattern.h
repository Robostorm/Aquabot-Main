
/* File LedPattern.h  */
#ifndef LEDPATTERN
#define LEDPATTERN

#include <Arduino.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

class LedPattern{
public:

  virtual boolean isReady();
  virtual void update(int &red, int &blue, int &green);
  virtual char* getName();

};

class Blue: public LedPattern{
public:
  boolean isReady(){return true;}
  void update(int &red, int &blue, int &green){blue = 255;}
  char* getName(){return "Blue";}


};

class RedFade: public LedPattern{
private:
  int oldRed = 0;

public:
  boolean isReady();
  void update(int &red, int &blue, int &green);
  char* getName();

};

class BillFlash: public LedPattern{
private:
  int state = 0;
  unsigned long billMillis = 0;
public:
  boolean isReady();
  void update(int &red, int &blue, int &green);
  char* getName();

};

class BillBlue: public LedPattern{
public:
  boolean isReady();
  void update(int &red, int &blue, int &green);
  char* getName();

};

class DispGreen: public LedPattern{
public:
  boolean isReady();
  void update(int &red, int &blue, int &green);
  char* getName();

};

class FadeBG: public LedPattern{
private:
  int old;
  int state;
public:
  boolean isReady();
  void update(int &red, int &blue, int &green);
  char* getName();
};


#endif /* !LEDPATTERN */
