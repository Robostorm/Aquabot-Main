
/* File LedStrip.h  */
#ifndef LEDSTRIP
#define LEDSTRIP

#include "LedPattern.h"
#include "pins.h"
#include "conf.h"
#include "states.h"

class LedStrip{
private:

  int patternIndex = 0;
  LedPattern* ledPatterns[LEDPATTERNS];

  int redPin;
  int greenPin;
  int bluePin;

  char* ledName;

  unsigned long ledsMillis = 0UL;

public:
  LedStrip(int redP, int greenP, int blueP, char* name);
  void init();
  void update(unsigned long now);
  void setPattern(LedPattern* pattern, int index);
};

#endif /* !LEDSTRIP */
