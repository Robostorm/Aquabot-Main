#include <Arduino.h>

#include "Screen.h"

void Screen::update(){
  Serial1.print("?x00?y0");
  Serial1.print(title);
  Serial1.print("?x00?y1");
  Serial1.print(line1);
  Serial1.print("?x00?y2");
  Serial1.print(line2);
  Serial1.print("?x00?y3");
  Serial1.print(line3);

  Serial.println(title);
  Serial.println(line1);
  Serial.println(line2);
  Serial.println(line3);
}

void Screen::setTitle(char* chars){
  title = chars;
}

void Screen::setLine1(char* chars){
  line1 = chars;
}

void Screen::setLine2(char* chars){
  line2 = chars;
}

void Screen::setLine3(char* chars){
  line3 = chars;
}
