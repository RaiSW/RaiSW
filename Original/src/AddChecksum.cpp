//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddChecksum.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

AddChecksum::AddChecksum(void)
{
   ulSum = 0;
}
//---------------------------------------------------------------------------

unsigned long __fastcall AddChecksum::Add(unsigned char *pucStart, unsigned char *pucEnd)
{
   while (pucStart <= pucEnd)
      ulSum = (ulSum + *(pucStart++)) & 0xff;
   return (ulSum);
};
//---------------------------------------------------------------------------

unsigned long __fastcall AddChecksum::Add(unsigned short *pusStart, unsigned short *pusEnd)
{
   while (pusStart <= pusEnd)
      ulSum = (ulSum + *(pusStart++)) & 0xffff;
   return (ulSum);
}
//---------------------------------------------------------------------------

unsigned long __fastcall AddChecksum::Add(unsigned long *pulStart, unsigned long *pulEnd)
{
   while (pulStart <= pulEnd)
      ulSum += *(pulStart++);
   return (ulSum);
}
//---------------------------------------------------------------------------

