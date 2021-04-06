//Hitbox.h
#ifndef Hitbox_h
#define Hitbox_h

#include "Option.h"
class Hitbox
{
  public:
    int x, y, w, h;
    byte cbVal;
    MenuPt *reference;// = new Option(0,"ll","ll");
    Hitbox(int _x, int _y, int _w, int _h, MenuPt *_reference) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      reference = _reference;
      cbVal = 0;
    }
    Hitbox(int _x, int _y, int _w, int _h, MenuPt *_reference, byte _cbVal) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      reference = _reference;
      cbVal = _cbVal;
    }
    Hitbox() {
      x = -100; y = -100; w = 0;  h = 0;
      reference = NULL;
      cbVal = 0;
    }

    void setPosition(int _x, int _y, int _w, int _h) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
    }
    void setReference(MenuPt *_reference) {
      reference = _reference;
    }
    void setCallbackValue(byte _cbVal) {
      cbVal = _cbVal;
    }

    void trigger() {
      //Option o = *reference; Works actually!
      //Serial.println((*reference).desc + " -> " + (*reference).value);
      if (cbVal == 0)
        (*reference).onClick();
      else
        (*reference).onClick(cbVal);
    }

    
};

#endif
