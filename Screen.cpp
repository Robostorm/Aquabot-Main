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
  static int firstRun = 1;

  Serial1.print("?x00?y0");
  Serial1.print(title);

  char line[20];

  if(firstRun == 1){
    for(int i = 0; i < 3; i++){

      sprintf(line, "%s: %i%c", keys[i], *data[i], units[i]);

      Serial1.print("?x00?y");
      Serial1.print(i+1);
      Serial1.print(line);

      oldData[i] = *data[i];
    }
    firstRun = 0;
  }

  for(int i = 0; i < 3; i++){
    if(*data[i] != oldData[i]){

      sprintf(line, "%s: %i%c", keys[i], *data[i], units[i]);

      Serial1.print("?x00?y");
      Serial1.print(i+1);
      Serial1.print(line);

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

void DataScreen::setUnit(int index, char unit){
  units[index] = unit;
}
