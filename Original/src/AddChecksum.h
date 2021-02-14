//---------------------------------------------------------------------------

#ifndef AddChecksumH
#define AddChecksumH 
//---------------------------------------------------------------------------
class AddChecksum
{
   private:

   public:
      unsigned long ulSum;
       
      AddChecksum(void);
      unsigned long __fastcall Add(unsigned char  *pucStart, unsigned char  *pucEnd);
      unsigned long __fastcall Add(unsigned short *pusStart, unsigned short *pusEnd);
      unsigned long __fastcall Add(unsigned long  *pulStart, unsigned long  *pulEnd);
};
#endif
