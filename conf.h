
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
#define SERVOMIN 45
#define SERVOMAX 135

// LCD
#define LCDDELAY 500UL
#define DRAWDELAY 5000UL
#define LCDWIDTH 20
#define LCDHEIGHT 4
#define MAINMENU 0
#define SETBTLS 1
#define DISPBTLS 2
#define LEDBRGHT 3
#define PASSWORD "369"
#define SELECT '>'
#define MAXINT 999

// Keypad
#define KEYDELAY 10UL
#define KEYROW 5
#define KEYCOL 4
#define NOKEY ' '

// LED Strips
#define LEDSDELAY 10UL
#define LEDPATTERNS 3
#define BLNKDELAY 150UL
#define RAINBOWSPEED 1
#define FADESPEED 2

#endif /* !CONF */
