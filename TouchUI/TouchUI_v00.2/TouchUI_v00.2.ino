#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "TouchScreen.h"

#include <Fonts/NotoSans_Regular15pt7b.h>
#include <Fonts/NotoSans_Regular11pt7b.h>
#include <Fonts/NotoSans_Regular6pt7b.h>

// referenzieren von klassen in anderen Klassen
// übergeben von arrays - check
// Array definieren, ohne zu befüllen


// Tab -> Page -> Option -> Value

// For the Adafruit shield, these are the default.
#define TFT_DC 22
#define TFT_CS 10

// These are the four touchscreen analog pins
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 15   // can be a digital pin
#define XP 22   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 100
#define TS_MINY 70
#define TS_MAXX 900
#define TS_MAXY 900

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define MAXTOUCHLOCK 15

// This is Preset colors in 16-Bit Hex values (not standard 24-Bit)
#define COL_BG      0x5248
#define COL_TXT     0xFFFF
#define COL_YELLOW  0xDE64
#define COL_RED     0xD927
#define COL_GREEN   0x7EC4
#define COL_BLUE    0x2B1D

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//include later to have other definitions in
#include "SerialCom.h"
#include "MenuPt.h"
#include "Option.h"
#include "Hitbox.h"

byte touchLock;
uint16_t lastX, lastY;
String valuesBool[] = { "ON", "OFF" }; String valuesIntensity[] = { "1", "2", "3" };
Option options[] = {
  Option(0, "AF-Lock", 1, valuesBool),
  Option(1, "Cool Feature", 1, valuesBool),
  Option(2, "L-Sensitivity", 1, valuesIntensity),
  Option(3, "Mag-Sensitivity", 3, valuesIntensity)/*,
  Option(4, "Auto pwr-off", "OFF")*/
};


//später 11 = 5 Options + 2 Page-Tasten + 3 Tabs + 1 Back
Hitbox hitboxes[4]; //NEEDED a second empty constructor...



void setup() {
  /*for (uint8_t i = 0; i < sizeof(options) / sizeof(Option); i++) {
    hitboxes[i] = Hitbox((int)options[i].x, options[i].y, options[i].w, options[i].h, &options[i]);
  }*/
  fillHitboxesOptions();

  Serial.begin(9600);
  delay(1000);
  Serial.println("ILI9341 TouchUI!");

  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

  Serial.print("Screen size: x = "); Serial.print(tft.width());
  Serial.print(", y = "); Serial.println(tft.height());

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  Button9();
}

void loop() {
  String cmd = serialProcess();

  // Retrieve a point
  TSPoint p = ts.getPoint();
  if (touchLock == 0) {
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      //Serial.print("X: "); Serial.print(p.x);
      //Serial.print(", Y: "); Serial.print(p.y);
      //Serial.print(", Z: "); Serial.print(p.z);
  
      uint16_t nY = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
      uint16_t nX = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
      p.x = nX;
      p.y = nY;
  
      //Serial.print("X: "); Serial.print(p.x);
      //Serial.print(", Y: "); Serial.print(p.y);
      //Serial.print(", Z: "); Serial.println(p.z);
      //Only erasing last cross from Background by re-pasinting it with BG-Color
      tft.drawLine(lastX - 20, lastY, lastX + 20, lastY, ILI9341_BLACK);
      tft.drawLine(lastX, lastY - 20, lastX, lastY + 20, ILI9341_BLACK);
  
      //Button9();
      tft.drawLine(p.x - 20, p.y, p.x + 20, p.y, COL_GREEN);
      tft.drawLine(p.x, p.y - 20, p.x, p.y + 20, COL_GREEN);
  
      processTouch(p, hitboxes, sizeof(hitboxes) / sizeof(Hitbox));
  
      lastX = nX; lastY = nY;
      touchLock = MAXTOUCHLOCK;
    }
  } else if (touchLock > 0) {
    if (p.z <= MINPRESSURE) 
      touchLock--;
    else
      touchLock = MAXTOUCHLOCK;
  }

  if (cmd == "cfont")
    CustomFont();
  else if (cmd == "b4")
    Button4();
  else if (cmd == "b9")
    Button9();
  else if (cmd == "bs5")
    ButtonS5();
  else if (cmd == "clr")
    tft.fillScreen(ILI9341_BLACK);

}

void fillHitboxesOptions() {
  for (uint8_t i = 0; i < sizeof(options) / sizeof(Option); i++) {
    hitboxes[i] = Hitbox((int)options[i].x, options[i].y, options[i].w, options[i].h, &options[i]);
  }
}

void fillHitboxesNull() {
  for (uint8_t i = 0; i < sizeof(hitboxes) / sizeof(Hitbox); i++) {
    hitboxes[i] = Hitbox();
  }
}

void popupValues2(String val1, String val2) {
  byte n = 2;
  //dither();
  int         cx = 205,
              cy = 101,
              w = 80,
              h = 38,
              d = 10,
              m = 17;
  tft.fillRoundRect(cx-m,cy-m,2*m+w,2*m+n*h+(n-1)*d,9,ILI9341_BLACK);
  tft.drawRoundRect(cx-m,cy-m,2*m+w,2*m+n*h+(n-1)*d,9,COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT LAWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);


  fillHitboxesNull();
  for (int i=0; i<n; i++) {
    tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);
    tft.setCursor(cx + 15, cy + 26);
    if(i==0)
      tft.println(val1);
    else if (i==1)
      tft.println(val2);
    hitboxes[i].setPosition(cx, cy, w, h);
    cy+=h+d;
  }
}

void popupValues3(String val1, String val2, String val3) {
  byte n = 3;
  //dither();
  int         cx = 205,
              cy = 75,
              w = 80,
              h = 38,
              d = 10,
              m = 17;
  tft.fillRoundRect(cx-m,cy-m,2*m+w,2*m+n*h+(n-1)*d,9,ILI9341_BLACK);
  tft.drawRoundRect(cx-m,cy-m,2*m+w,2*m+n*h+(n-1)*d,9,COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT LAWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);

  fillHitboxesNull();
  for (int i=0; i<n; i++) {
    tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);
    tft.setCursor(cx + 15, cy + 26);
    if(i==0)
      tft.println(val1);
    else if (i==1)
      tft.println(val2);
    else if (i==2)
      tft.println(val3);
    hitboxes[i].setPosition(cx, cy, w, h);
    cy+=h+d;
  }
}

void Button9() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 50,
              cy = 5,
              w = 220,
              h = 38;
  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT ALWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);
  tft.drawLine(cx, cy + h, cx + w + 45, cy + h, COL_BG);
  tft.setCursor(cx + 10, cy + 26);
  tft.println("Toller Titel");
  /*
    for (int i = 0; i < 5; i++) {
    if (i == 0) {
      //tft.drawRoundRect(cx, cy, w+45, h, 4, COL_BG);

      cy += h + 10;
      continue;
    }

    tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);

    tft.setCursor(cx + 10, cy + 26);
    if (i == 0)
      tft.println("AF-Lock");
    else if (i == 1)
      tft.println("Cool Feature");
    else if (i == 2)
      tft.println("L-Sensitivity");
    else if (i == 3)
      tft.println("Mag-Sensitivity");
    else if (i == 4)
      tft.println("Auto pwr-off");

    tft.setCursor(cx + w - 10 - (20 * 2), cy + 26);
    if (i == 0)
      tft.print("ON");
    else if (i == 1)
      tft.print("ON");
    else if (i == 2)
      tft.print("1");
    else if (i == 3)
      tft.print("3");
    else if (i == 4)
      tft.print("OFF");

    cy += h + 10;
    }
  */
  for (uint8_t i = 0; i < sizeof(options) / sizeof(Option); i++) {
    options[i].draw();
  }

  //nav right Page
  cx = cx + w + 10,
  cy = 70,
  w = 35,
  h = 35;
  tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);
  tft.drawRoundRect(cx, cy + 50 + 60, w, h, 4, COL_BG);
  tft.setCursor(cx + 10, cy + 70 - 1);
  tft.print("1");
  cy += 5;
  tft.drawLine(cx + w - 10 - 17, cy + 70 + 15, cx + w - 17, cy + 70 + 5, COL_TXT);
  tft.setFont(&NotoSans_Regular6pt7b);
  tft.setCursor(cx + 14, cy + 70 + 20);
  tft.print("10");
  tft.setFont(&NotoSans_Regular11pt7b); //reset font

  //Tab-Bar
  cx = 0,
  cy = 35,
  w = 40,
  h = 35;
  for (int i = 0; i < 4; i++) {
    if (i == 0)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_YELLOW);
    else if (i == 1)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_RED);
    else if (i == 2)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_GREEN);
    else if (i == 3)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_BLUE);
    cy += h + 10;
  }
}

// ----Touch----

// passing array.length on does not work, due to it having to be a pointer
void processTouch(TSPoint p, Hitbox *h, byte hSize) {
  bool isHit;
  for (uint8_t i = 0; i < hSize; i++) {
    isHit = touchCheckCollision(h[i], p.x, p.y);
    if (isHit) {
      h[i].trigger();
      break;
    }
  }
}

bool touchCheckCollision(Hitbox h, int tx, int ty) {
  if (h.x <= tx && tx <= h.x + h.w) {
    if (h.y <= ty && ty <= h.y + h.h) {
      return true;
    }
  }
  return false;
}

/*bool touchCheckCollision(int bx, int by, int bw, int bh, int tx, int ty) {
  if (bx <= tx && tx <= bx + bw) {
    if (by <= ty && ty <= by + bh) {
      return true;
    }
  }
  return false;
  }*/

// ----Tft-Screen----

void ButtonS5() {
  //tft.fillScreen(ILI9341_BLACK);
  int         cx = 35,
              cy = 101,
              w = 250,
              h = 38;
  tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT LAWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);

  tft.setCursor(cx + 10, cy + 26);
  tft.println("Random Warning");
  //tft.setCursor(cx+w-10-(20 * 2), cy+26);
  //tft.print("ON");

  tft.fillCircle(260, 120, 15, COL_YELLOW);
  tft.fillTriangle(260, 120 - 13, 260 - 10, 120 + 8, 260 + 10, 120 + 8, COL_RED);
}


void Button4() {
  //tft.fillScreen(ILI9341_BLACK);
  /*
    int         cx = 60,
              cy = 5,
              w = 250,
              h = 38;
    tft.setTextColor(COL_TXT);  tft.setTextSize(1);
    //SET FONT ALWAYS BEFORE CURSOR, else y shift down
    tft.setFont(&NotoSans_Regular11pt7b);
    for (int i = 0; i < 5; i++) {
    tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);

    tft.setCursor(cx + 10, cy + 26);
    if (i == 0)
      tft.println("AF-Lock");
    else if (i == 1)
      tft.println("Cool Feature");
    else if (i == 2)
      tft.println("L-Sensitivity");
    else if (i == 3)
      tft.println("Mag-Sensitivity");
    else if (i == 4)
      tft.println("Auto pwr-off");

    tft.setCursor(cx + w - 10 - (20 * 2), cy + 26);
    if (i == 0)
      tft.print("ON");
    else if (i == 1)
      tft.print("ON");
    else if (i == 2)
      tft.print("1");
    else if (i == 3)
      tft.print("3");
    else if (i == 4)
      tft.print("OFF");

    cy += h + 10;
    } */
  for (uint8_t i = 0; i < sizeof(options) / sizeof(Option); i++) {
    options[i].draw();
  }
  int   cx = 0,
        cy = 35,
        w = 50,
        h = 50;
  for (int i = 0; i < 3; i++) {
    if (i == 0)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_YELLOW);
    else if (i == 1)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_RED);
    else if (i == 2)
      tft.fillRoundRect(cx, cy, w, h, 4, COL_GREEN);
    cy += h + 10;
  }
}

void CustomFont() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = tft.height() / 2 - 1;
  tft.setCursor(cx, cy);
  tft.setTextColor(0xB1DA);  tft.setTextSize(1);
  tft.setFont(&NotoSans_Regular15pt7b);
  tft.println("Hello World!");
}
