#include "button.h"

#include <Arduino.h>

Button::Button(unsigned char pin) {
  this->pin = pin;
}

bool Button::isPressed() {
  if (this->state == BUTTONPRESSED)
    return true;
  if (this->state == BUTTONLONGPRESSED)
    return true;
  return false;
}

bool Button::isShortPressed() {
  if (this->state == BUTTONSHORTPRESSED) {
    this->state = BUTTONUNPRESSED;
    return true;
  }
  return false;
}

bool Button::isLongPressed() {
  if (this->state == BUTTONLONGPRESSED)
    return true;
  return false;
}

void Button::abortShortPress() {
  if (this->isPressed())
    this->state = BUTTONLONGPRESSED;
}

void Button::tick() {
  unsigned char val;
  val = digitalRead(this->pin);

  if (val != HIGH) {

    if (this->isLongPressed())
      return;

    if (!this->isPressed()) {
      // button is just pressed down
      this->duration = 0;
      this->state = BUTTONPRESSED;
    } else {
      // button is pressed, but not long-pressed yet
      this->duration++;
      if (this->duration > BUTTONSHORTDURATION)
        this->state = BUTTONLONGPRESSED;
    }

  } else {

    if (!this->isPressed())
      return;

    // button is just released

    if (!this->isLongPressed()) {
      this->state = BUTTONSHORTPRESSED;
    } else {
      this->state = BUTTONUNPRESSED;
    }

  }
}
