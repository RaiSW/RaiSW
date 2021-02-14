#ifndef HexCRC_mainH
#define HexCRC_mainH
//---------------------------------------------------------------------------
typedef struct {IntelHex  src;
                unsigned long  ulPointer;
                unsigned short usCRC16;
                unsigned long  ulADD16;
               } tCheckData;
typedef tCheckData * tpCheckData;

bool iGetFileAndBuildCRC(AnsiString fileSrc, bool bQuiet, tpCheckData pCheckData);
void writeCRC(AnsiString fileSrc, bool bQuiet, tpCheckData pCheckData);
void writeHelp(void);

#endif
