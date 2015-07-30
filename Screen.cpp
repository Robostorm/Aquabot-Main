#include <Arduino.h>

#include <string.h>

#include "Screen.h"
#include "Lcd.h"

#include "conf.h"

//********************
// Screen
//********************
Screen::Screen(char* t){
  strcpy(title, t);
}

void Screen::init(unsigned long now){

}

void Screen::update(unsigned long now){
  Serial.println("Updating");
  Serial1.print("?x00?y0");
  Serial1.print(title);
  Serial1.print("?x00?y1");
  Serial1.print(lines[0]);
  Serial1.print("?x00?y2");
  Serial1.print(lines[1]);
  Serial1.print("?x00?y3");
  Serial1.print(lines[2]);
}

//********************
// DataScreen
//********************
DataScreen::DataScreen(char* t, char* k1, int* d1, char u1,
  char* k2, int* d2, char u2, char* k3, int* d3, char u3): Screen(t){

  strcpy(keys[0], k1);
  strcpy(keys[1], k2);
  strcpy(keys[2], k3);

  data[0] = d1;
  data[1] = d2;
  data[2] = d3;

  units[0] = u1;
  units[1] = u2;
  units[2] = u3;
}

void DataScreen::init(unsigned long now){

}

void DataScreen::update(unsigned long now){

  for(int i = 0; i < 3; i++){
    sprintf(lines[i], "%s: %i%c", keys[i], *data[i], units[i]);
  }

  Serial1.print("?x00?y0");
  Serial1.print(title);
  Serial1.print("?x00?y1");
  Serial1.print(lines[0]);
  Serial1.print("?x00?y2");
  Serial1.print(lines[1]);
  Serial1.print("?x00?y3");
  Serial1.print(lines[2]);

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
      }else{
        Serial1.print("?x18?y0");
        Serial1.print('X');
      }
    }
  }else{
    Serial1.print("?x18?y0");
    Serial1.print(' ');
  }
}

//********************
// Menu Screen
//********************
MenuScreen::MenuScreen(char* t, Screen* s, int n): Screen(t){
  screens = s;
  size = n;
}

void MenuScreen::init(unsigned long now){

}

void MenuScreen::update(unsigned long now){
  for(int i = 0; i < size; i++){
    char title[20];
    strcpy(title, screens[i].getTitle());
    for(int c = 0; c < 20; c++){

    }
  }
}

/*
int Screen::update(unsigned long now){
  Serial1.print("?x00?y0");conflicting
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

void Screen::setLine3(char* chars){conflicting
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

  char key = Lcd::getKey();Serial.println(next);

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

*/
