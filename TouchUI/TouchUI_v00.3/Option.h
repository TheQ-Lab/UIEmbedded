//Options.h
#ifndef Option_h
#define Option_h

#include "Hitbox.h"
extern void popupValues2(String,String), popupValues3(String,String,String);
extern void drawMenu();
extern Hitbox hitboxes[];
class Option : public MenuPt
{
  public:
    short nTab, nPage, nOption;
    int x, y, w, h;
    String desc;
    byte setVal, nValues;
    String values[3];
    Option(short pos, String _desc, byte _setVal, String *_values) {
      desc = _desc;
      //value = _value;
      setVal = _setVal;
      //values = *_values;
      for (unsigned int i = 0; i < sizeof(values)/sizeof(String); i++) {
        values[i] = _values[i];
      }
      if(values[2] == "")
        nValues = 2;
      else
        nValues = 3;
      /*int     cx = 60,
              cy = 5;
      w = 250;
      h = 38;*/
      int     cx = 50,
              cy = 5;
              w = 220;
              h = 38;
      cy += h + 10;
      for (int i = 0; i < 5; i++) {
        if (i == pos) {
          x = cx;
          y = cy;
          nOption = pos;
          break;
        }
        cy += h + 10;
      }
    }
    
    Option() {}

    void draw() {
      tft.fillRoundRect(x, y, w, h, 4, COL_BG);

      tft.setTextColor(COL_TXT);  tft.setTextSize(1);
      //SET FONT ALWAYS BEFORE CURSOR, else y shift down
      tft.setFont(&NotoSans_Regular11pt7b);
      tft.setCursor(x + 10, y + 26);
      tft.println(desc);
      tft.setCursor(x + w - 10 - (20 * 2), y + 26);
      tft.print(values[setVal-1]);
    }

    void onClick() {
      Serial.println(desc + " -> " + values[setVal-1] + " : " + values[0] + " - " + values[1] + " - " + values[2]);
      if(nValues == 2)
        popupValues2(values[0], values[1]);
      else
        popupValues3(values[0], values[1], values[2]);
      for(int i = 0; i<nValues; i++) {
        hitboxes[i].setReference(this);
        hitboxes[i].setCallbackValue(i+1);
      }
    }

    void onClick(byte callbackVal) {
      setVal = callbackVal;
      drawMenu();
    }
};

#endif
