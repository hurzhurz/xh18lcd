# Firmware Development
This is example (proof of concept) for a custom firmware for the XH18LCD.

Motivation was to get a starting point so that a version can follow that supports the [Flexible OpenSource motor firmware](https://github.com/OpenSource-EBike-firmware/TSDZ2_wiki/wiki), so the XH18LCD can be used alternative for the KT-LCD3.

This version is made for the original TSDZ2 motor firmware.

![custom firmware](/images/custom-firmware.jpg)

## How to build
1. Get Arduino
2. Install [Sduino: Small Devices Arduino](https://github.com/tenbaht/sduino)
3. Replace boards.txt delivered by Sduino with my [extended boards.txt](boards.txt)
4. Get my modified version of the UTFT display library: https://github.com/hurzhurz/UTFT-STM8S
5. Download folder XH18LCD and open the main project file XH18LCD.ino
6. Select board "STM8S005C6 XH18-LCD"
7. Build it
8. For Uploading/Flashing, see where to connect the ST-LINK [here](/programming-interface.md)

## Implemented features
* Power off by long pressing the power button
* Adjustable assist level off/1-5 by turning the grip (1 is the unused/hidden one)
* Settings menu for wheel size and maximum speed
  * to enter and leave, long press the "i" button
  * select the option to change by power and i button and change it by turning the grip
* Speed display (in km/h)
* Odometer for total and trip
* Settings and total distance saved in EEPROM
* LCD backlight can be cycled though the steps off/low/high by double pressing the power button
* Toggling of the headlight by single press of the power button, indicated by a white or blue little box in the upper right corner
* Push assist / 6kmh mode, activated by holding grip rotated and indicated by a red or green little box in the upper right corner
* Battery level indicator

## Caveats
* Probably not very efficient
* Size is at the limit of what the flash allows
* Display refresh of whole screen is slow and holds up main loop, so reduced to a minimum (just changes)


## Possible improvements in the future
* Use of interrupts for buttons (if available)
* Use of PWM for LCD backlight (if available)
* Usage of just the "nativ" SDCC instead of Sduino for higher efficiency (performance/space)
* Adapt to display additional data from the [improved motor firmware](https://endless-sphere.com/forums/viewtopic.php?f=28&t=94351)
* **Support for the [Flexible OpenSource firmware](https://github.com/OpenSource-EBike-firmware/TSDZ2_wiki/wiki)**
* Nicer layout
