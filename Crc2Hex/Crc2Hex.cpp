
#include <iostream>
#include <fstream>
#include <string>
#include "Lib.h"
#include "IntelHex.h"

using namespace std;

#define PRG_VERSION     2
#define SUB_VERSION     0
#define SUB_SUB_VERSION 0

#define FLASH_SIZE 0x100000
#define OFFS_FLASH_START     56
#define OFFS_FLASH_END       60
#define OFFS_FLASH_GAP_START 64
#define OFFS_FLASH_GAP_END   68
#define OFFS_ADD32           72
#define OFFS_CRC16           76

#define ANZAHL_LIZ 2  // Anzahl der gültigen Lizenzen
#define LAENGE_LIZ 32 // Länge des Lizenzstrings

typedef struct {
    IntelHex  src;
    unsigned long  ulPointer;
    unsigned short usCRC16;
    unsigned long  ulADD16;
} tCheckData;
typedef tCheckData* tpCheckData;

char cLicence[][LAENGE_LIZ+1] = { "Bosch_Rexroth_AG_Hannover_Marine",
                                  "Bosch_Rexroth_AG_Hannover_Mar_02" };

void writeHelp(void);
bool iGetFileAndBuildCRC(string, bool bQuiet, tpCheckData pData);



int main(int argc, char** argv)
{
    int result = 1;
    bool bQuiet = false;
    bool bHelp = false;
    string fileName;
    tCheckData checkData;

    string s = "CRC Builder for Intel-Hex-Files V" + to_string(PRG_VERSION);
    s = s + "." + to_string(SUB_VERSION);
    s = s + "." + to_string(SUB_SUB_VERSION);
    
    cout << s << endl;
    cout << "(c) 2021 AVENTICS GmbH - R&D Electronics Marine" << endl;

    if (argc >= 2)
    {   // es muss mindestens 1 Argument übergeben werden
        for (int i = 1; i < argc; i++)
        {
            string cmd = UCase(argv[i]);
            if (cmd == "QUIET")
            {
                bQuiet = true;
            }
            else if (cmd == "HELP")
            {
                bHelp = true;
            }
            else
            {
                fileName = argv[i];
            }
        }
        if (bHelp)
        {
            // Ausgabe des Hilfetextes
            writeHelp();
        }
        else
        {
            /* File bearbeiten */
            ifstream ifile;
            ifile.open(fileName);
            if (ifile)
            {
                // File einlesen und CRC-Checksumme ermitteln
                if (iGetFileAndBuildCRC(fileName, bQuiet, &checkData))
                {
                //    // File kopieren und CRC-Checksumme eintragen
                //    writeCRC(fileName, bQuiet, &checkData);
                //    /* EasyCODE - */
                    result = 0;  // CRC-Ermittlung hat fehlerfrei funktioniert
                }
            }
            else
            {
                cout << "Error: file \"" << fileName.c_str() << "\" don't exist!" << endl;
            }
        }
    }
    else
    {
        cout << "Error: please call \"Crc2Hex.exe <filename.hex> [quiet] [help]\"" << endl;
    }
    return result;
}

void writeHelp(void)
{
    // Ausgabe der Helpinformationen
    cout << endl;
    cout << "call \"Crc2Hex.exe <filename.hex> [quiet] [help]\"" << endl << endl;
    cout << "[quiet] no output of additional information like checksum and program length" << endl;
    cout << "[help]  output of this help" << endl;
}


bool iGetFileAndBuildCRC(string fileSrc, bool bQuiet, tpCheckData pData)
{
    // Hex-File einlesen, Lizenzschlüssel suchen und Stelle merken, sowie
    // CRC16 und ADD16-Checksumme bilden
    char* mem = new char[FLASH_SIZE];
  //todo  MemGap cMemList;
  //  CRC16 clCRC16;
  //  AddChecksum clAdd16;
    int found;
    int i;
    int iResult;
    bool iRtnValue = false;
    string s;
    unsigned long ulPointer;
    int iLicenceNo;
 
    // Speicher in der Größe des Flash reservieren
    memset(mem, 0xff, FLASH_SIZE);
    if ((iResult = pData->src.Read(fileSrc, mem, FLASH_SIZE)) != INTHEX_OK)
    {
        cout << "Error: corrupt Intel-Hex-File. Error result " << iResult << endl;
    }
    else
    {
        // Checksummen ermitteln
        found = false;
        iLicenceNo = 0;
        
        // Licence-Key suchen
        while (!found && iLicenceNo < ANZAHL_LIZ)
        {
            for (ulPointer = 0; ulPointer < (pData->src._ulHighAdr - LAENGE_LIZ); ulPointer++)
            {
                if (!memcmp(mem + ulPointer, cLicence[iLicenceNo], LAENGE_LIZ))
                {
                    found = true;
                    break;
                }
            }
            iLicenceNo++;
        }
        if (found)
        {
            pData->ulPointer = ulPointer; // Pointer auf den Key merken
            if (!bQuiet)
            {
                //todo s = "highest address: 0x" + IntToHex((int)(pData->src._ulHighAdr / 65536), 0)
                //    + IntToHex((int)(pData->src._ulHighAdr & 0xffff), 0)
                //    + "  (" + FloatToStrF((float)pData->src._ulHighAdr / 1024, ffFixed, 7, 1) + " kByte)";
                //cout << s.c_str() << endl;

                s = cLicence[iLicenceNo - 1];
                s = "found licence key: " + s;
                cout << s.c_str() << endl;

                //todo s = "structure addr.: 0x"
                //    + IntToHex((int)(ulPointer / 65536), 0)
                //    + IntToHex((int)(ulPointer & 0xfff), 0);
                //cout << s.c_str() << endl;
            }
            // Liste des Speicherbereich aufbauen, über den die Checksummen gebildet werden sollen
            //todo cMemList.Init(*((unsigned long*)(mem + ulPointer + OFFS_FLASH_START)),
            //    *((unsigned long*)(mem + ulPointer + OFFS_FLASH_END)));
            //cMemList.AddGap(*((unsigned long*)(mem + ulPointer + OFFS_FLASH_GAP_START)),
            //    *((unsigned long*)(mem + ulPointer + OFFS_FLASH_GAP_END)));
            //cMemList.AddGap(ulPointer + OFFS_ADD32, ulPointer + OFFS_ADD32 + 5);

            // Über alle Elemente der Speicherbereichsliste
            //todo for (i = 0; i < cMemList.Size(); i++)
            //{
            //    // CRC16-Summe bilden
            //    pData->usCRC16 = clCRC16.Add((unsigned char*)(mem + cMemList.Start(i)),
            //        (unsigned char*)(mem + cMemList.End(i)));
            //    // ADD16-Summe bilden
            //    pData->ulADD16 = clAdd16.Add((unsigned short*)(mem + cMemList.Start(i)),
            //        (unsigned short*)(mem + cMemList.End(i)));
            //}
            if (!bQuiet)
            {
                //todo cout << "calculated checksums" << endl;
                //s = "   CRC16: 0x" + IntToHex((int)pData->usCRC16, 0);
                //cout << s.c_str() << endl;
                //s = "   ADD16: 0x" + IntToHex((unsigned short)pData->ulADD16, 0);
                //cout << s.c_str() << endl;
            }
            // Alles gut gelaufen
            iRtnValue = true;
        }
        else
        {
            // Lizensierung wurde im Hex-File nicht gefunden
            cout << "Error: Licence key in source file is missing!" << endl;
        }
        /* EasyCODE ) */
    }
    delete[] mem;
    return iRtnValue;
}

