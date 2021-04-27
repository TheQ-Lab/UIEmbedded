//Tab.h

#ifndef Tab_h
#define Tab_h

//#include "Hitbox.h"
#include "Option.h"
extern void switchTab(byte);
class Tab : public MenuPt
{
  public:
    int x,y,w,h;
    Option *opts;
    int nOpts;
    short nTab;
    uint16_t color;
    Tab(short pos, Option *_reference, uint16_t _color, int _nOpts) {
      opts = _reference;
      color = _color;
      nOpts = _nOpts;
      nTab = pos;
      short cx = 0;
      short cy = 15;
      w = 40;
      h = 35;
      for (int i = 1; i <= 4; i++) {
        if (i == pos) {
          x = cx;
          y = cy;
        }
        //HERE HITBOXES TABS    
        cy += h + 10;
      }
    }
    
    void draw(){
      tft.fillRoundRect(x, y, w, h, 4, color);
    }
    
    void onClick(){
      Serial.print("OOohhh, you touche my "); Serial.println(nTab);
      switchTab(nTab);
    }
    
    void onClick(byte){
      
    }
};

#endif
