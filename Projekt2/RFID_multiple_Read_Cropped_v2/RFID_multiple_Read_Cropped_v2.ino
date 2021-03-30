/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>

#define N_NFC           2
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN01        17          // Configurable, see typical pin layout above
#define SS_PIN02        24          // Configurable, see typical pin layout above

const byte NFCSensorPins[N_NFC] = {
  17,
  24
};
MFRC522 NFCSensors[N_NFC];
MFRC522 NFC1(SS_PIN01, RST_PIN);   // Create MFRC522 instance
MFRC522 NFC2(SS_PIN02, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  
  for(byte i=0; i<N_NFC; i++) {
    NFCSensors[i] = MFRC522(NFCSensorPins[i], RST_PIN);         // Create MFRC522 instance
    NFCSensors[i].PCD_Init();                                   // Init MFRC522 card
    NFCSensors[i].PCD_AntennaOff();
  }
  
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}

//*****************************************************************************************//
void loop() {

  for(byte i=0; i<N_NFC; i++) {
    unsigned long startTime = micros();
    NFCSensors[i].PCD_AntennaOn();
    if (NFCSensors[i].PICC_IsNewCardPrese) {
      if (NFCSensors[i].PICC_ReadCardSerial()) {
        ReadNFC(NFCSensors[i]);
      } else {
        Serial.print("noRead ");
      }
    } else {
      Serial.print("noPresence ");
    }
    NFCSensors[i].PCD_AntennaOff();
    startTime = micros() - startTime;
    Serial.print(startTime);
  }
  
  //-------------------------------------------

  Serial.println("");
}
//*****************************************************************************************//

unsigned long ReadNFC(MFRC522 NFCSensor) {
  unsigned long startTime = micros();
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------
  
//    Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  //NFCSensor.PICC_DumpDetailsToSerial(&(NFCSensor.uid)); //dump some details about the card

  //NFCSensor.PICC_DumpToSerial(&(NFCSensor.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------

  //Serial.print(F("Name: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  //------------------------------------------- GET FIRST NAME
  status = NFCSensor.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(NFCSensor.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(NFCSensor.GetStatusCodeName(status));
    return;
  }

  status = NFCSensor.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(NFCSensor.GetStatusCodeName(status));
    NFCSensor.PICC_HaltA();
    NFCSensor.PCD_StopCrypto1();
    NFCSensor.PCD_Init();
    return;
  }

  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");

  //---------------------------------------- GET LAST NAME

  byte buffer2[18];
  block = 1;

  status = NFCSensor.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(NFCSensor.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(NFCSensor.GetStatusCodeName(status));
    //NFCSensor.PCD_Reset();
    return;
  }

  status = NFCSensor.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(NFCSensor.GetStatusCodeName(status));
    //"TIMEOUT in Communication" in Reading
    //-> Softlock, not resolved by PCD_Resest()
    //"Reading failed: The CRC_A does not match." in Reading
    //-> Softlock
    NFCSensor.PICC_HaltA();
    NFCSensor.PCD_StopCrypto1();
    NFCSensor.PCD_Init();
    //NFCSensor.PICC_HaltA();
    return;
  }

  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
  }
  Serial.print("");

  //----------------------------------------

//  Serial.println(F("\n**End Reading**\n"));

  //delay(1000); //change value if you want to read cards faster

  NFCSensor.PICC_HaltA();
  NFCSensor.PCD_StopCrypto1();

  return micros()-startTime;
}
