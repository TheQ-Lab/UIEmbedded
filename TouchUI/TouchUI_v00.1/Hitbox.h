//Hitbox.h
#ifndef Hitbox_h
#define Hitbox_h

#include "Option.h"
class Hitbox
{
  public:
    int x, y, w, h;
    String desc;
    Option *reference;// = new Option(0,"ll","ll");
    Hitbox(int _x, int _y, int _w, int _h, String _desc, Option *_reference) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      desc = _desc;
      reference = _reference;
    }
    /*
    void addOption(Option& o) {
      reference = o;
    }*/

    void trigger() {
      Serial.println(desc);
    }
};

#endif
