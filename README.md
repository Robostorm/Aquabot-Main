# Aquabot-Main-2015
Code for the main arduino(teensy) in the aquabot

This teensy controls the bill acceptor, LCD, keypad, and LED strips. There will be a disable signal wire running down to the driving board. Driving of the Aquabot will be disabled when this signal is high, but this is not yet final.

# Pinout
<pre>
Pin   Function    Pin Type
---------------------------
0     I/O Header  PWM
1     I/O Header  PWM
2     I/O Header  Serial RX
3     Display     Serial TX
4     N/C         I/O
5     I/O Header  I/O
6     Status LED  I/O
7     I/O Header  I/O
8     I/O Header  I/O
9     I/O Header  I/O
10    Out Driver  I/O
11    Out Driver  I/O
12    Out Driver  I/O
13    Out Driver  I/O
14    Out Driver  PWM
15    Out Driver  PWM
16    Out Driver  PWM
17    Out Driver  I/O
18    N/C         I/O
19    N/C         I/O
20    N/C         I/O
21    N/C         I/O
22    Photo Int   I/O
23    IR Detector I/O
24    Bill Motor  PWM
25    Speaker     PWM
26    Servo       PWM
27    I/O Header  PWM
38    N/C         Analog Input
39    N/C         Analog Input
40    N/C         Analog Input
41    N/C         Analog Input
42    N/C         Analog Input
43    N/C         Analog Input
44    N/C         Analog Input
45    N/C         Analog Input
</pre>

Most of the pins on the I/O header are now routed to the keypad, LED strips, and cooler switch. One of the analog pins is now connected to the cooler thermistor.

The out driver pins go to the I/O header though a DIP socket for one of the output drivers on the original bill acceptor control board. This will be replaced with jumpers to connect the Teensey pin directly.

# Bill Acceptor
The squence for accepting bills is like this:
First, the IR sensor must detect the bill. Then, the bill motor must be accepted to pull the bill in. At this point, the 'head sensor' magnetic sensor will start reading the magnetic threads in the bill. This is currently not used, but if anyone whants to implement it, they can. Now, the photo interupter will detect the lever at the end of the run of the bill exit being pressed. This lever will be released once the bill exits. This will now have to shut off the bill motor. This can also be used time the run of the bill through the bill acceptor to do some basic error checking.
