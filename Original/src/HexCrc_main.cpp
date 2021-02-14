/* EasyCODE V8 */
/* EasyCODE ( 0 */
#include <vcl.h>
#pragma hdrstop
#pragma argsused
#include <iostream.h>
#include "IntelHex.h"
#include "MemGap.h"
#include "CRC16.h"
#include "AddChecksum.h"
#include "HexCrc_main.h"
/* EasyCODE - */
// Konstanten des Flashs und der Flashstruktur
#define FLASH_SIZE 0x100000
#define OFFS_FLASH_START     56
#define OFFS_FLASH_END       60
#define OFFS_FLASH_GAP_START 64
#define OFFS_FLASH_GAP_END   68
#define OFFS_ADD32           72
#define OFFS_CRC16           76
/* EasyCODE - */
#define ANZAHL_LIZ 2  // Anzahl der gültigen Lizenzen
#define LAENGE_LIZ 32 // Länge des Lizenzstrings
/* EasyCODE < */
// Stuktur für VersionInfo
struct ProgVersionInfo{
 int iVersion;
 int iSubVersion;
 int iSubSubVersion;
};
/* EasyCODE > */
/* Lizenzstring für die Suche der festgelegten Struktur im Hex-File */
char cLicence[ANZAHL_LIZ][LAENGE_LIZ+1] = {"Bosch_Rexroth_AG_Hannover_Marine\0",
                                           "Bosch_Rexroth_AG_Hannover_Mar_02\0"};
/* EasyCODE ( 0 
iGetFileAndBuildCRC */
/* EasyCODE F */
bool iGetFileAndBuildCRC(AnsiString fileSrc,bool bQuiet, tpCheckData pData)
{
    // Hex-File einlesen, Lizenzschlüssel suchen und Stelle merken, sowie
    // CRC16 und ADD16-Checksumme bilden
    char *mem = new char[FLASH_SIZE];
    MemGap cMemList;
    CRC16 clCRC16;
    AddChecksum clAdd16;
    int found;
    int i;
    int iResult;
    bool iRtnValue = false;
    AnsiString s;
    unsigned long ulPointer;
    int iLicenceNo;
    /* EasyCODE - */
    // Speicher in der Größe des Flash reservieren
    memset(mem,0xff,FLASH_SIZE);
    if ( (iResult = pData->src.Read(fileSrc,mem,FLASH_SIZE)) != INTHEX_OK )
    {
        cout << "Error: corrupt Intel-Hex-File. Error result " << iResult << endl;
    }
    else
    {
        /* EasyCODE ( 0 
        Checksummen ermitteln */
        found = false;
        iLicenceNo = 0;
        /* EasyCODE - */
        // Licence-Key suchen
        while ( !found && iLicenceNo < ANZAHL_LIZ )
        {
            for ( ulPointer = 0; ulPointer < (pData->src._ulHighAdr - LAENGE_LIZ); ulPointer++ )
            {
                if ( !memcmp(mem + ulPointer, cLicence[iLicenceNo], LAENGE_LIZ) )
                {
                    found = true;
                    break;
                }
            }
            iLicenceNo++;
        }
        if ( found )
        {
            pData->ulPointer = ulPointer; // Pointer auf den Key merken
            if ( !bQuiet )
            {
                s = "highest address: 0x"+IntToHex((int)(pData->src._ulHighAdr/65536),0)
                  + IntToHex((int)(pData->src._ulHighAdr & 0xffff),0)
                  + "  (" + FloatToStrF((float)pData->src._ulHighAdr/1024,ffFixed,7,1) + " kByte)";
                cout << s.c_str() << endl;
                
                s = cLicence[iLicenceNo-1];
                s = "found licence key: " + s;
                cout << s.c_str() << endl;
                
                s = "structure addr.: 0x"
                  + IntToHex((int)(ulPointer/65536),0)
                  + IntToHex((int)(ulPointer & 0xfff),0);
                cout << s.c_str() << endl;
            }
            // Liste des Speicherbereich aufbauen, über den die Checksummen gebildet werden sollen
            cMemList.Init  (*((unsigned long *)(mem + ulPointer + OFFS_FLASH_START)),
                            *((unsigned long *)(mem + ulPointer + OFFS_FLASH_END)));
            cMemList.AddGap(*((unsigned long *)(mem + ulPointer + OFFS_FLASH_GAP_START)),
                            *((unsigned long *)(mem + ulPointer + OFFS_FLASH_GAP_END)));
            cMemList.AddGap(ulPointer + OFFS_ADD32,ulPointer + OFFS_ADD32 + 5);
            
            // Über alle Elemente der Speicherbereichsliste
            for ( i=0; i < cMemList.Size(); i++ )
            {
                // CRC16-Summe bilden
                pData->usCRC16 = clCRC16.Add((unsigned char *)(mem + cMemList.Start(i)),
                                             (unsigned char *)(mem + cMemList.End(i)));
                // ADD16-Summe bilden
                pData->ulADD16 = clAdd16.Add((unsigned short *)(mem + cMemList.Start(i)),
                                             (unsigned short *)(mem + cMemList.End(i)));
            }
            if ( !bQuiet )
            {
                cout << "calculated checksums" << endl;
                s = "   CRC16: 0x" + IntToHex((int)pData->usCRC16,0);
                cout << s.c_str() << endl;
                s = "   ADD16: 0x" + IntToHex((unsigned short)pData->ulADD16,0);
                cout << s.c_str() << endl;
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

/* EasyCODE ) */
/* EasyCODE ( 0 
writeCRC */
/* EasyCODE F */
void writeCRC(AnsiString fileSrc, bool bQuiet, tpCheckData pData)
{
    // Hex-File kopieren mit Namensanhang CRC16-Checksumme und in das
    // kopierte File die CRC16 und ADD16-Checksumme eintragen
    int iErg;
    AnsiString fileDest;
    /* EasyCODE - */
    // Dest-Filename bilden (Source + CRC16-Checksumme)
    fileDest = fileSrc.SubString(1,fileSrc.Pos('.')-1) + "_"
             + IntToHex(pData->usCRC16,4)
             + ExtractFileExt(fileSrc);
    /* EasyCODE - */
    // SourceFile nach DestFile kopieren
    iErg =CopyFile(fileSrc.c_str(),fileDest.c_str(),false);
    /* EasyCODE - */
    // CRC16 ins Hexfile schreiben
    pData->src.Write(fileDest,pData->ulPointer+OFFS_CRC16,
                     pData->usCRC16,INTHEX_L_H);
    /* EasyCODE - */
    // ADD16 ins Hexfile schreiben (High-Word = 0)
    pData->src.Write(fileDest,(unsigned long)pData->ulPointer+OFFS_ADD32,
                     pData->ulADD16,INTHEX_L_H);
    if ( !bQuiet )
    {
        cout << "write: " << fileDest.c_str() << endl;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
writeHelp */
/* EasyCODE F */
void writeHelp(void)
{
    // Ausgabe der Helpinformationen
    /* EasyCODE - */
    cout << endl;
    cout << "call \"Crc2Hex.exe <filename.hex> [quiet] [help]\"" << endl << endl;
    cout << "[quiet] Keine zusaetzliche Ausgabe von Programmlaenge und Checksummen" << endl;
    cout << "        auf der Konsole" << endl;
    cout << "[help]  Ausgabe dieser Hilfe" << endl;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetProgramVersionInfo */
/* EasyCODE F */
void GetProgramVersionInfo( HANDLE hHandle, ProgVersionInfo * Ver )
{
    // Ermittelt die Versionsinfo eines Programms
    /* EasyCODE - */
    #define MAX_PATH_LEN 512
    /* EasyCODE - */
    char ModuleFileName[ MAX_PATH_LEN ];
    DWORD  aVIH;
    DWORD  aSize;
    unsigned int Len;
    LPVOID pData;
    tagVS_FIXEDFILEINFO vsInfo;
    LPVOID pBuffer;
    AnsiString sResult = "";
    if ( Ver==NULL )
    {
        return ;
    }
    else
    {
        if ( GetModuleFileName( hHandle, ModuleFileName, MAX_PATH_LEN ) )
        {
            // HInstance
            aSize = GetFileVersionInfoSize( ModuleFileName, &aVIH );
            pData = malloc( aSize+10 );
            GetFileVersionInfo( ModuleFileName, aVIH, aSize, pData );
            if ( VerQueryValue( pData,"\\",&pBuffer,&Len)
                 || VerQueryValue( pData, NULL, &pBuffer,&Len)
                 && ( pBuffer != NULL ) )
            {
                if ( Len > sizeof(vsInfo) )
                {
                    Len = sizeof(vsInfo);
                }
                if ( Len > 0 )
                {
                    memcpy( &vsInfo, pBuffer, Len );
                    Ver->iVersion = vsInfo.dwFileVersionMS >> 16 ;
                    Ver->iSubVersion = vsInfo.dwFileVersionMS & 0xFFFF ;
                    Ver->iSubSubVersion = vsInfo.dwFileVersionLS >> 16 ;
                }
            }
            free( pData );
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
main */
/* EasyCODE F */
int main(int argc, char* argv[])
{
    tCheckData checkData;
    int iResult = 1;
    int i;
    bool bQuiet = false;
    bool bHelp = false;
    AnsiString fileName;
    ProgVersionInfo prgVersion;
    AnsiString s;
    /* EasyCODE - */
    // Versionsinfo holen
    GetProgramVersionInfo(HInstance,&prgVersion);
    /* EasyCODE - */
    // Programmmeldung auf Konsole ausgeben
    s = IntToStr(prgVersion.iVersion);
    s = s + "." + IntToStr(prgVersion.iSubVersion);
    s = s + "." + IntToStr(prgVersion.iSubSubVersion);
    s = "CRC16 Builder for Intel-Hex-Files V" + s;
    cout << s.c_str() << endl;
    cout << "(c) 2009-2010 Bosch Rexroth AG - DC-IA/ESM3" << endl;
    if ( argc >= 2 )
    {
        // es muss mindestens 1 Argument übergeben werden
        for ( i=1; i<argc ; i++ )
        {
            if ( UpperCase(argv[i]) == "QUIET" )
            {
                bQuiet = true;
            }
            else
            {
                if ( UpperCase(argv[i]) == "HELP" )
                {
                    bHelp = true;
                }
                else
                {
                    fileName = argv[i];
                }
            }
        }
        if ( bHelp )
        {
            // Ausgabe des Hilfetextes
            writeHelp();
        }
        else
        {
            /* EasyCODE ( 0 
            File bearbeiten */
            if ( FileExists(fileName) )
            {
                // File einlesen und CRC-Checksumme ermitteln
                if ( iGetFileAndBuildCRC(fileName, bQuiet, &checkData) )
                {
                    // File kopieren und CRC-Checksumme eintragen
                    writeCRC(fileName,bQuiet, &checkData);
                    /* EasyCODE - */
                    iResult = 0;  // CRC-Ermittlung hat fehlerfrei funktioniert
                }
            }
            else
            {
                cout << "Error: file \"" << fileName.c_str() << "\" don't exist!" << endl;
            }
            /* EasyCODE ) */
        }
    }
    else
    {
        cout << "Error: please call \"Crc2Hex.exe <filename.hex> [quiet] [help]\"" << endl;
    }
    return iResult;
}

/* EasyCODE ) */
/* EasyCODE ) */
