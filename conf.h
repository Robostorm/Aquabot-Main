
/* File conf.h  */
#ifndef CONF
#define CONF

// Status LED
#define LEDDELAY 1000UL

// Dispenser
#define SENSEDELAY 10UL
#define MOTORDELAY 10UL
#define SERVODELAY 1UL
#define SERVOSTOP 700UL
#define SERVOSTEP 1
#define SERVOMIN 25
#define SERVOMAX 135
#define MAXBTLS 10
#define BTLS_EEPROM_ADR 0

// LCD
//#define SETUP_LCD
#define LCDDELAY 100UL
#define DRAWDELAY 1000UL
#define LCDWIDTH 20
#define LCDHEIGHT 4
#define MAINSCRN 0
#define MAINMENU 1
#define SETBTLS 2
#define DISPBTLS 3
#define LEDBRGHT 4
#define PASSWORD "369"
#define PASSSTRT '#'
#define PASSLEN 3
#define PASSACPT 'Y'
#define PASSDENY 'N'
#define SELECT '>'
#define MAXINT 999

// Keypad
#define KEYDELAY 100UL
#define KEYROW 5
#define KEYCOL 4
#define NOKEY ' '

// LED Strips
#define LEDSDELAY 10UL
#define LEDPATTERNS 3
#define BLNKDELAY 150UL
#define RAINBOWSPEED 1
#define FADESPEED 2

// Buttons
#define BUTTONSDELAY 10UL
#define LONGBUTTONDELAY 5000UL

#endif /* !CONF */
