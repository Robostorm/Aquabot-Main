#include <Keypad.h>

#include <PWMServo.h>

//-*-C++-*-
//https://github.com/Robostorm/Aquabot-Bill-Feed-Dispencer/tree/2015
/* File Aquabot-Main-2015.ino  */
#ifndef AQUABOT
#define AQUABOT

#include <PWMServo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

//#define DEBUGLEDS

//#include "Dispenser.h"
//#include "Lcd.h"
//#include "LedStrip.h"
//#include "LedPattern.h"

//LedStrip topLeds = LedStrip(RED2, GREEN2, BLUE2, "Top Leds");
//LedStrip frontLeds = LedStrip(RED1, GREEN1, BLUE1, "Front Leds");

int bottles = 10;
int bottleSold = 0;
int coolerTemp = 0;

int ledState;
int irState;
int photoState;

int dispState = DISPREADY;
int servoState = 0;
int servoWait;

PWMServo servo;

int motorPower = 0;
int servoPos = SERVOMIN;

char* password = "   ";
int passi = 0;

boolean lcdUnlock = false;

char keys[KEYROW][KEYCOL] = {
  {'L','0','R','E'},
  {'7','8','9','^'},
  {'4','5','6','D'},
  {'1','2','3','U'},
  {'[',']','#','*'}
};
byte rowPins[KEYROW] = {10, 11, 12, 13, 17}; //connect to the row pinouts of the keypad
byte colPins[KEYCOL] = {5, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYROW, KEYCOL);

char key = NOKEY;

void setup(){
  Serial.begin(9600);\
  delay(3000);
  Serial1.begin(9600);
  Serial.println("Setting up");

  servo.attach(SERVOPIN);

  pinMode(IRPIN, INPUT);
  pinMode(PHOTOPIN, INPUT);
  pinMode(BILLPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  delay(1000);
  Serial1.print("?f");
  delay(1000);

  Serial.println("Finnished setting up");
}

void loop(){
  static unsigned long oldNow = 0;
  unsigned long now = millis();
  sensorUpdate(now);
  motorUpdate(now);
  ledUpdate(now);
  dispenserUpdate(now);
  ledStripUpdate(now);
  lcdUpdate(now);
  Serial.println(now-oldNow);
  oldNow = now;
}

int ledUpdate(unsigned long now){
  static unsigned long ledMillis = 0UL;
  static int ledState = LOW;

  if(now - ledMillis >= LEDDELAY){
    if(ledState == HIGH){
      ledState = LOW;
    }else if(ledState == LOW) {
      ledState = HIGH;
    }
    digitalWrite(LEDPIN, ledState);
    ledMillis = now;
  }

  return ledState;
}

void ledStripUpdate(unsigned long now){
  static unsigned long stripMillis = 0UL;
  if(now - stripMillis >= LEDSDELAY){

    static int flashState = 0;
    static int rainbowState = 1;
    static int fadeState = 0;
    static unsigned long billMillis = 0;

    double red2 = 0;
    double green2 = 0;
    double blue2 = 0;

    double red1 = 0;
    double green1 = 0;
    double blue1 = 0;

    static double oldRed1 = 0;
    static double oldGreen1 = 0;
    static double oldBlue1 = 0;

    static double oldRed2 = 0;
    static double oldGreen2 = 0;
    static double oldBlue2 = 0;

    if(dispState == DISPDISPENSING){
      green1 = 255;
      switch(flashState){
        case 0:
          green2 = 255;
          blue2 = 0;
          red2 = 0;
          if(now-billMillis >= BLNKDELAY){
            billMillis = now;
            flashState = 1;
          }
          break;
        case 1:
          green2 = 0;
          blue2 = 255;
          red2 = 0;
          if(now-billMillis >= BLNKDELAY){
            billMillis = now;
            flashState = 0;
          }
          break;
      }
    }else if(dispState == DISPGETTING){
      red1 = 255;
      switch(flashState){
        case 0:
          red2 = 0;
          green2 = 255;
          blue2 = 0;
          if(now-billMillis >= BLNKDELAY){
            billMillis = now;
            flashState = 1;
          }
          break;
        case 1:
          green2 = 0;
          blue2 = 255;
          red2 = 0;
          if(now-billMillis >= BLNKDELAY){
            billMillis = now;
            flashState = 0;
          }
          break;
      }
    }else{
      switch(rainbowState){
        case 1:
          red2 = 255;
          green2 = oldGreen2+RAINBOWSPEED;
          blue2 = 0;
          if(green2 >= 255){
            green2 = 255;
            rainbowState = 2;
          }
          break;
        case 2:
          red2 = oldRed2-RAINBOWSPEED;
          green2 = 255;
          blue2 = 0;
          if(red2 <= 0){
            red2 = 0;
            rainbowState = 3;
          }
          break;
        case 3:
          red2 = 0;
          green2 = 255;
          blue2 = oldBlue2+RAINBOWSPEED;
          if(blue2 >= 255){
            blue2 = 255;
            rainbowState = 4;
          }
          break;
        case 4:
          red2 = 0;
          green2 = oldGreen2-RAINBOWSPEED;
          blue2 = 255;
          if(green2 <= 0){
            green2 = 0;
            rainbowState = 5;
          }
          break;
        case 5:
          red2 = oldRed2+RAINBOWSPEED;
          green2 = 0;
          blue2 = 255;
          if(red2 >= 255){
            red2 = 255;
            rainbowState = 6;
          }
          break;
        case 6:
          red2 = 255;
          green2 = 0;
          blue2 = oldBlue2-RAINBOWSPEED;
          if(blue2 <= 0){
            blue2 = 0;
            rainbowState = 1;
          }
          break;
      }

      switch(fadeState){
        case 0:
          red1 = oldRed1+FADESPEED;
          blue1 = oldBlue1-FADESPEED;
          if(red1 >= 255 || blue1 <= 0){
            red1 = 255;
            blue1 = 0;
            fadeState = 1;
          }
          break;
        case 1:
        red1 = oldRed1-FADESPEED;
        blue1 = oldBlue1+FADESPEED;
        if(red1 <= 0 || blue1 >= 255){
          red1 = 0;
          blue1 = 255;
          fadeState = 0;
        }
          break;
      }

    }

    oldRed1 = red1;
    oldGreen1 = green1;
    oldBlue1 = blue1;

    oldRed2 = red2;
    oldGreen2 = green2;
    oldBlue2 = blue2;

    #ifdef DEBUGLEDS
    Serial.print(red2);
    Serial.print(":");
    Serial.print(green2);
    Serial.print(":");
    Serial.print(blue2);
    Serial.print("|");
    Serial.print(red1);
    Serial.print(":");
    Serial.print(green1);
    Serial.print(":");
    Serial.print(blue1);
    Serial.print("|");
    Serial.print(rainbowState);
    Serial.println();
    #endif

    if(RED1 != 27)
      analogWrite(RED1, red1);
    else
      digitalWrite(RED1, (red1 <= 0 ? LOW : HIGH));

    if(GREEN1 != 27)
      analogWrite(GREEN1, green1);
    else
      digitalWrite(GREEN1, (green1 <= 0 ? LOW : HIGH));

    if(BLUE1 != 27)
      analogWrite(BLUE1, blue1);
    else
      digitalWrite(BLUE1, (blue1 <= 0 ? LOW : HIGH));


    if(RED2 != 27)
      analogWrite(RED2, red2);
    else
      digitalWrite(RED2, (red2 <= 0 ? LOW : HIGH));

    if(GREEN2 != 27)
      analogWrite(GREEN2, green2);
    else
      digitalWrite(GREEN2, (green2 <= 0 ? LOW : HIGH));

    if(BLUE2 != 27)
      analogWrite(BLUE2, blue2);
    else
      digitalWrite(BLUE2, (blue2 <= 0 ? LOW : HIGH));


    stripMillis = now;

  }
}

void dispenserUpdate(unsigned long now){

  switch(dispState){
  case DISPREADY:
    if(irState == LOW){
      if(bottles < 0)
        dispState = GETTING;
    }
    servoState = 0;
    servoWait = 0;
    break;
  case DISPGETTING:
    motorPower = 255;
    if(photoState == HIGH) {
      dispState = DISPENSING;
    }
    break;
  case DISPDISPENSING:
    if(photoState == HIGH){
      break;
    }else{
      motorPower = 0;
      dispense(now);
    }
    break;

  case DISPDONE:
    dispState = READY;
    bottles--;
    bottleSold++;
    break;
  }
}

void sensorUpdate(unsigned long now){
  static unsigned long sensorMillis = 0UL;

  if(now - sensorMillis >= SENSEDELAY){
    irState = digitalRead(IRPIN);
    photoState = digitalRead(PHOTOPIN);
    sensorMillis = now;
  }
}

void motorUpdate(unsigned long now){
  static unsigned long motorMillis = 0UL;

  if(now - motorMillis >= MOTORDELAY){
    analogWrite(BILLPIN, motorPower);
    servo.write(servoPos);
    motorMillis = now;
  }
}

void dispense(unsigned long now){

  static unsigned long servoMillis = 0UL;

  //Serial.println(now-servoMillis);

  if(now - servoMillis >= SERVODELAY){

    if(servoState == 0){
      if(servoPos < SERVOMAX){
        servoPos += SERVOSTEP;
      }else{
        servoState = 1;
      }
    }

    if(servoState == 1){
      if(servoWait < SERVOSTOP/SERVODELAY){
        servoWait++;
      }else{
        servoState = 2;
      }
    }

    if(servoState == 2){
      if(servoPos > SERVOMIN){
        servoPos -= SERVOSTEP;
      }else{
        servoState = 3;
      }
    }

    if(servoState == 3){
      dispState = DONE;
    }
    servoMillis = now;
  }
}

void lcdUpdate(unsigned long now){
  static unsigned long lcdMillis = 0UL;
  static boolean firstDraw = true;

  static int oldBottles = bottles;
  static int oldBottleSold = bottleSold;
  static int oldCoolerTemp = coolerTemp;

  if(now - lcdMillis >= LCDDELAY){
    //Serial.println(now - lcdMillis);

    char nkey = keypad.getKey();
    if (nkey){
      key = nkey;
    }

    if(lcdUnlock){

    }else{

      char line[20];

      if(firstDraw == 1){
        Serial1.print("?x00?y0");
        Serial1.print("      Aquabot");

        sprintf(line, "%s: %i%c", "Bottles Left", bottles, ' ');
        Serial1.print("?x00?y1");
        Serial1.print(line);

        sprintf(line, "%s: %i%c", "Bottles Sold", bottleSold, ' ');
        Serial1.print("?x00?y2");
        Serial1.print(line);

        sprintf(line, "%s: %i%c", "Temperature", coolerTemp, 'F');
        Serial1.print("?x00?y3");
        Serial1.print(line);

        Serial1.print("?x19?y0");
        Serial1.print(key);
        firstDraw = false;
      }

      if(bottles != oldBottles){
        sprintf(line, "%s: %i%c", "Bottles Left", bottles, ' ');
        Serial1.print("?x00?y1");
        Serial1.print(line);
      }

      if(bottleSold != oldBottleSold){
        sprintf(line, "%s: %i%c", "Bottles Sold", bottleSold, ' ');
        Serial1.print("?x00?y2");
        Serial1.print(line);
      }

      if(coolerTemp != oldCoolerTemp){
        sprintf(line, "%s: %i%c", "Temperature", coolerTemp, 'F');
        Serial1.print("?x00?y3");
        Serial1.print(line);
      }

      Serial1.print("?x19?y0");
      Serial1.print(key);
      firstDraw = false;

      switch(key){
        case NOKEY:
          Serial1.print("?x18?y0");
          Serial1.print(' ');
          break;
        case '#':
          passi = 0;
          break;
        default:
          password[passi] = key;
          passi++;
          //Serial.println(password);
          if(passi >= 3){
            passi = 0;
            int res = strcmp(password, PASSWORD);
            if(res == 0){
              Serial1.print("?x18?y0");
              Serial1.print('G');
              //lcdUnlock = true;
            }else{
              Serial1.print("?x18?y0");
              Serial1.print('X');
            }
          }
      }
    }

    lcdMillis = now;

    key = NOKEY;
  }
}

#endif /* !AQUABOT */
