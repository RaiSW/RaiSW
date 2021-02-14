#pragma once

#include <string>
using namespace std;

class IntelHex
{
private:
    int    EvaluateHexLn(string, int);
    char   cHexToChar(char);
    int    HexLnToCharLn(char*, char*, int);
    string CharLnToHexLn(char*, int);
    int    WriteCommon(string, unsigned long, unsigned char, unsigned char*);

    char* _cMem;
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
    int Read(string, char*, unsigned long);
    int Write(string, unsigned long, unsigned char, unsigned char);
    int Write(string, unsigned long, unsigned short, unsigned char);
    int Write(string, unsigned long, unsigned long, unsigned char);

    unsigned long _ulHighAdr;
};

// Rückmeldungen der Funktion Read() und Write()
#define INTHEX_OK                 0
#define INTHEX_SYNTAX_ERR         1
#define INTHEX_CHECKSUM_ERR       2
#define INTHEX_LENGTH_ERR         3
#define INTHEX_TXT_AFTER_END_ERR  4
#define INTHEX_MEM_ERR            5
#define INTHEX_FILE_NOT_FOUND     6

// Reihenfolge, wie Words abgespeichert werden
#define INTHEX_L_H  0 // Low-Byte  an Low-Adress
#define INTHEX_H_L  1 // High-Byte an Low-Adress