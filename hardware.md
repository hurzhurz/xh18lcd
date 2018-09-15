# Hardware
## MCU & Pin assignment
* Type: STM8S005C6
* [Datasheet](https://www.st.com/resource/en/datasheet/stm8s005c6.pdf)
* [photos](/images/pcb/)

pin | function
--- | --------
PA2 | LCD control /CS
PA3 | LCD control /RESET
PA4 | LCD control RS
PA5 | LCD control /WR
PA6 | LCD control /RD
PB5 | Button K1 (on/off)
PB6 | Button K2 (i)
PB7 | Grip-hallsensor analog input
PC1 | LCD data DB6
PC2 | LCD data DB5
PC3 | LCD data DB4
PC4 | LCD data DB3
PC5 | LCD data DB2
PC6 | LCD data DB1
PC7 | LCD data DB0
PD2 | LCD backlight control
PD3 | LCD backlight control
PD5 | UART TX
PD6 | UART RX
PD7 | Soft power latch
PE5 | LCD data DB6

## Power supply / control
* soft power latch
  * The supply voltage from the battery is switched via a [transistor](/images/latch-transistor.jpg).
  * It is activated while pressing the power button.
  * The microcontroller has to set its pin PD7 to HIGH so it stays on when the button is no more pressed
* buck regulator 12V for the needs of the LCD Backlight
* 5V linear regulator for the microcontroller, fed from 12V

## LCD
* Type: [COG-T177MZHS-11](/images/lcd.jpg)
* [Datasheet of similar display](http://www.tecenstar.com/Kindeditor/attached/file/20160112/20160112164723_2158.pdf)
* Resolution: 160x128
* Controller: ST7735S
* Interface: 8-bit parallel

* Backlight can be controlled via PD2 and PD3:
  * one LOW, one HIGH: normal brightness
  * both HIGH: maximum brightness


## Buttons
* Buttons are connected to the pins stated above.
* LOW active
* external pullup resistors
* capacitors for debouncing(?)

## Grip hallsensor
* Rotation state of the grip is detected by a hallsensor
* [SS49E](/images/hallsensor.jpg) connected to analog input via a RC filter
