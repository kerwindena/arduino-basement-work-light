#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include "button.h"
#include "light.h"

const unsigned char numPwmPins = 6;
const unsigned char pwmPin[numPwmPins] = {3, 5, 6, 9, 10, 11};
const unsigned char numButPins = 4;
const unsigned char butPin[numButPins] = {7, 8, 12, 13};
Encoder encoder(2, 4);

const char numLights = 5;
Light* light[numLights];
const char numButtons = 4;
Button* button[numButtons];

unsigned char val = HIGH;

void setup() {
  Serial.begin(115200);
  // PWM:
  for (int i=0; i<numPwmPins; i++)
    pinMode(pwmPin[i], OUTPUT);

  // Buttons:
  for (int i=0; i<numButPins; i++)
    pinMode(butPin[i], INPUT_PULLUP);

  // Lights:
  light[0] = new Light(pwmPin[0], LIGHTON);  // backlight
  light[1] = new Light(pwmPin[1]);
  light[2] = new Light(pwmPin[2]);
  light[3] = new Light(pwmPin[3]);
  light[4] = new Light(pwmPin[4]);

  // Buttons:
  button[0] = new Button(butPin[0]);
  button[1] = new Button(butPin[1]);
  button[2] = new Button(butPin[2]);
  button[3] = new Button(butPin[3]);
}

void loop() {
  bool anyButtonPressed = false;
  int knobval;

  delay(1);

  // tick the buttons
  for (int i=0; i<numButtons; i++ )
    button[i]->tick();

  // get the knob value
  knobval = encoder.read();
  if (knobval != 0) {
    encoder.write(0);

    // interpret the buttons and the knob
    for (int i=0; i<numButtons; i++ ) {
      if (button[i]->isPressed()) {
        anyButtonPressed = true;
        button[i]->abortShortPress();
        light[i+1]->offsetBrightness(knobval);
      }
    }

    // interpret knob for the background light
    if (!anyButtonPressed) {
      light[0]->offsetBrightness(knobval);
    }

  }

  // check for short presses to toggle the lights
  for (int i=0; i<numButtons; i++ ) {
    if (button[i]->isShortPressed()) {
      light[i+1]->toggle();
    }
  }

  // tick the lights
  for (int i=0; i<numLights; i++ )
    light[i]->tick();
}
