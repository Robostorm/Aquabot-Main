#include <Arduino.h>

#include "Screen.h"

#include "conf.h"

void Screen::update(unsigned long now){
  Serial1.print("?x00?y0");
  Serial1.print(title);
  Serial1.print("?x00?y1");
  Serial1.print(line1);
  Serial1.print("?x00?y2");
  Serial1.print(line2);
  Serial1.print("?x00?y3");
  Serial1.print(line3);
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

void DataScreen::update(unsigned long now){
  Serial1.print("?x00?y0");
  Serial1.print(title);
  for(int i = 0; i < 3; i++){
    if(*data[i] != oldData[i]){
      Serial1.print("?x00?y");
      Serial1.print(i+1);
      Serial1.print(keys[i]);
      Serial1.print("?x17?y");
      Serial1.print(i+1);
      Serial1.print("   ");
      Serial1.print("?x17?y");
      Serial1.print(i+1);
      Serial1.print(*data[i]);
      oldData[i] = *data[i];
    }
  }
}

void DataScreen::setKey(int index, char* key){
  keys[index] = key;
}

void DataScreen::setData(int index, int* dat){
  data[index] = dat;
}
