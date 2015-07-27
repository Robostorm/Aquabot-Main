# Aquabot-Main-2015
Code for the main arduino(teensy) in the aquabot

This teensy controls the bill acceptor, LCD, keypad, and LED strips. There will be a disable signal wire running down to the driving board. Driving of the Aquabot will be disabled when this signal is high, but this is not yet final.

# Pinout
<pre>
Pin   Function    Pin Type
---------------------------
0     Red LED 1   I/O, PWM, INT0
1     Blue LED 1  I/O, PWM, INT1
2     N/C         I/O, RX , INT2
3     Display     I/O, TX , INT3
4     AUX Header  I/O
5     Keypad      I/O
6     Status LED  I/O
7     Keypad      I/O
8     Keypad      I/O
9     Keypad      I/O
10    Keypad      I/O
11    Keypad      I/O
12    Keypad      I/O
13    Keypad      I/O
14    Green LED 2 I/O, PWM
15    Red LED 2   I/O, PWM
16    Blue LED 2  I/O, PWM
17    Keypad      I/O
18    Cooler SW   I/O, INT6
19    N/C         I/O, INT7
20    N/C         I/O
21    N/C         I/O
22    Photo Int   I/O
23    IR Detector I/O
24    Bill Motor  I/O, PWM
25    Speaker     I/O, PWM
26    Servo       I/O, PWM
27    Green LED 1 I/O, PWM
28    RX Rudder   I/O
29    RX Aux 1    I/O
30    RX Aux 2    I/O
31    Drvng Enbl  I/O
32    RX Elev     I/O
33    RX Alie     I/O
34    RX Thro     I/O
35    Drvng Dsbl  I/O
36    N/C         I/O, INT4
37    N/C         I/O, INT5
38    Cooler Temp I/O, A0
39    N/C         I/O, A1
40    N/C         I/O, A2
41    N/C         I/O, A3
42    N/C         I/O, A4
43    N/C         I/O, A5
44    N/C         I/O, A6
45    N/C         I/O, A7
</pre>

# Bill Acceptor
The squence for accepting bills is like this:
First, the IR sensor must detect the bill. Then, the bill motor must be accepted to pull the bill in. At this point, the 'head sensor' magnetic sensor will start reading the magnetic threads in the bill. This is currently not used, but if anyone whants to implement it, they can. Now, the photo interupter will detect the lever at the end of the run of the bill exit being pressed. This lever will be released once the bill exits. This will now have to shut off the bill motor. This can also be used time the run of the bill through the bill acceptor to do some basic error checking.
