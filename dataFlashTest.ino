//==============================================================
//  dataFlashTest
//  Mike McPherson
//  November 2013
//==============================================================

//==============================================================
// serial port
//==============================================================
//#define DIAGPORT_RX     4
//#define DIAGPORT_TX     5
//#include <SoftwareSerial.h>
//SoftwareSerial diagport(DIAGPORT_RX, DIAGPORT_TX);

//==============================================================
// "globals"
//==============================================================
uint8_t gServer[33] = "wxftp.mmcpix.com";
uint8_t gUserName[33] = "wxd4t4";
uint8_t gPassword[33] = "w34th3r";
uint8_t gFTPpath[33] = "/";
uint8_t gWxId[33] = "Holmbeck";

//==============================================================
// data flash
//==============================================================
#define DF_SELECT   10
#define DF_MOSI     11
#define DF_MISO     12
#define DF_SPICLOCK 13
#include <Sodaq_dataflash.h>

//==============================================================
// buffer for data read
//==============================================================
#include "DataRecord.h"
wxDataRecord dataRecord;
uint8_t pageBuffer[512];
#define PAGES 3

void setup(void)
{
  flashInit();
  Serial.begin(19200);
  Serial.println("dataFlashTest v1.01");
  
  //flashFill();
}

void loop(void)
{
  checkCommands();
}

//==============================================================
// clear entire flash chip
//==============================================================
void flashClear(void)
{
  Serial.print("chip erase...");
  dflash.chipErase();
  Serial.println("completed");
}

//==============================================================
// hex dump one page
//==============================================================
void flashDump(int page)
{
  int line;
  int col;
  int i;
  int data;
  
  Serial.print("dump page ");
  if (page<16)
    Serial.print("0");
  Serial.println(page, HEX);
  
  // read page from flash
  //=====================
  flashRead(page);
  
  for (line=0; line<32; line++)
  {
    if (line<16)
      Serial.print("0");
    Serial.print(line, HEX);
    Serial.print(": ");
    for (col=0; col<16; col++)
    {
      i = line*16+col;
      data = pageBuffer[i];
      if (data < 16)
        Serial.print("0");
      Serial.print(data, HEX);
      Serial.print(" ");       
    }
    Serial.println();
  }
  Serial.println();
}

//==============================================================
// erase one page of flash
//==============================================================
void flashErase(int page)
{
  // erase a page of flash
  //======================
  dflash.pageErase(page);
}

//==============================================================
// fill one page with "data"
//==============================================================
void flashFill(int page, uint8_t data)
{
  // initalise flash by writing data to all bytes
  //=============================================
  int i;
  
  Serial.print("fill page ");
  if (page<16)
    Serial.print("0");
  Serial.print(page, HEX);
  Serial.print(" with: ");
  Serial.println(data);
  
 // first erase the page
  //=====================
  flashErase(page);
  
  // fill flash internal buffer with data
  //=====================================
  for(i=0; i<512; i++)
    flashWriteByte(i, data);

  // flush flash internal buffer to flash
  //=====================================
  flashFlushPage(page);
}

//==============================================================
// flush one page from its internal buffer to flash
//==============================================================
void flashFlushPage(int page)
{
  // write a page from internal buffer to flash
  //===========================================
  dflash.writeBuf1ToPage(page);
}

//==============================================================
// initialise communication with flash chip
//==============================================================
void flashInit(void)
{
  dflash.init(DF_MISO, DF_MOSI, DF_SPICLOCK, DF_SELECT);
}

//==============================================================
// read one page from flash to RAM
//==============================================================
void flashRead(int page)
{
  // read a page from flash into local memory
  //=========================================
  readPage(page, &pageBuffer[0], sizeof(pageBuffer));
}

//==============================================================
// write one byte to flash internal buffer
//==============================================================
void flashWriteByte(int address, uint8_t data)
{
  // write one byte of data to flash internal buffer
  //================================================
  dflash.writeByteBuf1(address, data);
}

