//Hitbox.h
#ifndef Hitbox_h
#define Hitbox_h

#include "Option.h"
class Hitbox
{
  public:
    int x, y, w, h;
    MenuPt *reference;// = new Option(0,"ll","ll");
    Hitbox(int _x, int _y, int _w, int _h, MenuPt *_reference) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      reference = _reference;
    }
    Hitbox() {
      x = -100; y = -100; w = 0;  h = 0;
      //reference = _reference;
    }

    void trigger() {
      //Option o = *reference; Works actually!
      //Serial.println((*reference).desc + " -> " + (*reference).value);
      (*reference).onClick();
    }
};

#endif
