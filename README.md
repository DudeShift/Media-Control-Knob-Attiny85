# Media Control Knob Attiny85
A Attiny85 powering Printables (Prusaprints) Media Control Knob that works as a HID. 
(If you are reading this, its still a work in progress)
## Description

An in-depth paragraph about your project and overview of use.

## Getting Started

### Attiny85 DigiStump Board on Arduino IDE
[Read the DigitStump Wiki page here](https://digistump.com/wiki/digispark/tutorials/connecting "DigiStump Wiki") on how to get your Attiny85 set up for the Ardunio IDE but I will try to paraphrase below:
1. Install Digistump drivers from [https://github.com/digistump/DigistumpArduino/releases/download/1.6.7/Digistump.Drivers.zip](https://github.com/digistump/DigistumpArduino/releases/download/1.6.7/Digistump.Drivers.zip) running “DPInst64” (on 64bit systems). Installing them directly worked for me but manually setting drivers [as seen in this Youtube video](https://www.youtube.com/watch?v=MmDBvgrYGZs) that was linked on the Digistump Wiki and forums works as well for many people.
1. Add http://digistump.com/package_digistump_index.json to "Additional Boards Manager URLs" in Perferences.
2. Install "Digistump AVR Boards" from Tools -> Board -> Board Manager.
3. Use the "Digispark (Default - 16.5mhz)" board for programming.
4. When ready to program, leave your Attiny85 UNPLUGGED, press upload and wait for the bottom output box the Arduino IDE to ask to plug in the Attiny85. 

### Libaries Needed

* [ClickEncoder by soligen2010](https://github.com/soligen2010/encoder "ClickEncoder by soligen2010")
* [TimerOne](https://github.com/PaulStoffregen/TimerOne "TimerOne") import from github, import from libary manager didn't support Attiny85
* [TrinketHidCombo](https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo "TrinketHidCombo")

### Libary editting
To work with Arduino IDE 1.8.19 we have to edit some of the libaries.

* Digistump Libaries: Go to C:\Users\[YourUserName]\AppData\Local\Arduino15\packages\digistump\hardware\avr\1.6.7, create a backup of the "libraries" folder (not in the same directory) and then delete the "libraries" folder. The v-usb files in Digitstump libaries causes conflict with v-usb files in TrinketHidCombo due to the Arduino IDE using the Digistump ones. (Fixes "Device Descriptor Request Failed" or "An unknown item was found in the report descriptor" Windows errors)
* TrinketKeyboard\usbdrv/usbdrvasm.S: Edit this file by adding before the `#define __SFR_OFFSET 0` (line 17)
```
#undef __SFR_OFFSET
 ```
 And at Line 73 change the `#if` to
 ```
 #if 0x41 < 0x40
 ```
 This is to fix the `#define __SFR_OFFSET 0` warning at compiling AND to fix "Device Descriptor Request Failed" or "An unknown item was found in the report descriptor" Windows errors. (From [Adafruit Trinket USB Issue 22](https://github.com/adafruit/Adafruit-Trinket-USB/issues/22) and [Issue 27](https://github.com/adafruit/Adafruit-Trinket-USB/issues/27)

### Executing program

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

## Help

Any advise for common problems or issues.
```
command to run if program contains helper info
```

## Authors

Contributors names and contact info

ex. Dominique Pizzie  
ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History

* 0.1
    * Initial Release

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)
