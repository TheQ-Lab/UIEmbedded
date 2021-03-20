#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//stm nucleo st476
//titel no outline

#include "SerialCom.h"

#include <Fonts/NotoSans_Regular15pt7b.h>
#include <Fonts/NotoSans_Regular11pt7b.h>
#include <Fonts/NotoSans_Regular10pt7b.h>
#include <Fonts/NotoSans_Regular9pt7b.h>
#include <Fonts/NotoSans_Regular6pt7b.h>
#include <Fonts/NotoSans_Regular5pt7b.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 22
#define TFT_CS 10

#define COL_BG      0x5248
#define COL_TXT     0xFFFF
#define COL_YELLOW  0xDE64
#define COL_RED     0xD927
#define COL_GREEN   0x7EC4
#define COL_BLUE    0x2B1D

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("ILI9341 Showreel!");

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

  Button8();
}

void loop() {
  String cmd = serialProcess();

  if (cmd == "cfont")
    CustomFont();
  else if (cmd == "stdfont")
    StandardFont();
  else if (cmd == "b1")
    Button1();
  else if (cmd == "b2")
    Button2();
  else if (cmd == "b3")
    Button3();
  else if (cmd == "b4")
    Button4();
  else if (cmd == "b5")
    Button5();
  else if (cmd == "b6")
    Button6();
  else if (cmd == "b7")
    Button7();
  else if (cmd == "b8")
    Button8();
  else if (cmd == "bs1")
    ButtonS1();
  else if (cmd == "bs2")
    ButtonS2();
  else if (cmd == "bs3")
    ButtonS3();
  else if (cmd == "bs4")
    ButtonS4();
  else if (cmd == "bs5")
    ButtonS5();
  else if (cmd == "bs6")
    ButtonS6();
  else if (cmd == "clr")
    tft.fillScreen(ILI9341_BLACK);
  else if (cmd == "dither") {
    bool draw = true;
    for (short x=0; x < 320; x++) {
      for (short y=0; y < 240; y++) {
        if (draw)
          tft.drawPixel(x, y, COL_BG);
        draw = !draw;
      }
      //to make it uneaven every time, from line vert. to dotted
      //draw = !draw;
    }
  }

}

void Button1() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = 5;
  while (cy < 200) {
    tft.fillRoundRect(cx, cy, 250, 50, 4, COL_BG);
    cy += 60;
  }
}

void Button2() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = 5,
              w = 250,
              h = 38;
  while (cy < 200) {
    tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);
    cy += h + 10;
  }
}

void Button3() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = 5,
              w = 250,
              h = 38;
  while (cy < 200) {
    tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);
    cy += h + 10;
  }
  cx = 0,
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

void Button4() {
  tft.fillScreen(ILI9341_BLACK);
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
  }
  cx = 0,
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

void Button5() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = 5,
              w = 250,
              h = 38;
  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT ALWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);
  for (int i = 0; i < 5; i++) {
    if (i == 0) {
      tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);
      tft.setCursor(cx + 10, cy + 26);
      tft.println("Toller Titel");
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
  cx = 0,
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

void Button6() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 50,
              cy = 5,
              w = 220,
              h = 38;
  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT ALWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);
  for (int i = 0; i < 5; i++) {
    if (i == 0) {
      tft.drawRoundRect(cx, cy, w+45, h, 4, COL_BG);
      tft.setCursor(cx + 10, cy + 26);
      tft.println("Toller Titel");
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
  //nav right Page
                cx = cx + w + 10,
                cy = 100,
                w = 35,
                h = 35;
  tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);
  tft.drawRoundRect(cx, cy+50, w, h, 4, COL_BG);

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

void Button7() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 50,
              cy = 5,
              w = 220,
              h = 38;
  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT ALWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);
  for (int i = 0; i < 5; i++) {
    if (i == 0) {
      tft.drawRoundRect(cx, cy, w+45, h, 4, COL_BG);
      tft.setCursor(cx + 10, cy + 26);
      tft.println("Toller Titel");
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
  
  //nav right Page
                cx = cx + w + 10,
                cy = 70,
                w = 35,
                h = 35;
  tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);
  tft.drawRoundRect(cx, cy+50+60, w, h, 4, COL_BG);
  tft.setCursor(cx+10, cy+70);
  tft.print("1");
  cy += 2;
  tft.drawLine(cx, cy+70+5, cx+w, cy+70+5, COL_TXT);
  tft.setFont(&NotoSans_Regular6pt7b);
  tft.setCursor(cx+10, cy+70+20);
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

void Button8() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 50,
              cy = 5,
              w = 220,
              h = 38;
  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT ALWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);
  for (int i = 0; i < 5; i++) {
    if (i == 0) {
      tft.drawRoundRect(cx, cy, w+45, h, 4, COL_BG);
      tft.setCursor(cx + 10, cy + 26);
      tft.println("Toller Titel");
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
  
  //nav right Page
                cx = cx + w + 10,
                cy = 70,
                w = 35,
                h = 35;
  tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);
  tft.drawRoundRect(cx, cy+50+60, w, h, 4, COL_BG);
  tft.setCursor(cx+10, cy+70-1);
  tft.print("1");
  cy += 5;
  tft.drawLine(cx+w-10-17, cy+70+15, cx+w-17, cy+70+5, COL_TXT);
  tft.setFont(&NotoSans_Regular6pt7b);
  tft.setCursor(cx+14, cy+70+20);
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

//------------------------------------------------------------------------------------------------

void ButtonS1() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 35,
              cy = 95;
  tft.fillRoundRect(cx, cy, 250, 50, 4, COL_BG);

  tft.setCursor(cx + 5, cy + 35);
  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  tft.setFont(&NotoSans_Regular15pt7b);
  tft.println("Menuepunkt");
}

void ButtonS2() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 35,
              cy = 101,
              w = 250,
              h = 38;
  tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  tft.setFont(&NotoSans_Regular11pt7b);
  tft.setCursor(cx + 10, cy + 26);
  tft.println("Menuepunkt");
}

void ButtonS3() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 35,
              cy = 101,
              w = 250,
              h = 38;
  tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT LAWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);

  tft.setCursor(cx + 10, cy + 26);
  tft.println("AF-Lock");
  tft.setCursor(cx + w - 10 - (20 * 2), cy + 26);
  tft.print("ON");
}

void ButtonS4() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 35,
              cy = 105,
              w = 250,
              h = 30;
  tft.fillRoundRect(cx, cy, w, h, 4, COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT LAWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular10pt7b);

  tft.setCursor(cx + 10, cy + 22);
  tft.println("AF-Lock");
  tft.setCursor(cx + w - 10 - (20 * 2), cy + 22);
  tft.print("ON");
}

void ButtonS5() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 35,
              cy = 101,
              w = 250,
              h = 38;
  tft.drawRoundRect(cx, cy, w, h, 4, COL_BG);

  tft.setTextColor(COL_TXT);  tft.setTextSize(1);
  //SET FONT LAWAYS BEFORE CURSOR, else y shift down
  tft.setFont(&NotoSans_Regular11pt7b);

  tft.setCursor(cx + 10, cy + 26);
  tft.println("Text-Feld? Dark Mode");
  tft.setCursor(cx + w - 10 - (20 * 2), cy + 26);
  //tft.print("ON");
}

void ButtonS6() {
  tft.fillScreen(ILI9341_BLACK);
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


void StandardFont() {
  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = tft.height() / 2 - 1;
  tft.setCursor(cx, cy);
  tft.setTextColor(0xB1DA);  tft.setTextSize(2);
  tft.setFont();
  tft.println("Hello World!");
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
