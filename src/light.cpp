#include "light.h"

#include <Arduino.h>
#include <EEPROM.h>

Light::Light(unsigned char pin, unsigned char state, unsigned int maxBrightness) {
  this->pin = pin;
  if (maxBrightness > LIGHTMAXBRIGHTNESS)
    maxBrightness = LIGHTMAXBRIGHTNESS;

  this->maxBrightness = maxBrightness;
  this->savedBrightness = this->load();

  this->targetBrightness = this->applyBrightness(this->savedBrightness);

  if (state != LIGHTON)
    this->switchOff();
}

unsigned int Light::applyBrightness(unsigned int brightness) {
  if (brightness <= this->maxBrightness)
    return brightness;else
  return this->maxBrightness;
}

void Light::updateBrightness() {
  if (this->actualBrightness == this->targetBrightness)
    return;

  int newBrightness = this->actualBrightness * (100-LIGHTMAVGALPHA) / 100;
  newBrightness += this->targetBrightness * LIGHTMAVGALPHA / 100;

  if (this->actualBrightness < this->targetBrightness) {
    if (newBrightness <= this->actualBrightness)
      newBrightness = this->actualBrightness + 1;
  } else {
    if (newBrightness >= this->actualBrightness)
      newBrightness = this->actualBrightness - 1;
  }

  this->actualBrightness = this->applyBrightness(newBrightness);
}

void Light::update() {
  analogWrite(this->pin, this->applyBrightness(this->actualBrightness));
}

unsigned int Light::getPersistentAddress() {
  return LIGHTPERMSTORAGEADDRESS + sizeof(unsigned int) * this->pin;
}

unsigned int Light::load() {
  unsigned int address = this->getPersistentAddress();
  unsigned int value = 0;
  EEPROM.get(address, value);
  return value;
}

void Light::save() {
  if (this->state != LIGHTON)
    return;
  if (this->targetBrightness != this->savedBrightness) {
    this->saveCounter++;
    if (this->saveCounter > LIGHTSAVEINTVAL) {
      unsigned int address = this->getPersistentAddress();
      this->saveCounter = 0;
      EEPROM.put(address, this->targetBrightness);
      this->savedBrightness = this->targetBrightness;
    }
  }
}

void Light::setBrightness(unsigned int brightness) {
  this->switchOn();

  this->targetBrightness = this->applyBrightness(brightness);
  this->actualBrightness = this->targetBrightness;

  this->update();
}

void Light::offsetBrightness(int offset) {
  this->switchOn();
  if (offset < 0) {
    if (-offset >= this->targetBrightness) {
      this->setBrightness(0);
      return;
    }
  }
  this->setBrightness(this->targetBrightness + offset);
}

void Light::switchOff() {
  this->saveCounter = LIGHTSAVEINTVAL;
  this->save();
  this->state = LIGHTOFF;
  this->targetBrightness = 0;
}

void Light::switchOn() {
  if (this->state == LIGHTON)
    return;
  this->targetBrightness = this->applyBrightness(this->savedBrightness);
  this->state = LIGHTON;
}

void Light::toggle() {
  if (this->state == LIGHTON)
    this->switchOff();
  else
    this->switchOn();
}

void Light::tick() {
  this->updateCounter++;
  if (this->updateCounter > LIGHTUPDATEINTVAL) {
    this->updateCounter = 0;
    this->updateBrightness();
    this->update();
  }
  this->save();
}
