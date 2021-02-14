#include "MemGap.h"

MemGap::MemGap(void)
{
}
//---------------------------------------------------------------------------

void MemGap::Init(unsigned long ulStart, unsigned long ulEnd)
{
    memRange.ulStart = ulStart;
    memRange.ulEnd = ulEnd;
    vMemRangeList.clear();
    vMemRangeList.push_back(memRange);
}
//---------------------------------------------------------------------------

int MemGap::Size(void)
{
    return(vMemRangeList.size());
}
//---------------------------------------------------------------------------

unsigned long MemGap::Start(int index)
{
    return(vMemRangeList[index].ulStart);
}
//---------------------------------------------------------------------------

unsigned long MemGap::End(int index)
{
    return(vMemRangeList[index].ulEnd);
}
//---------------------------------------------------------------------------

void MemGap::AddGap(unsigned long ulStart, unsigned long ulEnd)
{
    tMemRange memEintrag;
    unsigned int i;
    unsigned long ulTemp;

    // Gap auf Speicherbereich begrenzen
    if (ulStart < memRange.ulStart) ulStart = memRange.ulStart;
    if (ulEnd > memRange.ulEnd)   ulEnd = memRange.ulEnd;

    for (i = 0; i < vMemRangeList.size(); i++)
    {
        // �berschneidung eines Bereiches
        if (ulStart <= vMemRangeList[i].ulStart && ulEnd >= vMemRangeList[i].ulEnd)
        {
            vMemRangeList.erase(vMemRangeList.begin() + i); // Bereich l�schen
            i--;
            continue;
        }

        // �berlappung am unteren Ende des Bereiches
        if (ulStart <= vMemRangeList[i].ulStart && ulEnd >= vMemRangeList[i].ulStart)
        {
            vMemRangeList[i].ulStart = ulEnd + 1; // unteres Ende abschneiden
            //  Bereich l�schen, wenn Gr��e < 0
            if (vMemRangeList[i].ulStart > vMemRangeList[i].ulEnd)
            {
                vMemRangeList.erase(vMemRangeList.begin() + i);
                i--;
            }
            continue;
        }

        // �berlappung am oberen Ende des Bereiches
        if (ulStart <= vMemRangeList[i].ulEnd && ulEnd >= vMemRangeList[i].ulEnd)
        {
            vMemRangeList[i].ulEnd = ulStart - 1; // oberes Ende abschneiden
            //  Bereich l�schen, wenn Gr��e < 0
            if (vMemRangeList[i].ulStart > vMemRangeList[i].ulEnd)
            {
                vMemRangeList.erase(vMemRangeList.begin() + i);
                i--;
            }
            continue;
        }

        // Unterscneidung mit dem Bereich (Splitten)
        if (ulStart > vMemRangeList[i].ulStart && ulEnd < vMemRangeList[i].ulEnd)
        {
            ulTemp = vMemRangeList[i].ulEnd;
            vMemRangeList[i].ulEnd = ulStart - 1;
            memEintrag.ulStart = ulEnd + 1;
            memEintrag.ulEnd = ulTemp;
            vMemRangeList.insert(vMemRangeList.begin() + i + 1, memEintrag);
            break; // keine �berpr�fung weiterer Bereiche notwendig
        }
    }
}
