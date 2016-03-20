#ifndef __BUTTON_H
#define __BUTTON_H

#define BUTTONUNPRESSED 0
#define BUTTONPRESSED 1
#define BUTTONLONGPRESSED 2
#define BUTTONSHORTPRESSED 3

#define BUTTONSHORTDURATION 270

class Button {
  unsigned char pin;
  unsigned char state = BUTTONUNPRESSED;
  unsigned int duration = 0;

  public:
  Button(unsigned char);

  bool isPressed();
  bool isShortPressed();
  bool isLongPressed();

  void abortShortPress();

  void tick();

};

#endif
