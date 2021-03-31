//Hitbox.h
#ifndef Hitbox_h
#define Hitbox_h

#include "Option.h"
class Hitbox
{
  public:
    int x, y, w, h;
    Option *reference;// = new Option(0,"ll","ll");
    Hitbox(int _x, int _y, int _w, int _h, Option *_reference) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      reference = _reference;
    }
    Hitbox() {
      x = 0;
      y = 0;
      w = 0;
      h = 0;
      //reference = _reference;
    }

    void trigger() {
      //Option o = *reference; Works actually!
      Serial.println((*reference).desc + " -> " + (*reference).value);
    }
};

#endif
