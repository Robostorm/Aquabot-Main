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

int btd = 0; // Bottles to dispense

int ledBrightness = 100;

char line0[LCDWIDTH+1];
char line1[LCDWIDTH+1];
char line2[LCDWIDTH+1];
char line3[LCDWIDTH+1];

char oldLine0[LCDWIDTH+1];
char oldLine1[LCDWIDTH+1];
char oldLine2[LCDWIDTH+1];
char oldLine3[LCDWIDTH+1];

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

  strcpy(line0, "      Aquabot       ");
  strcpy(line1, "      Test1         ");
  strcpy(line2, "      2Test         ");
  strcpy(line3, "    ThreeTest       ");

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
  keyUpdate(now);
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

void keyUpdate(unsigned long now){
  static unsigned long keyMillis = 0UL;

  static int curPos = 1;

  static int screen = MAINSCRN;

  if(now - keyMillis >= KEYDELAY){
    //Serial.println(now - lcdMillis);

    char nkey = keypad.getKey();
    if (nkey){
      key = nkey;
    }

    //Serial.print(key);
    //Serial.println();

    strcpy(line0, "                    ");
    strcpy(line1, "                    ");
    strcpy(line2, "                    ");
    strcpy(line3, "                    ");

    switch(screen){
      case MAINSCRN:{
        strcpy(line0, "      Aquabot       ");
        sprintf(line1, "%s: %i%c", "Bottles Left", bottles, ' ');
        sprintf(line2, "%s: %i%c", "Bottles Sold", bottleSold, ' ');
        sprintf(line3, "%s: %i%s", "Cooler Temp", coolerTemp, "F   ");

        static int passIndex = 0;
        static char password[PASSLEN];

        switch(key){
          case NOKEY:
            break;
          case PASSSTRT:
            passIndex = 0;
            break;
          default:
            password[passIndex] = key;
            passIndex++;
            if(passIndex >= 3){
              passIndex = 0;
              int res = strcmp(password, PASSWORD);
              if(res == 0){
                line0[18] = PASSACPT;
                screen = MAINMENU;
              }else{
                line0[18] = PASSDENY;
              }
            }
        }

        break;
      }

      case MAINMENU:{

        static int curPos = 1;

        strcpy(line0, "      Main Menu     ");
        strcpy(line1, " Set Bottles        ");
        strcpy(line2, " Dispense Bottle    ");
        strcpy(line3, " Set LED Brightness ");



        switch(key){
          case NOKEY:
            break;
          case 'U':
            if(curPos > 1){
              curPos--;
            }else{
              curPos = 3;
            }
            break;
          case 'D':
            if(curPos < 3){
              curPos++;
            }else{
              curPos = 1;
            }
            break;
          case '^':
            screen = MAINSCRN;
            break;
          case 'E':
            switch(curPos){
              case 1:
                screen = SETBTLS;
                break;
              case 2:
                screen = DISPBTLS;
                break;
              case 3:
                screen = LEDBRGHT;
                break;
            }
            break;
        }

        switch(curPos){
          case 1:
            line1[0] = SELECT;
            break;
          case 2:
            line2[0] = SELECT;
            break;
          case 3:
            line3[0] = SELECT;
            break;
        }
        break;
      }

      case SETBTLS:{
        static boolean set = true;
        switch(key){
          case NOKEY:
            break;
          case 'L':
            if(set == false){
              set = true;
            }
            break;
          case 'R':
            if(set == true){
              set = false;
            }
            break;
        }


        if(set){
          //Serial.println("set");
          int tmp = getInt(" Bottles In Cooler  ", 0, MAXBTLS);
          strcpy(line3, "     Bottles Sold ->");

          switch(tmp){
            case -1:
              break;
            case -2:
              screen = MAINMENU;
              break;
            default:
              bottles = tmp;
              screen = MAINMENU;
              break;
          }
        }else{
          //Serial.println("not set");
          int tmp = getInt("    Bottles Sold    ", 0, MAXINT);
          strcpy(line3, "<- Bottles in Cooler");
          switch(tmp){
            case -1:
              break;
            case -2:
              screen = MAINMENU;
              break;
            default:
              bottleSold = tmp;
              screen = MAINMENU;
              break;
          }
        }
        break;
      }

      case DISPBTLS:{ // Dispense Bottle
        strcpy(line1, "   Dispensing...    ");
        btd++;
        screen = MAINMENU;
        break;
      }

      case LEDBRGHT:{ // Set Brightness
        int tmp = getInt(" Brightness of LEDs", 0, 100);
        switch(tmp){
          case -1:
            break;
          case -2:
            screen = MAINMENU;
            break;
          default:
            ledBrightness = tmp;
            screen = MAINMENU;
            break;
        }
        break;
      }
    }

    line0[19] = key;

    keyMillis = now;

    key = NOKEY;
  }
}

void lcdUpdate(unsigned long now){
  static unsigned long drawMillis = 0UL;

  if(now - drawMillis >= DRAWDELAY){
    Serial1.print("?x00");
    Serial1.print("?y0");
    Serial1.print(line0);
    Serial1.print("?x00");
    Serial1.print("?y1");
    Serial1.print(line1);
    Serial1.print("?x00");
    Serial1.print("?y2");
    Serial1.print(line2);
    Serial1.print("?x00");
    Serial1.print("?y3");
    Serial1.print(line3);

    strcpy(oldLine0, line0);
    strcpy(oldLine1, line1);
    strcpy(oldLine2, line2);
    strcpy(oldLine3, line3);

    drawMillis = now;
  }


  static unsigned long lcdMillis = 0UL;

  if(now - lcdMillis >= LCDDELAY){

    Serial1.print("?y0");
    for(int i = 0; i <= LCDWIDTH; i++){
      if(line0[i] != oldLine0[i]){
        char x[3];
        sprintf(x, "%02i", i);
        Serial1.print("?x");
        Serial1.print(x);
        Serial1.print(line0[i]);
        oldLine0[i] = line0[i];
      }
    }

    Serial1.print("?y1");
    for(int i = 0; i <= LCDWIDTH; i++){
      if(line1[i] != oldLine1[i]){
        char x[3];
        sprintf(x, "%02i", i);
        Serial1.print("?x");
        Serial1.print(x);
        Serial1.print(line1[i]);
        oldLine1[i] = line1[i];
      }
    }

    Serial1.print("?y2");
    for(int i = 0; i <= LCDWIDTH; i++){
      if(line2[i] != oldLine2[i]){
        char x[3];
        sprintf(x, "%02i", i);
        Serial1.print("?x");
        Serial1.print(x);
        Serial1.print(line2[i]);
        oldLine2[i] = line2[i];
      }
    }

    Serial1.print("?y3");
    for(int i = 0; i <= LCDWIDTH; i++){
      if(line3[i] != oldLine3[i]){
        char x[3];
        sprintf(x, "%02i", i);
        Serial1.print("?x");
        Serial1.print(x);
        Serial1.print(line3[i]);
        oldLine3[i] = line3[i];
      }
    }
    lcdMillis = now;
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
    firstDraw = false;
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
        lim = -1;
        digit = 0;
        break;
      }

      if(in > upper){
        for(int i = 0; i < digits; i++){
          inBuf[i] = '_';
        }
        lim = 1;
        digit = 0;
        break;
      }
      digit = 0;
      firstDraw = true;
      return in;

      break;
    case '^':
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
      }
      lim = 0;
  }

  strcpy(line0, title);

  if(lim == -1){
    strcpy(line1, "      Too Low!      ");
  }else if(lim == 1){
    strcpy(line1, "      Too High!     ");
  }else{
    char line[LCDWIDTH+1];
    int n = sprintf(line, "%i to %i", lower, upper);
    int l = LCDWIDTH/2-n/2;

    for(int i = 0; i < n; i++){
      line1[i+l] = line[i];
    }
  }

  int l = LCDWIDTH/2-digits/2;
  for(int i = 0; i < digits; i++){
    line2[i+l] = inBuf[i];
  }

  strcpy(line3, "                    ");


  return -1;
}

#endif /* !AQUABOT */
