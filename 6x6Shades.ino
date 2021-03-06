#define LED_PIN  6

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define MAXBRIGHTNESS 255
#define STARTBRIGHTNESS 255

#define cycleTime 15000

#define hueTime 30
#define EEPROMDELAY 2000

#include <FastLED.h>
#include <EEPROM.h>
#include "palettes.h"
#include "messages.h"
#include "font.h"
#include "XYmap.h"
#include "utils.h"
#include "FireworksXY.h"
#include "effects.h"
#include "buttons.h"

functionList effectList[] = {
rainUp,
twoAnimations,
threeSineVert,
spinPlasma,
crazyRainbowV,
crazyRainbowH,
rainDown,
rainDown2,
snow,
shadesOutline2,
greenPortal,
colorPortal,
fireworks,
amazingNoise,
BlacK_Blue_Magenta_WhiteNoise,
SunsetNoise,
es_vintage_57Noise,
CloudNoise,
ForestNoise,
amazing,
rain,
threeSine,
plasma,
rider,
glitter,
colorFill,
threeDee,
sideRain,
juggle,
barfight,
confetti,
slantBars,
shadesOutline,
spirals,
scrollTextZero,
scrollTextOne,
scrollTextTwo
};

const byte numEffects = (sizeof(effectList)/sizeof(effectList[0]));

// Runs one time at the start of the program (power up or reset)
void setup() {
  Serial.begin(19200);
  byte eepromWasWritten = EEPROM.read(0);
  if (eepromWasWritten == 99) {
    currentEffect = EEPROM.read(1);
    autoCycle = EEPROM.read(2);
    currentBrightness = EEPROM.read(3);
  }

  if (currentEffect > (numEffects - 1)) currentEffect = 0;

  // write FastLED configuration data
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LAST_VISIBLE_LED + 1);

  // set global brightness value
  FastLED.setBrightness( scale8(STARTBRIGHTNESS, MAXBRIGHTNESS) );

  // configure input buttons
  pinMode(MODEBUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESSBUTTON, INPUT_PULLUP);

}


// Runs over and over until power off or reset
void loop() {
  Serial.print(currentEffect);
  Serial.print("\n");
  currentMillis = millis(); // save the current timer value
  updateButtons();          // read, debounce, and process the buttons
  doButtons();              // perform actions based on button state
  checkEEPROM();            // update the EEPROM if necessary

  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > cycleTime && autoCycle == true) {
    cycleMillis = currentMillis;
    if (++currentEffect >= numEffects) currentEffect = 0; // loop to start of effect list
    effectInit = false; // trigger effect initialization when new effect is selected
  }

  // increment the global hue value every hueTime milliseconds
  if (currentMillis - hueMillis > hueTime) {
    hueMillis = currentMillis;
    hueCycle(1); // increment the global hue value
  }

  // run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect](); // run the selected effect function
    random16_add_entropy(1); // make the random values a bit more random-ish
  }

  // run a fade effect too if the confetti effect is running
  if (effectList[currentEffect] == confetti || effectList[currentEffect] == rainDown2) fadeAll(1);
 
 if (fadeActive > 0) {
    fadeAll(fadeActive);
  }

  FastLED.show(); // send the contents of the led memory to the LEDs

}
