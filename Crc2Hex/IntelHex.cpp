#include "IntelHex.h"

#define DATA_LINE   0
#define END_OF_FILE 1
#define EXT_SEG_ADR 2
#define EXT_LIN_ADR 4

char IntelHex::cHexToChar(char cValue)
{
    /* Funktion IntelHex::cHexToChar() */
    /*
       Wandelt eine ASCII-HexCharacter in char um.
       Bei einem fehlerhaften Character wird -1 zurückgegeben.
    */
    
    // Kleinbuchstaben in Großbuchstaben wandeln
    cValue = toupper(cValue);
    
    cValue -= '0';
    if (cValue > 9)
    {
        if (cValue >= 'A' - '0' && cValue <= 'F' - '0')
        {
            cValue -= ('A' - '0' - 10);
        }
        else
        {
            cValue = -1; // Fehler
        }
    }
    return cValue;
}

int IntelHex::HexLnToCharLn(char* cSource, char* cDest, int iLength)
{
    /*  Funktion IntelHex::HexLnToIntLn() */
    /*
       Eine kompette Zeile mit der Länge iLength-Bytes von ASCII-Hex nach char konvertieren.
       Bei unglültigen Zeichen wird die Routine mit false verlassen.
    */
    
    int i;
    int iResult = true;
    char cHigh, cLow;
    for (i = 0; i < iLength; i++)
    {
        // nächstes Byte holen
        cHigh = *cSource++;
        cLow = *cSource++;
        if ( // auf gültige Zeichen prüfen
            !isxdigit(cHigh) || !isxdigit(cLow))
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
}

string IntelHex::CharLnToHexLn(char* cSource, int iLength)
{
    /* Funktion IntelHex::CharLnToHexLn() */
    /*
       Eine kompette Zeile mit der Länge iLength-Bytes von char nach ASCII-Hex.
    */
    
    int i;
    string line;
    char cHigh, cLow;
    for (i = 0; i < iLength; i++) // Komplette Zeile auf gültige Zeichen prüfen
    {
  //todo      line += IntToHex((unsigned char)*cSource++, 2);
    }
    return line;
 }

int IntelHex::EvaluateHexLn(string HexLine, int iStore)
{
    /* Funktion IntelHex::EvaluateHexLn() */
    /*
      Eine Intel-Hex-Zeile auswerten und ggf. abspeichern.
      Falls die Zeile Fehleraufweist
    */
   
    int iResult = INTHEX_OK;
    // char c;
    int iLength;
 //todo   AddChecksum Checksum;
    unsigned long ulHighAdr;
    /* EasyCODE - */
 //todo   iLength = HexLine.Length();
    if ( /* Eine Zeile muß aus mindestens 11 Zeichen bestehen,
            die Anzahl der Zeichen muß ungerade sein und die
            Zeile muß mit ':' anfangen */
        (iLength >= 11) &&
        (iLength % 2) &&
        (HexLine[1] == ':'))
    {
        // Die Zeile in char wandlen 
//todo        HexLnToCharLn(&HexLine[2], _cLine, iLength / 2);
        /* EasyCODE - */
        // Datenlänge und Recordtype auslesen
        _ucDataLength = _cLine[0];
        _ucRecordType = _cLine[3];
        //if ( // Checksumme ok ?
        //   Checksum.Add(_cLine, _cLine + iLength / 2 - 1) == 0)
        //{
        //    /* Record auswerten */
        //    switch (_ucRecordType)
        //    {
        //    case DATA_LINE: // Daten eintragen
        //        if (_ucDataLength == iLength / 2 - 5)
        //        {
        //            // Adressoffset abspeichern
        //            _uiAdrOffset = ((unsigned int)_cLine[1] << 8) + _cLine[2];
        //            
        //            // Höchste Adresse ermitteln
        //            ulHighAdr = _ulExtLinAdr + _ulExtSegAdr + _uiAdrOffset + _ucDataLength - 1;
        //            if (ulHighAdr > _ulHighAdr)
        //            {
        //                _ulHighAdr = ulHighAdr;
        //            }
        //            
        //            if ( iStore) // soll abgespeichert werden?
        //            {
        //                if ( // Ist der Speicherplatz ausreichend ?
        //                    ulHighAdr < _ulMemSize)
        //                {
        //                    memcpy(_cMem + _ulExtLinAdr + _ulExtSegAdr + _uiAdrOffset, &_cLine[4], _ucDataLength);
        //                }
        //                else
        //                {
        //                    iResult = INTHEX_MEM_ERR;
        //                }
        //            }
        //        }
        //        else
        //        {
        //            iResult = INTHEX_LENGTH_ERR;
        //        }
        //        break;
        //    case END_OF_FILE: // End of File
        //        _uiFindLastLine = true;
        //        if (_ucDataLength != 0)
        //        {
        //            iResult = INTHEX_LENGTH_ERR;
        //        }
        //        break;
        //    case EXT_SEG_ADR: // Extended Segment Address Record
        //        _ulExtSegAdr = (((unsigned int)_cLine[4] << 8) + _cLine[5]) << 4;
        //        if (_ucDataLength != 2)
        //        {
        //            iResult = INTHEX_LENGTH_ERR;
        //        }
        //        break;
        //    case EXT_LIN_ADR: // Extended Linear Address Record
        //        _ulExtLinAdr = (((unsigned int)_cLine[4] << 8) + _cLine[5]) << 16;
        //        if (_ucDataLength != 2)
        //        {
        //            iResult = INTHEX_LENGTH_ERR;
        //        }
        //        break;
        //    default:
        //        break;
        //    }
        //    if (_ucRecordType != 1 && _uiFindLastLine) // Fehler falls die letzte Zeile schon gefunden wurde
        //    {
        //        iResult = INTHEX_TXT_AFTER_END_ERR;
        //    }
        //}
        //else
        //{
        //    iResult = INTHEX_CHECKSUM_ERR;
        //}
    }
    else
    {
        iResult = INTHEX_SYNTAX_ERR;
    }
    return iResult;
}

/* EasyCODE F */
/* TODO :
Alle Adressen auf groesser Flashsize untersuchen und Gap muss innerhalb der Min max-Werte liegen.
Falls Adresse beim Schreiben nicht gefunden wrid, mu¯ Wert an die Datei angehõngt werden. */
int IntelHex::WriteCommon(string Filename, unsigned long ulAdr, unsigned char ucLength, unsigned char* pucValue)
{
    /* Funktion IntelHex::WriteCommon */
    unsigned long ulLineAdr;
    int iResult;
    int iPos = 0; // Lesepointer auf den Anfang setzen
    string sLine;
//todo    AddChecksum Checksum;
    string line;
    int iFound;
    char cCheck;
    int i;
    
    _ulExtSegAdr = 0;
    _ulExtLinAdr = 0;
    _ulHighAdr = 0;
    _uiFindLastLine = false;
    
    // File zum Lesen und Schreiben oeffenen
//todo    fstream file(Filename.c_str(), ios_base::in | ios_base::out);
    
    // Abfragen, ob Datei zum Schreiben ge÷ffnet werden kann
    // Solange noch eine Zeile vorhanden ist und noch nicht alles gespeichert wurde
    while (true) //todo   getline(file, sLine) && (ucLength))
    {
        // Zeile auswerten
        if ((iResult = EvaluateHexLn(sLine.c_str(), false)) == INTHEX_OK)
        {
            if (_ucRecordType == DATA_LINE)
            {
                // Anfangsadresse der Zeile bestimmen
                ulLineAdr = _ulExtLinAdr + _ulExtSegAdr + _uiAdrOffset;
                
                iFound = false;
                while (ucLength)
                {
                    // Ist die gew³nschte Adresse in der Zeile
                    if ((ulAdr >= ulLineAdr) && (ulAdr < (ulLineAdr + (unsigned long)_ucDataLength)))
                    {
                        ucLength--;
                        
                        // Ein Byte speichern
                        _cLine[ulAdr - ulLineAdr + 4] = *(pucValue + ucLength);
                        
                        // nõchste Adresse selektieren
                        ulAdr++;
                        
                        iFound = true;
                    }
                    else
                    {
                        /* Adresse nicht mehr in der Zeile */
                        break;
                    }
                }
                
                if (iFound)
                {
                    // Checksumme ueber die Zeile ermitteln und anhaengen
//todo                    Checksum.ulSum = 0;
//                    /
//                    Checksum.Add((unsigned char*)_cLine, (unsigned char*)(_cLine + _ucDataLength + 3));
                    
//                    _cLine[_ucDataLength + 4] = (unsigned char)(0x100 - Checksum.ulSum);
                    
                    // Zeile in Hex wandeln
//todo                    line = ":" + CharLnToHexLn(_cLine, _ucDataLength + 5);
                    
                    // Abspeichern an die Position vor dem letzen Lesen der letzten Zeile
                    file.seekp(iPos); // ios_base::beg- Funktioniert bie Builder C++ 6 nicht
                    
                    iFound = false;
                    for (i = 0; i < 10; i++)
                    {
                        file.seekp(iPos - i);
                        
                        file.read(&cCheck, 1);
                        if (cCheck == ':')
                        {
                            iFound = true;
                            
                            file.seekp(iPos - i);
                            break;
                        }
                    }
                    if (iFound)
                    {
                        file.write(line.c_str(), 2 * (_ucDataLength + 5) + 1);
                    }
                }
            }
            else
            {
                /* Todo
                  Record = Letzte Zeile
                  Letzte Zeile abschneiden
                  Rest abpeichern
                  letzte Zeile wieder anfuegen
                  evtl vorher SegAdr neu setzen
                */
            }
            // Lesepointer auslesen
            iPos = file.tellg();
        }
    }
    file.close();
    return false; // Returncode anpassen
}

// Schnittstellenfunktionen
IntelHex::IntelHex(void)
{
    // Konstruktor IntelHex::IntelHex()
}

int IntelHex::Read(string Filename, char* pucStart, unsigned long ulLength)
{
    /* Funktion IntelHex::Read() */
    int iResult = !INTHEX_OK;
    string sLine;
    
    _cMem = pucStart;
    _ulMemSize = ulLength;
    
    _ulExtSegAdr = 0;
    _ulExtLinAdr = 0;
    _ulHighAdr = 0;
    _uiFindLastLine = false;
    
//todo    ifstream file(Filename.c_str());
    //todo while (getline(file, sLine))
    //{
    //    if ((iResult = EvaluateHexLn(sLine.c_str(), true)) != INTHEX_OK)
    //    {
    //        break;
    //    }
    //}
    //file.close();
    return iResult; //Returncode erweitern 
}

int IntelHex::Write(string Filename, unsigned long ulAdr, unsigned char ucValue, unsigned char ucHighByteFirst)
{
    /* Funktion IntelHex::Write() CHAR */
    return WriteCommon(Filename, ulAdr, 1, &ucValue);
}

int IntelHex::Write(string Filename, unsigned long ulAdr, unsigned short usValue, unsigned char ucHighByteFirst)
{
    /* Funktion IntelHex::Write() WORD */
    unsigned char ucValue[2];
    if (ucHighByteFirst == INTHEX_H_L)
    {
        ucValue[1] = usValue / 256;
        ucValue[0] = usValue & 0xff;
    }
    else
    {
        ucValue[0] = usValue / 256;
        ucValue[1] = usValue & 0xff;
    }
    return WriteCommon(Filename, ulAdr, 2, ucValue);
}

int IntelHex::Write(string Filename, unsigned long ulAdr, unsigned long ulValue, unsigned char ucHighByteFirst)
{
    /* Funktion IntelHex::Write() LONG */
    unsigned char ucValue[4];
    if (ucHighByteFirst == INTHEX_H_L)
    {
        ucValue[3] = (ulValue / 0x1000000) & 0xff;
        ucValue[2] = (ulValue / 0x10000) & 0xff;
        ucValue[1] = (ulValue / 0x100) & 0xff;
        ucValue[0] = ulValue & 0xff;
    }
    else
    {
        ucValue[0] = (ulValue / 0x1000000) & 0xff;
        ucValue[1] = (ulValue / 0x10000) & 0xff;
        ucValue[2] = (ulValue / 0x100) & 0xff;
        ucValue[3] = ulValue & 0xff;
    }
    return WriteCommon(Filename, ulAdr, 4, ucValue);
}
