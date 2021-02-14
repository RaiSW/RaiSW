//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CRC16.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

CRC16::CRC16(void)
{
   usSum = 0;
}
//---------------------------------------------------------------------------

CRC16::CRC16(unsigned short usValue)
{
   CRC16();
   usSum = usValue;
}

//---------------------------------------------------------------------------

CRC16::~CRC16(void)
{
}
//---------------------------------------------------------------------------

unsigned short __fastcall CRC16::Add(unsigned char *pucStart, unsigned char *pucEnd)
{
   unsigned char value;
   unsigned int i;
   
   while (pucStart <= pucEnd)
   {
      value = *((unsigned char *)pucStart);
      for (i = 0; i < 8; i++)
      {
         if ((usSum ^ value) & 1)
         {
            usSum = (usSum >> 1) ^ GENERATOR_POLYNOM;
         }
         else
            usSum >>= 1;
         value >>= 1;
      }
      ++pucStart;
   }
   return (usSum);
}
//---------------------------------------------------------------------------

