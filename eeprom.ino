//=====================================================================
// eeprom.ino
//
//  Created on: 6th January 2012
//      Author: Mike McPherson
//=====================================================================

#include <EEPROM.h>

// offsets into EEPROM
//====================
#define eeSERVER     0
#define eeUSERNAME  32

#define eePASSWORD  64
#define eeFTPPATH   96
#define eeWXID      128

//==============================================================
// "globals"
//==============================================================
extern uint8_t gServer[33];
extern uint8_t gUserName[33];
extern uint8_t gPassword[33];
extern uint8_t gFTPpath[33];
extern uint8_t gWxId[33];

// module to save and restore settings 
// settings are stored in EEPROM
//====================================

void eepromDump(void)
{
  int row;
  int col;
  byte oneByte;

  // read entire eeprom
  //===================
  for (row=0; row<16; row++)
  {
    Serial.print("0");
    Serial.print(row, HEX);
    Serial.print(":");
    for (col=0; col<16; col++)
    {
      oneByte = EEPROM.read(row*16+col);
      Serial.print(" ");
      if (oneByte < 16)
        Serial.print("0");
      Serial.print(oneByte, HEX);
    }
    Serial.println();
  }
  Serial.println();
}

void eepromRead(uint8_t *target, int offset, int size)
{
  int i;

  // read value from address
  //========================
  for (i=0; i<size; i++)
  {
    target[i] = EEPROM.read(offset + i);
  }
  target[size] = '/0';
}

void eepromReadAll(void)
{
  // read saved settings from EEPROM
  //================================
  eepromRead(&gServer[0], eeSERVER, 32);
  eepromRead(&gUserName[0], eeUSERNAME, 32);
  eepromRead(&gPassword[0], eePASSWORD, 32);
  eepromRead(&gFTPpath[0], eeFTPPATH, 32);
  eepromRead(&gWxId[0], eeWXID, 32);
}

void eepromSaveAll(void)
{
  // save all settings
  //==================
  eepromWrite(gServer, eeSERVER, 32);
  eepromWrite(gUserName, eeUSERNAME, 32);
  eepromWrite(gPassword, eePASSWORD, 32);
  eepromWrite(gFTPpath, eeFTPPATH, 32);
  eepromWrite(gWxId, eeWXID, 32);
}

void eepromWrite(uint8_t *target, int offset, int size)
{
  int i;

  // write value to next location
  //=============================
  for (i=0; i<size; i++)
  {
    // write one byte
    //===============
    EEPROM.write(offset + i, target[i]);
  }
}

void eepromWriteByte(int address, uint8_t data)
{
  EEPROM.write(address, data);
}


