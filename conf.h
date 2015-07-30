
/* File conf.h  */
#ifndef CONF
#define CONF

// Status LED
#define LEDDELAY 1000UL

// Dispenser
#define SENSEDELAY 10UL
#define MOTORDELAY 10UL
#define SERVODELAY 1UL
#define SERVOSTOP 400UL
#define SERVOSTEP 1
#define SERVOMIN 45
#define SERVOMAX 135

// LCD
#define LCDDELAY 100UL
#define SCREENS 2
#define MAINSCR 0
#define MENUSCR 1
#define PASSWORD "#369"
#define SELECT '>'

// Keypad
#define KEYDELAY 0UL
#define KEYROW 5
#define KEYCOL 4
#define NOKEY ' '

// LED Strips
#define LEDSDELAY 10UL
#define LEDPATTERNS 3
#define BLNKDELAY 150UL

#endif /* !CONF */
