
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
//#include <SdFat.h>                // SD card & FAT filesystem library
//#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
//#include <Adafruit_ImageReader.h> // Image-reading functions

#include "SerialCom.h"

#include <Fonts/NotoSans_Regular15pt7b.h>
#include <Fonts/NotoSans_Regular11pt7b.h>
#include <Fonts/NotoSans_Regular10pt7b.h>
#include <Fonts/NotoSans_Regular9pt7b.h>
#include <Fonts/NotoSans_Regular6pt7b.h>
#include <Fonts/NotoSans_Regular5pt7b.h>
#include <Fonts/font_AwesomeF000.h>

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
  Serial.println("ILI9341 Bitmap Test!");

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


  /*
  tft.setTextColor(COL_TXT);
  tft.setFont(&font_AwesomeF000);
  tft.setCursor(0, 12);
  for (int i=0; i < 128; i++) {
    if (i == 10) continue;
    tft.print((char)i);
  }*/

  tft.fillScreen(ILI9341_BLACK);
  int         cx = 60,
              cy = tft.height() / 2 - 1;
  tft.setCursor(cx, cy);
  tft.setTextColor(0xB1DA);  tft.setTextSize(1);
  tft.setFont(&NotoSans_Regular15pt7b);
  tft.println("Hello World!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
