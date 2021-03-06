#include <Keypad.h>
#include <EEPROM.h>
#include <PWMServo.h>
#include <SoftwareSerial.h>

//-*-C++-*-
//https://github.com/Robostorm/Aquabot-Bill-Feed-Dispencer/tree/2015
/* File Aquabot-Main-2015.ino  */

#ifndef AQUABOT
#define AQUABOT

#include <PWMServo.h>

#include "pins.h"
#include "conf.h"
#include "states.h"

#define DEBUGLEDS

byte bottles = MAXBTLS;
short bottleSold = 0;
int coolerTemp = 0;

SoftwareSerial bluetoothSerial(BLUETOOTHRX, BLUETOOTHTX);

int ledState;
int irState;
int photoState;

int moneyState = DISPREADY;
int servoState = 0;
int servoWait;

boolean dispensing = false;

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
  bluetoothSerial.begin(9600);
  Serial.println("Setting up");

  servo.attach(SERVOPIN);

  pinMode(IRPIN, INPUT);
  pinMode(PHOTOPIN, INPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(POT1, INPUT);
  pinMode(BILLPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);

  EEPROM.get(BTLS_EEPROM_ADR, bottleSold);

  Serial.print("Bottles: ");
  Serial.println(bottleSold);

  #ifdef SETUP_LCD
  // Disable custom boot screen
  Serial1.print("?S0");
  // Set LCD size to 4x20 chars
  Serial1.print("?G420");
  // Set backlight to full brightness
  Serial1.print("?BFF");
  delay(100);
  // Disable cursor
  Serial1.print("?c0");
  delay(100);
  #endif // SETUP_LCD

  // Clear screen
  Serial1.print("?f");

  Serial.println("Finnished setting up");
}

void loop(){
  static unsigned long oldNow = 0;
  unsigned long now = millis();
  //bluetoothUpdate(now);
  buttonUpdate(now);
  sensorUpdate(now);
  motorUpdate(now);
  ledUpdate(now);
  moneyUpdate(now);
  dispenserUpdate(now);
  ledStripUpdate(now);
  keyUpdate(now);
  lcdUpdate(now);
  oldNow = now;
}

int bluetoothUpdate(unsigned long now){
    static unsigned long bluetoothMillis = 0UL;

    if(now - bluetoothMillis >= BLUETOOTHDELAY) {
      
      int state = 0;
      Serial.println(bluetoothSerial.available(), DEC);
      if(bluetoothSerial.available() >= 4) {
        state = bluetoothSerial.read();
        state = bluetoothSerial.read()<<8;
        state = bluetoothSerial.read()<<16;
        state = bluetoothSerial.read()<<24;
        Serial.println(state, DEC);
        if(state >= 0&& state <= 255){
          analogWrite(RED1, state);
          analogWrite(RED2, state);
        }
        if(state >= 256&& state <= 511){
          analogWrite(BLUE1, state-256);
          analogWrite(BLUE2, state-256);
        }
        if(state >= 512&& state <= 767){
          analogWrite(GREEN1, state-512);
          analogWrite(GREEN2, state-512);
        }
        if(state = 768){
          //btd += 1;
        }
      }

      bluetoothMillis = now;
    }
}

int buttonUpdate(unsigned long now){
    static unsigned long buttonMillis = 0UL;

    static int last_button1_state = HIGH;
    static int last_button2_state = HIGH;

    static unsigned long last_button1_millis;
    static unsigned long last_button2_millis;

    static int button1_long_pressed = false;
    static int button2_long_pressed = false;

    if(now - buttonMillis >= BUTTONDELAY) {
        int button1 = digitalRead(BUTTON1);

        // start the timer when the button is depressed
        if(button1 == LOW && last_button1_state == HIGH) {
            last_button1_millis = now;
            button1_long_pressed = false;
        }

        // check if the button has been held long enough for a long press
        if(button1 == LOW && now - last_button1_millis >= LONGBUTTONDELAY) {
            if(button1_long_pressed == false) {
                button1_long_pressed = true;
                bottleSold = 0;
            }
        }

        // check if the button was released before a long press
        // this is a short press
        if(button1 == HIGH && last_button1_state == LOW && button1_long_pressed == false) {
            bottles = MAXBTLS;
        }

        last_button1_state = button1;

        int button2 = digitalRead(BUTTON2);

        // start the timer when the button is depressed
        if(button2 == LOW && last_button2_state == HIGH) {
            last_button2_millis = now;
            button2_long_pressed = false;
        }

        // check if the button has been held long enough for a long press
        if(button2 == LOW && now - last_button2_millis >= SHORTBUTTONDELAY) {
            if(button2_long_pressed == false) {
                button2_long_pressed = true;
                btd++;
            }
        }

        // check if the button was released before a long press
        // this is a short press
        if(button2 == HIGH && last_button2_state == LOW && button2_long_pressed == false) {
            // do nothing right now
        }

        last_button2_state = button2;

        //int pot = analogRead(POT1);
        //ledBrightness = pot * 100.0/1023.0;

        buttonMillis = now;
    }

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

/*
          |   Idle   |  Getting |  Dispensing |  Done    | No Bottles
   -------|----------|----------|-------------|----------|------------
    Main  | Fade RGB | Flash BG |   Flash BG  |  Blue    |    Red
    Front | Fade BG  |   Blue   |   Flash BG  | Flash BG |    Red
*/

void ledStripUpdate(unsigned long now){
  static unsigned long stripMillis = 0UL;
  if(now - stripMillis >= LEDSDELAY){

    static int flashState = 0;
    static int rainbowState = 1;
    static int fadeState = 0;
    static unsigned long billMillis = 0;
    static unsigned long doneMillis = 0;

    // Main LEDs
    double red2 = 0;
    double green2 = 0;
    double blue2 = 0;

    // Front LEDs
    double red1 = 0;
    double green1 = 0;
    double blue1 = 0;

    static double oldRed1 = 0;
    static double oldGreen1 = 0;
    static double oldBlue1 = 0;

    static double oldRed2 = 0;
    static double oldGreen2 = 0;
    static double oldBlue2 = 0;

    switch (moneyState) {
      case DISPGETTING:
        // Flash main blue/green
        // Front blue
        red1 = 0;
        green1 = 0;
        blue1 = 255;
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
            red2 = 0;
            green2 = 0;
            blue2 = 255;
            if(now-billMillis >= BLNKDELAY){
              billMillis = now;
              flashState = 0;
            }
            break;
        }
        break;
      case DISPDISPENSING:
        // Flash both LEDs blue/green
        switch(flashState){
          case 0:
            red2 = 0;
            green2 = 255;
            blue2 = 0;
            red1 = 0;
            green1 = 255;
            blue1 = 0;
            if(now-billMillis >= BLNKDELAY){
              billMillis = now;
              flashState = 1;
            }
            break;
          case 1:
            red2 = 0;
            green2 = 0;
            blue2 = 255;
            red1 = 0;
            green1 = 0;
            blue1 = 255;
            if(now-billMillis >= BLNKDELAY){
              billMillis = now;
              flashState = 0;
            }
            break;
        }
        doneMillis = now;
        break;
      default:
        if (now - doneMillis < DONEFLASH) {
          // Flash front blue/green
          // Main blue
          red2 = 0;
          green2 = 0;
          blue2 = 255;
          switch(flashState){
            case 0:
              red1 = 0;
              green1 = 255;
              blue1 = 0;
              if(now-billMillis >= BLNKDELAY){
                billMillis = now;
                flashState = 1;
              }
              break;
            case 1:
              red1 = 0;
              green1 = 0;
              blue1 = 255;
              if(now-billMillis >= BLNKDELAY){
                billMillis = now;
                flashState = 0;
              }
              break;
          }
        } else if (bottles <= 0) {
          // All red
          red2 = 255;
          green2 = 0;
          blue2 = 0;
          red1 = 255;
          green1 = 0;
          blue1 = 0;
        } else {
          // RAINBOWS!!!
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
              green1 = oldGreen1+FADESPEED;
              blue1 = oldBlue1-FADESPEED;
              if(green1 >= 255 || blue1 <= 0){
                green1 = 255;
                blue1 = 0;
                fadeState = 1;
              }
              break;
            case 1:
            green1 = oldGreen1-FADESPEED;
            blue1 = oldBlue1+FADESPEED;
            if(green1 <= 0 || blue1 >= 255){
              green1 = 0;
              blue1 = 255;
              fadeState = 0;
            }
              break;
          }
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

    // Cannot PWM pin 27, 26, or 25 due to the PWMServo library using timers.

    if(RED1 != 27 && RED1 != 26 && RED1 != 25)
      analogWrite(RED1, red1);
    else
      digitalWrite(RED1, (red1 <= 0 ? LOW : HIGH));

    if(GREEN1 != 27 && GREEN1 != 26 && GREEN1 != 25)
      analogWrite(GREEN1, green1);
    else
      digitalWrite(GREEN1, (green1 <= 0 ? LOW : HIGH));

    if(BLUE1 != 27 && BLUE1 != 26 && BLUE1 != 25)
      analogWrite(BLUE1, blue1);
    else
      digitalWrite(BLUE1, (blue1 <= 0 ? LOW : HIGH));


    if(RED2 != 27 && RED2 != 26 && RED2 != 25)
      analogWrite(RED2, red2);
    else
      digitalWrite(RED2, (red2 <= 0 ? LOW : HIGH));

    if(GREEN2 != 27 && GREEN2 != 26 && GREEN2 != 25)
      analogWrite(GREEN2, green2);
    else
      digitalWrite(GREEN2, (green2 <= 0 ? LOW : HIGH));

    if(BLUE2 != 27 && BLUE2 != 26 && BLUE2 != 25)
      analogWrite(BLUE2, blue2);
    else
      digitalWrite(BLUE2, (blue2 <= 0 ? LOW : HIGH));


    stripMillis = now;

  }
}

void moneyUpdate(unsigned long now){

  switch(moneyState){
  case DISPREADY:
    if(irState == LOW && bottles > 0){
      moneyState = GETTING;
    }
    break;
  case DISPGETTING:
    motorPower = 255;
    if(photoState == HIGH) {
      moneyState = DISPENSING;
    }
    break;
  case DISPDISPENSING:
    if(photoState == HIGH){
      break;
    }else{
      motorPower = 0;
      btd++;
      moneyState = DISPDONE;
    }
    break;
  case DISPDONE:
    moneyState = READY;
    bottles--;
    bottleSold++;
    EEPROM.put(BTLS_EEPROM_ADR, bottleSold);
    break;
  }
}

void dispenserUpdate(unsigned long now){

  if(dispensing){
    dispense(now);
  }else{
    if(btd > 0){
      dispensing = true;
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

void dispense(unsigned long now){

  static unsigned long servoMillis = 0UL;


  if(now - servoMillis >= SERVODELAY){
    Serial.println(servoState);

    switch(servoState){
      case 0:{
        if(servoPos < SERVOMAX){
          servoPos += SERVOSTEP;
          break;
        }else{
          servoState = 1;
          break;
        }
        break;
      }
      case 1:{
        if(servoWait < SERVOSTOP/SERVODELAY){
          servoWait++;
          break;
        }else{
          servoWait = 0;
          servoState = 2;
          break;
        }
        break;
      }
      case 2:{
        if(servoPos > SERVOMIN){
          servoPos -= SERVOSTEP;
          break;
        }else{
          servoState = 3;
          break;
        }
        break;
      }
      case 3:{
        if(servoWait < SERVOSTOP/SERVODELAY){
          servoWait++;
          break;
        }else{
          servoState = 0;
          servoWait = 0;
          dispensing = false;
          btd--;
          break;
        }
        break;
      }
    }

    servoMillis = now;
  }
}

void keyUpdate(unsigned long now){
  static unsigned long keyMillis = 0UL;

  static int curPos = 1;

  static int screen = MAINSCRN;

  if(now - keyMillis >= KEYDELAY){

    char nkey = keypad.getKey();
    if (nkey){
      key = nkey;
    }

    strcpy(line0, "                    ");
    strcpy(line1, "                    ");
    strcpy(line2, "                    ");
    strcpy(line3, "                    ");

    switch(screen){
      case MAINSCRN:{
        strcpy(line0, "      Aquabot       ");
        sprintf(line1, "%s: %i%c", "Bottles Left", bottles, ' ');
        sprintf(line2, "%s: %i%c", "Bottles Sold", bottleSold, ' ');
        //sprintf(line3, "%s: %i%s", "Cooler Temp", coolerTemp, "F   ");
        strcpy(line3, "                    ");

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
              EEPROM.put(BTLS_EEPROM_ADR, bottleSold);
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

    Serial1.flush();

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

    Serial1.flush();

    lcdMillis = now;
  }


}

int getInt(char* title, int lower, int upper){
  static boolean firstDraw = true;
  static int digit = 0;
  static int digits = snprintf(0,0,"%+d",upper)-1;
  static char inBuf[4];

  static char oldTitle[21];

  if(strcmp(title, oldTitle) != 0){
    Serial.println("Changed!");
    strcpy(oldTitle, title);
    firstDraw = true;
  }

  int in;
  int lim = 0;

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
      firstDraw = true;
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
