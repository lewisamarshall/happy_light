Happy Light
===========
A custom wake-up light PCB and arduino code. 

Lights
------
Intended for use with [Adafruit RGBW LED Strip](https://www.adafruit.com/product/2439?length=4). 
These run at 12 V, and draw up to 1.6A/meter, or 400 mA/meter per color.

Power
-----
The board is intended to run on 12V power input (both microcontroller and LED power).
For a full spool (4m) of the lights above, this could be about 80W total.
Power is controlled by 4x TO-3 darlington power transistors.

Control
=======
This is designed for an Arduino Nano. The firmware assumes a Nano 33 IOT, to get network time.
