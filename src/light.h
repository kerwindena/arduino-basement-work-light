#ifndef __LIGHT_H
#define __LIGHT_H

#define LIGHTMAXBRIGHTNESS 255
#define LIGHTPERMSTORAGEADDRESS 0
#define LIGHTMAVGALPHA 5
#define LIGHTSAVEINTVAL 100
#define LIGHTUPDATEINTVAL 50

#define LIGHTON 1
#define LIGHTOFF 0


class Light {
  unsigned int maxBrightness = LIGHTMAXBRIGHTNESS;
  unsigned int actualBrightness = 0;
  unsigned int targetBrightness = 0;
  unsigned int savedBrightness = 0;
  unsigned char pin;
  unsigned char state = LIGHTOFF;

  unsigned int saveCounter = 0;
  unsigned int updateCounter = 0;

  unsigned int applyBrightness(unsigned int);
  void updateBrightness();
  void update();
  unsigned int getPersistentAddress();
  unsigned int load();
  void save();

  public:
  Light(unsigned char pin) : Light(pin, LIGHTOFF) {};
  Light(unsigned char pin, unsigned char state) : Light(pin, state, LIGHTMAXBRIGHTNESS) {};
  Light(unsigned char, unsigned char, unsigned int);

  void setBrightness(unsigned int);
  void offsetBrightness(int);
  void switchOff();
  void switchOn();
  void toggle();

  void tick();
};

#endif
