/* EasyCODE V6.8 05.10.2004 21:56:40 */
/* EasyCODE O
If=horizontal
LevelNumbers=no
LineNumbers=no
Colors=16777215,0,12582912,12632256,0,0,0,16711680,8388736,0,33023,32768,0,0,0,0,0,32768,12632256,255,65280,255,255,16711935
ScreenFont=Courier New,Standard,80,4,-11,0,400,0,0,0,0,0,0,3,2,1,49,96,96
PrinterFont=Courier New,,110,4,-93,0,400,0,0,0,0,0,0,3,2,1,49,600,600
LastLevelId=2 */
/* EasyCODE ( 1
   IntelHex.h */
#ifndef IntelHexH
#define IntelHexH
/* EasyCODE C */
class IntelHex
   {
   private:
      int        __fastcall EvaluateHexLn(AnsiString,int);
      char       __fastcall cHexToChar(char);
      int        __fastcall HexLnToCharLn(char *, char *, int);
      AnsiString __fastcall CharLnToHexLn(char *, int);
      int        __fastcall WriteCommon(AnsiString,unsigned long,unsigned char,unsigned char *);
      
      char *_cMem;
      unsigned long _ulMemSize;
      unsigned long _ulExtSegAdr;
      unsigned long _ulExtLinAdr;
      unsigned int  _uiAdrOffset;
      unsigned char _ucRecordType;
      unsigned char _ucDataLength;
      unsigned int  _uiFindLastLine;
      unsigned char _cLine[40];
   public:
      IntelHex(void);
      int __fastcall Read(AnsiString, char *, unsigned long);
      int __fastcall Write(AnsiString, unsigned long, unsigned char,  unsigned char);
      int __fastcall Write(AnsiString, unsigned long, unsigned short, unsigned char);
      int __fastcall Write(AnsiString, unsigned long, unsigned long,  unsigned char);
      /* EasyCODE - */
      unsigned long _ulHighAdr;
   };
/* EasyCODE E */
// Rückmeldungen der Funktion Read() und Write()
#define INTHEX_OK                 0
#define INTHEX_SYNTAX_ERR         1
#define INTHEX_CHECKSUM_ERR       2
#define INTHEX_LENGTH_ERR         3
#define INTHEX_TXT_AFTER_END_ERR  4
#define INTHEX_MEM_ERR            5
#define INTHEX_FILE_NOT_FOUND     6
/* EasyCODE - */
// Reihenfolge, wie Words abgespeichert werden
#define INTHEX_L_H  0 // Low-Byte  an Low-Adress
#define INTHEX_H_L  1 // High-Byte an Low-Adress
/* EasyCODE - */
#endif
/* EasyCODE ) */
