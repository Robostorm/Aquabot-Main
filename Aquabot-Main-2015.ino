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

int btd = 0; // Bottles to dispense

int ledBrightness = 100;

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
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Setting up");

  servo.attach(SERVOPIN);

  pinMode(IRPIN, INPUT);
  pinMode(PHOTOPIN, INPUT);
  pinMode(BILLPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);

  delay(5000);
  Serial1.print("?f");
  Serial1.print("?c0");

  Serial.println("Finnished setting up");
}

void loop(){
  static unsigned long oldNow = 0;
  unsigned long now = millis();
  sensorUpdate(now);
  motorUpdate(now);
  ledUpdate(now);
  moneyUpdate(now);
  dispenserUpdate(now);
  ledStripUpdate(now);
  lcdUpdate(now);
  //Serial.println(now-oldNow);
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

    red1 *= ledBrightness/100.0;
    green1 *= ledBrightness/100.0;
    blue1 *= ledBrightness/100.0;

    red2 *= ledBrightness/100.0;
    green2 *= ledBrightness/100.0;
    blue2 *= ledBrightness/100.0;

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

void moneyUpdate(unsigned long now){

  switch(dispState){
  case DISPREADY:
    if(irState == LOW && bottles > 0){
      //if(bottles == 0)
        dispState = GETTING;
    }
    //servoState = 0;
    //servoWait = 0;
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
      btd++;
      dispState = DISPDONE;
      //int done = dispense(now);
      //if(done == 1){
      //  dispState = DISPDONE;
      //}
    }
    break;

  case DISPDONE:
    dispState = READY;
    bottles--;
    bottleSold++;
    break;
  }
}

void dispenserUpdate(unsigned long now){
  static boolean dispensing  = false;

  //Serial.print(btd);
  //Serial.print(":");
  //Serial.println(dispensing);


  if(dispensing){
    int tmp = dispense(now);
    if(tmp == 1){
      //Serial.print(":");
      //Serial.println("Done");
      dispensing = false;
      btd--;
    }
  }else{
    if(btd > 0){
      //Serial.print(":");
      //Serial.println("Bottles");
      dispensing = true;
      servoState = 0;
      servoWait = 0;
    }
  }

  //Serial.println();
}

void sensorUpdate(unsigned long now){
  static unsigned long sensorMillis = 0UL;

  if(now - sensorMillis >= SENSEDELAY){
    irState = digitalRead(IRPIN);
    photoState = digitalRead(PHOTOPIN);
    coolerTemp = ((double) analogRead(COOLTEMP))*(500.0/1023.0);
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

int dispense(unsigned long now){

  static unsigned long servoMillis = 0UL;

  //Serial.println(now-servoMillis);


  if(now - servoMillis >= SERVODELAY){

    //Serial.print(servoState);
    //Serial.print(":");
    //Serial.print(servoPos);
    //Serial.print(":");
    //Serial.print(servoWait);
    //Serial.print(":");
    //Serial.print(now - servoMillis);
    //Serial.print(":");
    //Serial.println(now);

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
        servoWait = 0;
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
      if(servoWait < SERVOSTOP/SERVODELAY){
        servoWait++;
      }else{
        servoState = 0;
        return 1;
      }
    }

    servoMillis = now;
    return 0;
  }
}

void lcdUpdate(unsigned long now){
  static unsigned long drawMillis = 0UL;
  static unsigned long lcdMillis = 0UL;
  static boolean firstDraw = true;

  static int oldBottles = bottles;
  static int oldBottleSold = bottleSold;
  static int oldCoolerTemp = coolerTemp;

  static int curPos = 1;

  static int screen = MAINMENU;

  boolean redraw = false;

  if(now - drawMillis >= DRAWDELAY){
    redraw = 1;
    //Serial.println("Redrawing");
    drawMillis = now;
  }

  if(now - lcdMillis >= LCDDELAY){
    //Serial.println(now - lcdMillis);

    char nkey = keypad.getKey();
    if (nkey){
      key = nkey;
    }

    //Serial.print(screen);
    //Serial.print(":");
    //Serial.print(firstDraw);
    //Serial.println();

    if(lcdUnlock){
      switch(screen){
        case 0: // Main Menu
          if(firstDraw){

            Serial1.print("?f");

            Serial1.print("?x00?y0");
            Serial1.print("     Main Menu");

            Serial1.print("?x01?y1");
            Serial1.print("Set Bottles");

            Serial1.print("?x01?y2");
            Serial1.print("Dispense Bottle");

            Serial1.print("?x01?y3");
            Serial1.print("Set LEDs Brightness");

            switch(curPos){
              case 1:
                Serial1.print("?x00?y1");
                Serial1.print(">");
                Serial1.print("?x00?y2");
                Serial1.print(' ');
                Serial1.print("?x00?y3");
                Serial1.print(' ');
                break;
              case 2:
                Serial1.print("?x00?y1");
                Serial1.print(' ');
                Serial1.print("?x00?y2");
                Serial1.print(">");
                Serial1.print("?x00?y3");
                Serial1.print(' ');
                break;
              case 3:
                Serial1.print("?x00?y1");
                Serial1.print(' ');
                Serial1.print("?x00?y2");
                Serial1.print(' ');
                Serial1.print("?x00?y3");
                Serial1.print(">");
                break;
            }

            firstDraw = false;
          }

          Serial1.print("?x19?y0");
          Serial1.print(key);
          Serial1.print("?x18?y0");
          Serial1.print(curPos);


          switch(key){
            case NOKEY:
              break;
            case 'U':
              if(curPos > 1){
                curPos--;
              }else{
                curPos = 3;
              }
              redraw = true;
              break;
            case 'D':
              if(curPos < 3){
                curPos++;
              }else{
                curPos = 1;
              }
              redraw = true;
              break;
            case '^':
              firstDraw = true;
              lcdUnlock = false;
              break;
            case 'E':
              screen = curPos;
              break;
          }

          if(redraw){

            switch(curPos){
              case 1:
                Serial1.print("?x00?y1");
                Serial1.print(">");
                Serial1.print("?x00?y2");
                Serial1.print(' ');
                Serial1.print("?x00?y3");
                Serial1.print(' ');
                break;
              case 2:
                Serial1.print("?x00?y1");
                Serial1.print(' ');
                Serial1.print("?x00?y2");
                Serial1.print(">");
                Serial1.print("?x00?y3");
                Serial1.print(' ');
                break;
              case 3:
                Serial1.print("?x00?y1");
                Serial1.print(' ');
                Serial1.print("?x00?y2");
                Serial1.print(' ');
                Serial1.print("?x00?y3");
                Serial1.print(">");
                break;
            }
          }

          break;
        case 1:{ // Set Bottles

          static boolean set = true;
          switch(key){
            case NOKEY:
              break;
            case 'L':
              if(set == false){
                set = true;
                Serial1.print("?f");
              }
              break;
            case 'R':
              if(set == true){
                set = false;
                Serial1.print("?f");
              }
              break;
          }


          if(set){
            //Serial.println("set");
            int tmp = getInt(" Bottles In Cooler", 0, 11);
            switch(tmp){
              case -1:
                break;
              case -2:
                screen = MAINMENU;
                curPos = 1;
                firstDraw = true;
                break;
              default:
                bottles = tmp;
                screen = MAINMENU;
                firstDraw = true;
                break;
            }
          }else{
            //Serial.println("not set");
            int tmp = getInt("    Bottles Sold", 0, 999);
            switch(tmp){
              case -1:
                break;
              case -2:
                screen = MAINMENU;
                curPos = 1;
                firstDraw = true;
                break;
              default:
                bottleSold = tmp;
                screen = MAINMENU;
                firstDraw = true;
                break;
            }
          }
          break;
        }
        case 2: // Dispense Bottle
          btd++;
          screen = MAINMENU;
          break;

        case 3:{ // Set Brightness
          int tmp = getInt(" Brightness of LEDs", 0, 100);
          switch(tmp){
            case -1:
              break;
            case -2:
              screen = MAINMENU;
              curPos = 3;
              firstDraw = true;
              break;
            default:
              ledBrightness = tmp;
              screen = MAINMENU;
              firstDraw = true;
              break;
          }
          break;
        }
      }
    }else{

      //Serial.println(redraw);
      if(firstDraw == 1 || redraw == 1){

        Serial1.print("?f");

        Serial1.print("?x00?y0");
        Serial1.print("      Aquabot");

        char line1[21];
        sprintf(line1, "%s: %i%c", "Bottles Left", bottles, ' ');
        Serial1.print("?x00?y1");
        Serial1.print(line1);

        char line2[21];
        sprintf(line2, "%s: %i%c", "Bottles Sold", bottleSold, ' ');
        Serial1.print("?x00?y2");
        Serial1.print(line2);

        char line3[21];
        sprintf(line3, "%s: %i%s", "Temperature", coolerTemp, "F    ");
        Serial1.print("?x00?y3");
        Serial1.print(line3);

        Serial1.print("?x19?y0");
        Serial1.print(key);
        firstDraw = false;
      }

      if(bottles != oldBottles){
        char line[21];
        sprintf(line, "%s: %i%c", "Bottles Left", bottles, ' ');
        Serial1.print("?x00?y1");
        Serial1.print(line);
      }

      if(bottleSold != oldBottleSold){
        char line[21];
        sprintf(line, "%s: %i%c", "Bottles Sold", bottleSold, ' ');
        Serial1.print("?x00?y2");
        Serial1.print(line);
      }

      if(coolerTemp != oldCoolerTemp){
        char line[21];
        sprintf(line, "%s: %i%c", "Temperature", coolerTemp, 'F');
        Serial1.print("?x00?y3");
        Serial1.print(line);
      }

      Serial1.print("?x19?y0");
      Serial1.print(key);

      switch(key){
        case NOKEY:
          Serial1.print("?x18?y0");
          Serial1.print(' ');
          break;
        case '#':
          passi = 0;
          break;
        default:
          //Serial.println(passi);
          password[passi] = key;
          passi++;
          if(passi >= 3){
            passi = 0;
            int res = strcmp(password, PASSWORD);
            if(res == 0){
              Serial1.print("?x18?y0");
              Serial1.print('Y');
              lcdUnlock = true;
              firstDraw = true;
            }else{
              Serial1.print("?x18?y0");
              Serial1.print('N');
            }
          }
      }
    }

    lcdMillis = now;

    key = NOKEY;
  }
}

int getInt(char* title, int lower, int upper){
  static boolean firstDraw = true;
  static int digit = 0;
  static int digits = snprintf(0,0,"%+d",upper)-1;
  //static int digits = upper > 0 ? (int) log10((double) upper) + 1 : 1;
  static char inBuf[4];

  static char oldTitle[21];

  if(strcmp(title, oldTitle) != 0){
    Serial.println("Changed!");
    strcpy(oldTitle, title);
    firstDraw = true;
  }

  //Serial.println(digits);

  int in;
  int lim = 0;
  boolean redraw = false;

  if(upper > MAXINT){
    upper = MAXINT;
  }

  if(firstDraw){
    digits = snprintf(0,0,"%+d",upper)-1;
    for(int i = 0; i < sizeof(inBuf)-1; i++){
      if(i < digits){
        inBuf[i] = '_';
      }else{
        inBuf[i] = ' ';
      }
    }
  }

  Serial.println(digits);

  switch(key){
    case NOKEY:
      break;
    case ']':
      break;
    case '#':
      break;
    case '*':
      break;
    case 'U':
      break;
    case 'D':
      break;
    case 'L':
      break;
    case 'R':
      break;
    case 'E':
      for(int i = 0; i < digits; i++){
        if(inBuf[i] < '0' || inBuf[i] > '9'){
          inBuf[i] = '0';
        }
      }
      in = atoi(inBuf);
      if(in < lower){
        for(int i = 0; i < digits; i++){
          inBuf[i] = '_';
        }
        redraw = true;
        lim = -1;
        digit = 0;
        break;
      }

      if(in > upper){
        for(int i = 0; i < digits; i++){
          inBuf[i] = '_';
        }
        redraw = true;
        lim = 1;
        digit = 0;
        break;
      }
      firstDraw = true;
      digit = 0;
      return in;

      break;
    case '^':
      firstDraw = true;
      digit = 0;
      return -2;
      break;
    default:
      if(digit < digits){
        for(int i = 0; i < digits-1; i++){
          inBuf[i] = inBuf[i+1];
        }
        inBuf[digits-1] = key;
        digit++;
        redraw = true;
      }
      lim = 0;
  }

  if(firstDraw || redraw){
    Serial1.print("?f");

    Serial1.print("?x00?y0");
    Serial1.print(title);

    Serial1.print("?x06?y1");
    if(lim == -1){
      Serial1.print("Too Low!");
    }else if(lim == 1){
      Serial1.print("Too High!");
    }else{
      char line[21];
      int n = sprintf(line, "%i to %i", lower, upper);

      Serial1.print("?x0");
      Serial1.print(10-n/2);
      Serial1.print(line);

    }

    Serial1.print("?x09?y2");
    Serial1.print(inBuf);
    firstDraw = false;
  }

  return -1;
}

#endif /* !AQUABOT */
