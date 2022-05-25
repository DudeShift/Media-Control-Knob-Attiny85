// Import the ClickEncoder library from https://github.com/soligen2010/encoder: 
#include <ClickEncoder.h>
// TimerOne from github NOT ARDUNIO library from https://github.com/PaulStoffregen/TimerOne
#include <TimerOne.h>
/* Can't use HID-Project due to not allowing v-usb
 * DigiKeyboard doesn't have multi-media keys
 * So using Trinket which should work. (http://digistump.com/board/index.php?topic=1608.0)
 * View https://github.com/adafruit/Adafruit-Trinket-USB/blob/master/TrinketHidCombo/TrinketHidCombo.h for keycodes
 */

#include <TrinketHidCombo.h>
#include <TrinketHidComboC.h>

// Port Pin IDs for Arduino / Attiny
// For Attiny85, need to remove or destory led on pin 1.
#define PIN_A 1
#define PIN_B 0
#define PIN_Button 2 //Switch or Button on rotary encoder
#define STEPS 4 //Steps per notch? (CHECK)

// Settings for encoder
int16_t oldEncPos, encPos;
//# define ENC_DECODER ENC_FLAKY //uncomment for flaky notch (half resolution)

// Setings for having held button only make a single action
bool setHeldSingleAction;
bool firstTimeHeld;


ClickEncoder encoder(PIN_A, PIN_B, PIN_Button, STEPS);

// --- forward-declared function prototypes:
void handleButtonState(); // provide keypress from button state
void handleEncoderValue(); // provide keypress from encoder value
void TrinketHidCompatibleDelay(unsigned int desiredDelay_ms); //-this is a USB-compatible delay (CHECK) https://github.com/adafruit/Adafruit-Trinket-USB/issues/20

// Settings for being able to have multiple key sets that a user can switch between.
// For this ino file, just two sets toggeled between when double clicking.
bool setMultipleKeySets;
bool currentKeySet;
                 
void timerIsr()
{
  // This is the Encoder's worker routine. It will physically read the hardware
  // and all most of the logic happens here. Recommended interval for this method is 1ms.
  encoder.service();
}

void setup()
{
  // Configuration for ClickEncoder
  encoder.setAccelerationEnabled(true);
  encoder.setDoubleClickEnabled(true);

  // Configuration for personal encoder settings
  setHeldSingleAction = true;
  setMultipleKeySets = true;

  // Setting default values
  oldEncPos = 0; // Set default encoder postion (CHECK)
  currentKeySet = 0;
  firstTimeHeld = true;

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  TrinketHidCombo.begin(); // start the USB device engine and enumerate

  TrinketHidCompatibleDelay(2000); //increase to 2000 to remove the print errors entirely; decrease to 0 to make the errors worse and more frequent; use 1000 to cause occasional initial print errors only
  
  //wait until connected to USB before continuing
  //-note that isConnected() will change to true at that point 
  unsigned long counter = 0;
  while (TrinketHidCombo.isConnected() == false)
  {
    counter++;
    TrinketHidCombo.poll(); //The poll function must be called at least once every 10 ms, or you must command a keystroke. Otherwise, the computer may think that the device has stopped working, and give errors.
  }
}

void loop()
{  
    delay(10); // Wait 10 milliseconds
    TrinketHidCombo.poll(); // Keep HID usb connection alive
    // Reads Encoder's status/values and does keypresses
    handleButtonState();
    handleEncoderValue();
    
}

void handleButtonState()
{
    switch (encoder.getButton())
    {
    case ClickEncoder::Clicked:
      if (setMultipleKeySets && currentKeySet) { //altKeySet
        TrinketHidCombo.pressKey(0, KEYCODE_SPACE);
        TrinketHidCombo.pressKey(0,0);
      } else { //default keyset
        TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);
      }
      break;
    case ClickEncoder::DoubleClicked:
        currentKeySet = !currentKeySet; // change key set
        break;
    case ClickEncoder::Held:
      if (setHeldSingleAction) {
        if (firstTimeHeld) {
        TrinketHidCombo.pressKey(KEYCODE_LEFT_ALT, KEYCODE_M);
        TrinketHidCombo.pressKey(0, 0);
        firstTimeHeld = false;
        }
      } else {
        TrinketHidCombo.pressKey(KEYCODE_LEFT_ALT, KEYCODE_M);
        TrinketHidCombo.pressKey(0, 0);
      }
      break;
    case ClickEncoder::Released:
        firstTimeHeld = true;
        break;
    default:
        // do nothing for any other states
        break;
    }
}

void handleEncoderValue()
{
  encPos += encoder.getValue();
  if (encPos != oldEncPos)
  {
  int encDistance = encPos - oldEncPos;
  if (encDistance > 0) { // postive value so moving right
   for (int i = 0; i < encDistance; i++) { //Accleration Handling
    if (setMultipleKeySets && currentKeySet) {
     TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT);
     TrinketHidCombo.pressKey(0,0); 
    } else {
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
    }
   }
  } else { // negative value so moving left
    for (int i = 0; i > encDistance; i--) { //Accleration Handling
      if (setMultipleKeySets && currentKeySet) {
       TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT);
        TrinketHidCombo.pressKey(0,0); 
      } else {
        TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
      }
     }
    }
  oldEncPos = encPos;
 }
}

//--------------------------------------------------------------------------------------------
//TrinketHidCompatibleDelay
//-this is a USB-compatible delay, so that the 10ms max time between poll calls is not exceeded
//--------------------------------------------------------------------------------------------
void TrinketHidCompatibleDelay(unsigned int desiredDelay_ms)
{
  unsigned long t_start = millis(); //ms
  while (millis()-t_start<desiredDelay_ms)
  {
    TrinketHidCombo.poll(); //keep the HID USB connection to the computer alive 
  }
}
