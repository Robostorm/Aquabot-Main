#include <Arduino.h>

#include <string.h>

#include "Screen.h"
#include "Lcd.h"

#include "conf.h"

int Screen::update(unsigned long now){
  Serial1.print("?x00?y0");
  Serial1.print(title);
  Serial1.print("?x00?y1");
  Serial1.print(line1);
  Serial1.print("?x00?y2");
  Serial1.print(line2);
  Serial1.print("?x00?y3");
  Serial1.print(line3);
  return 0;
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

int DataScreen::update(unsigned long now){

  Serial.println(next);

  if(next == true){
    if(mainScreen.update(now) == 1) next = false;
    return 0;
  }

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

  char key = Lcd::getKey();

  if(key != ' '){
    password[passi] = key;
    passi++;
    Serial.println(password);
    if(passi >= 4){
      passi = 0;
      Serial.print("Password Accepted: ");
      Serial.println(password);
      if(strcmp(password, PASSWORD) == 0){
        Serial1.print("?x18?y0");
        Serial1.print('G');
        next = true;
      }else{
        Serial1.print("?x18?y0");
        Serial1.print('X');
      }
    }
  }else{
    Serial1.print("?x18?y0");
    Serial1.print(' ');
  }

  return 0;
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

int MenuScreen::update(unsigned long now){
  Serial1.print("?x00?y0");
  Serial1.print(title);

  char key = Lcd::getKey();

  switch(key){
    case ' ':
      break;
    case 'U':
      index++;
      break;
    case 'D':
      index--;
      break;
    case '^':
      return 1;
  }

  for(int i = 0; i < 3; i++){
    Serial1.print("?x01?y");
    Serial1.print(i+1);
    Serial1.print(lines[i]);
    if(i == index){
      Serial1.print("?x00?y");
      Serial1.print(i+1);
      Serial1.print(SELECT);
    }
  }

  return 0;
}
