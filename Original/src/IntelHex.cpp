/* EasyCODE V8 */
/* EasyCODE ( 0 */
#include <vcl.h>
#include <fstream.h>
#include "AddChecksum.h"
#include "IntelHex.h"
#pragma hdrstop
#pragma package(smart_init)
/* EasyCODE - */
#define DATA_LINE   0
#define END_OF_FILE 1
#define EXT_SEG_ADR 2
#define EXT_LIN_ADR 4
/* EasyCODE - */
// interne Funktionen
/* EasyCODE F */
char __fastcall IntelHex::cHexToChar(char cValue)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::cHexToChar() */
    /*
       Wandelt eine ASCII-HexCharacter in char um.
       Bei einem Fehlerhaften Character wur -1 zurckgegeben.
    */
    /* EasyCODE - */
    // Kleinbuchstaben in Groábuchstaben wandeln
    cValue = toupper(cValue);
    /* EasyCODE - */
    cValue -= '0';
    if ( cValue > 9 )
    {
        if ( cValue >= 'A'-'0' && cValue <= 'F'-'0' )
        {
            cValue -= ('A'-'0'-10);
        }
        else
        {
            cValue = -1; // Fehler
        }
    }
    return cValue;
    /* EasyCODE ) */
}

/* EasyCODE F */
int __fastcall IntelHex::HexLnToCharLn(char *cSource, char *cDest, int iLength)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::HexLnToIntLn() */
    /*
       Eine kompette Zeile mit der L„nge iLength-Bytes von ASCII-Hex nach char konvertieren.
       Bei unglltigen Zeichen wird die Routine mit false verlassen. 
    */
    /* EasyCODE - */
    int i;
    int iResult = true;
    char cHigh,cLow;
    for ( i=0;i<iLength;i++ )
    {
        // n„chstes Byte holen
        cHigh = *cSource++;
        cLow  = *cSource++;
        if ( // auf gltige Zeichen prfen
             !isxdigit(cHigh) || !isxdigit(cLow) )
        {
            // Zeichen fehlerhaft - Abbruch
            iResult = false;
            break;
        }
        else
        {
            // Byte wandeln und abspeichern
            *cDest++ = (cHexToChar(cHigh) << 4) + cHexToChar(cLow);
        }
    }
    return (iResult);
    /* EasyCODE ) */
}

/* EasyCODE F */
AnsiString __fastcall IntelHex::CharLnToHexLn(char *cSource, int iLength)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::CharLnToHexLn() */
    /*
       Eine kompette Zeile mit der L„nge iLength-Bytes von char nach ASCII-Hex.
    */
    /* EasyCODE - */
    int i;
    AnsiString line;
    char cHigh,cLow;
    for ( // Komplette Zeile auf gltige Zeichen prfen
          i=0;i<iLength;i++ )
    {
        line += IntToHex((unsigned char)*cSource++,2);
    }
    return line;
    /* EasyCODE ) */
}

/* EasyCODE F */
int __fastcall IntelHex::EvaluateHexLn(AnsiString HexLine, int iStore)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::EvaluateHexLn() */
    /*
      Eine Intel-Hex-Zeile auswerten und ggf. abspeichern.
      Falls die Zeile Fehleraufweist
    */
    /* EasyCODE - */
    int iResult = INTHEX_OK;
    // char c;
    int iLength;
    AddChecksum Checksum;
    unsigned long ulHighAdr;
    /* EasyCODE - */
    iLength = HexLine.Length();
    if ( /* Eine Zeile muá aus mindestens 11 Zeichen bestehen,
            die Anzahl der Zeichen muá ungerade sein und die
            Zeile muá mit ':' anfangen */
         (iLength >= 11) &&
         (iLength % 2)  &&
         (HexLine[1] == ':') )
    {
        // Die Zeile in char wandlen 
        HexLnToCharLn(&HexLine[2],_cLine,iLength/2);
        /* EasyCODE - */
        // Datenl„nge und Recordtype auslesen
        _ucDataLength = _cLine[0];
        _ucRecordType = _cLine[3];
        if ( // Checksumme ok ?
             Checksum.Add(_cLine,_cLine+iLength/2-1) == 0 )
        {
            /* EasyCODE ( 0 
            Record auswerten */
            switch ( // Recordtype auswerten
                     _ucRecordType )
            {
            case DATA_LINE // Daten eintragen
            :
                if ( _ucDataLength == iLength/2 - 5 )
                {
                    // Adressoffset abspeichern
                    _uiAdrOffset = ((unsigned int)_cLine[1] << 8) + _cLine[2];
                    /* EasyCODE - */
                    // H”chste Adresse ermitteln
                    ulHighAdr = _ulExtLinAdr + _ulExtSegAdr + _uiAdrOffset + _ucDataLength - 1;
                    if ( ulHighAdr > _ulHighAdr )
                    {
                        _ulHighAdr = ulHighAdr;
                    }
                    /* EasyCODE ( 0 
                    Zeile abspeichern */
                    if ( // soll abgespeichert werden?
                         iStore )
                    {
                        if ( // Ist der Speicherplatz ausreichend ?
                             ulHighAdr < _ulMemSize )
                        {
                            memcpy(_cMem + _ulExtLinAdr + _ulExtSegAdr + _uiAdrOffset,&_cLine[4],_ucDataLength);
                        }
                        else
                        {
                            iResult = INTHEX_MEM_ERR;
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    iResult = INTHEX_LENGTH_ERR;
                }
                break;
            case END_OF_FILE // End of File
            :
                _uiFindLastLine = true;
                if ( _ucDataLength != 0 )
                {
                    iResult = INTHEX_LENGTH_ERR;
                }
                break;
            case EXT_SEG_ADR // Extended Segment Address Record
            :
                _ulExtSegAdr = (((unsigned int)_cLine[4] << 8) + _cLine[5]) << 4;
                if ( _ucDataLength != 2 )
                {
                    iResult = INTHEX_LENGTH_ERR;
                }
                break;
            case EXT_LIN_ADR // Extended Linear Address Record
            :
                _ulExtLinAdr = (((unsigned int)_cLine[4] << 8) + _cLine[5]) << 16;
                if ( _ucDataLength != 2 )
                {
                    iResult = INTHEX_LENGTH_ERR;
                }
                break;
            default:
                break;
            }
            if ( // Fehler falls die letzte Zeile schon gefunden wurde
                 _ucRecordType != 1 && _uiFindLastLine )
            {
                iResult = INTHEX_TXT_AFTER_END_ERR;
            }
            /* EasyCODE ) */
        }
        else
        {
            iResult = INTHEX_CHECKSUM_ERR;
        }
    }
    else
    {
        iResult = INTHEX_SYNTAX_ERR;
    }
    return iResult;
    /* EasyCODE ) */
}

/* EasyCODE F */
/* TODO : 
Alle Adressen auf groesser Flashsize untersuchen und Gap muî innerhalb der Min max-WErte lliegen.
Falls Adresse beim Schreiben nicht gefunden wrid, muî Wert an die Datei angehängt werden. */
int __fastcall IntelHex::WriteCommon(AnsiString Filename,unsigned long ulAdr, unsigned char ucLength, unsigned char *pucValue)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::WriteCommon */
    unsigned long ulLineAdr;
    int iResult;
    int iPos = 0; // Lesepointer auf den Anfang setzen
    string sLine;
    AddChecksum Checksum;
    AnsiString line;
    int iFound;
    char cCheck;
    int i;
    /* EasyCODE - */
    _ulExtSegAdr = 0;
    _ulExtLinAdr = 0;
    _ulHighAdr = 0;
    _uiFindLastLine = false;
    /* EasyCODE - */
    // File zum Lesen und Schreiben öffenen
    fstream file(Filename.c_str(),ios_base::in|ios_base::out);
    /* EasyCODE - */
    // Abfragen, ob Datei zum Schreiben geöffnet werden kann
    while ( // Solange noch eine Zeile vorhanden ist und noch nicht alles gespeichert wurde
            getline(file,sLine) && (ucLength) )
    {
        if ( // Zeile auswerten
             (iResult = EvaluateHexLn(sLine.c_str(),false)) == INTHEX_OK )
        {
            if ( _ucRecordType == DATA_LINE )
            {
                // Anfangsadresse der Zeile bestimmen
                ulLineAdr = _ulExtLinAdr + _ulExtSegAdr + _uiAdrOffset;
                /* EasyCODE - */
                iFound = false;
                while ( ucLength )
                {
                    if ( // Ist die gewünschte Adresse in der Zeile
                         (ulAdr >= ulLineAdr) && (ulAdr < (ulLineAdr + (unsigned long)_ucDataLength)) )
                    {
                        ucLength--;
                        /* EasyCODE - */
                        // Ein Byte speichern
                        _cLine[ulAdr - ulLineAdr + 4] = *(pucValue + ucLength);
                        /* EasyCODE - */
                        // nächste Adresse selektieren
                        ulAdr++;
                        /* EasyCODE - */
                        iFound = true;
                    }
                    else
                    {
                        /* Adresse nicht
                           mehr in der Zeile */
                        break;
                    }
                }
                if ( iFound )
                {
                    // Checksumme über die Zeile ermittlen und anhängen
                    Checksum.ulSum = 0;
                    /* EasyCODE - */
                    Checksum.Add((unsigned char *)_cLine,(unsigned char *)(_cLine+_ucDataLength+3));
                    /* EasyCODE - */
                    _cLine[_ucDataLength+4] = (unsigned char)(0x100 - Checksum.ulSum);
                    /* EasyCODE - */
                    // Zeile in Hex wandeln
                    line = ":" + CharLnToHexLn(_cLine,_ucDataLength+5);
                    /* EasyCODE - */
                    // Abspeichern an die Position vor dem letzen Lesen der letzten Zeile
                    file.seekp(iPos); // ios_base::beg- Funktioniert bie Builder C++ 6 nicht
                    /* EasyCODE - */
                    iFound = false;
                    for ( i=0; i<10; i++ )
                    {
                        file.seekp(iPos-i);
                        /* EasyCODE - */
                        file.read(&cCheck,1);
                        if ( cCheck == ':' )
                        {
                            iFound = true;
                            /* EasyCODE - */
                            file.seekp(iPos-i);
                            break;
                        }
                    }
                    if ( iFound )
                    {
                        file.write(line.c_str(),2*(_ucDataLength+5)+1);
                    }
                }
            }
            else
            {
                /* Record = Letzte Zeile
                  Letzt Zeile abschneiden
                  Rest abpeichern
                  letzte Zeile wieder anfügen
                  evtl vorher SegAdr neu setzen
                */
            }
            // Lesepointer auslesen
            iPos = file.tellg();
        }
    }
    file.close();
    return // Returncode anpassen
           false;
    /* EasyCODE ) */
}

// Schnittstellenfunktionen
/* EasyCODE F */
IntelHex::IntelHex(void)
{
    // Konstruktor IntelHex::IntelHex()
}

/* EasyCODE F */
int __fastcall IntelHex::Read(AnsiString Filename, char *pucStart, unsigned long ulLength)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::Read() */
    int iResult = !INTHEX_OK;
    string sLine;
    /* EasyCODE - */
    _cMem = pucStart;
    _ulMemSize = ulLength;
    /* EasyCODE - */
    _ulExtSegAdr = 0;
    _ulExtLinAdr = 0;
    _ulHighAdr = 0;
    _uiFindLastLine = false;
    /* EasyCODE - */
    ifstream file(Filename.c_str());
    while ( getline(file,sLine) )
    {
        if ( (iResult = EvaluateHexLn(sLine.c_str(),true)) != INTHEX_OK )
        {
            break;
        }
    }
    file.close();
    return /* EasyCODE < */
    //Returncode erweitern
           iResult;
    /* EasyCODE > */
    ;
    /* EasyCODE ) */
}

/* EasyCODE F */
int __fastcall IntelHex::Write(AnsiString Filename,unsigned long ulAdr, unsigned char ucValue, unsigned char ucHighByteFirst)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::Write() CHAR */
    return WriteCommon(Filename,ulAdr,1,&ucValue);
    /* EasyCODE ) */
}

/* EasyCODE F */
int __fastcall IntelHex::Write(AnsiString Filename,unsigned long ulAdr, unsigned short usValue, unsigned char ucHighByteFirst)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::Write() WORD */
    unsigned char ucValue[2];
    if ( ucHighByteFirst == INTHEX_H_L )
    {
        ucValue[1] = usValue / 256;
        ucValue[0] = usValue & 0xff;
    }
    else
    {
        ucValue[0] = usValue / 256;
        ucValue[1] = usValue & 0xff;
    }
    return WriteCommon(Filename,ulAdr,2,ucValue);
    /* EasyCODE ) */
}

/* EasyCODE F */
int __fastcall IntelHex::Write(AnsiString Filename,unsigned long ulAdr, unsigned long ulValue, unsigned char ucHighByteFirst)
{
    /* EasyCODE ( 0 
    Funktion IntelHex::Write() LONG */
    unsigned char ucValue[4];
    if ( ucHighByteFirst == INTHEX_H_L )
    {
        ucValue[3] = (ulValue / 0x1000000) &0xff;
        ucValue[2] = (ulValue / 0x10000) &0xff;
        ucValue[1] = (ulValue / 0x100) & 0xff;
        ucValue[0] = ulValue & 0xff;
    }
    else
    {
        ucValue[0] = (ulValue / 0x1000000) &0xff;
        ucValue[1] = (ulValue / 0x10000) &0xff;
        ucValue[2] = (ulValue / 0x100) & 0xff;
        ucValue[3] = ulValue & 0xff;
    }
    return WriteCommon(Filename,ulAdr,4,ucValue);
    /* EasyCODE ) */
}

/* EasyCODE ) */
