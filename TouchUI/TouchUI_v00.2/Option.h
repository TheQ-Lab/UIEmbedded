//Options.h
#ifndef Option_h
#define Option_h

extern Adafruit_ILI9341 tft;
class Option : public MenuPt
{
  typedef void (*callback_t)();
  public:
    short nTab, nPage, nOption;
    int x, y, w, h;
    String desc, value;
    byte setVal;
    String values[3];
    callback_t callback;
    Option(short pos, String _desc, byte _setVal, String *_values, callback_t _callback)
    : callback(_callback){
      desc = _desc;
      //value = _value;
      setVal = _setVal;
      //values = *_values;
      for (unsigned int i = 0; i < sizeof(values)/sizeof(String); i++) {
        values[i] = _values[i];
      }
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
      Serial.println(desc + " -> " + value + " - " + values[0] + " - " + values[1] + " - " + values[2]);
      //popupValues2();
      callback();
    }

    void assignPopup()
      {
    }
};

#endif
