# Media Control Knob Attiny85
A Attiny85 powering Printables (Prusaprints) Media Control Knob that works as a HID. 
(If you are reading this, its still a work in progress)
## Description

An in-depth paragraph about your project and overview of use.

## Hardware Required
* Rotary Encoder
* Attiny85
* [3D printed base and knob by Mikolas Zuza](https://blog.prusa3d.com/3d-print-an-oversized-media-control-volume-knob-arduino-basics_30184/)

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
 
### Hardware "editting"
The Attiny85 has a smd LED connected to P1 that needs to be removed as it interferes with the rotary encoder readings. You can desolder it, use needle-nosed pliers to break it off, or carfully use a screw driver to destroy it from the top.

I recommend desoldering the right-angle header pins on the rotary encoder and either solder wires directly (or resolder right-angle header pins coming out from the bottom of the rotary encoder if using dupont wires) due to little space you have to connect wires when the rotary encoder is sitting flush with the 3D printed base. 

### usbconfig.h Settings (Changing device name and other rules)
By default from the TrinketHidCombo libary, the Attiny85 will be progammed to be a HID device that is:
* Vendor ID: 0x1782, Device ID: 0x24AB, which refers to a Vendor "Multiple Vendors" and unlisted Device ID.
* Vendor name: Adafruit, Device name: Trinket HID Combo, what will show up on your computers (for example in Windows->settings->devices will show the device name.

I don't believe that this is correct for the device desctription based on USB-IF standards since:
* A Vendor ID is $6,000 and can't find any resources relating to being able to "piggy-back" off of Vendor ID 0x1782.
* Adafruit's Vendor ID is 0x239A so not sure why they wouldn't for their Trinket Libabary set the Vendor and Device ID correctly (Adafruit has a device ID 0x081E as a "Trinket Mo")

So if you want to try to follow USB-IF standards then I encourge you to read [V-USB ID FAQ](https://github.com/obdev/v-usb/blob/master/usbdrv/USB-ID-FAQ.txt) and then [V-USB USB IDs for free](https://github.com/obdev/v-usb/blob/master/usbdrv/USB-IDs-for-free.txt) from the V-USB github. Following those guidelines, you can use Vendor ID 0x16c0 and Device ID based on your usecase. For example following the guidelines I used 0x27db for USB Keyboards (since only using common Keyboard keys in this project). Please note that the IDs are low byte first in usbconfig.h so using the Vendor ID, `#define  USB_CFG_DEVICE_ID       0xdb, 0x27` would be correct. 

For name changing, simply edit the character array in usbconfig.h of USB_CFG_VENDOR_NAME and USB_CFG_DEVICE_NAME (following the guidelines above) and update the name lengths.

## Help

Any advise for common problems or issues.
```
command to run if program contains helper info
```

## Version History

* 0.1
    * Initial Release

## Acknowledgments

Inspiration, code snippets, etc.
* Add later
