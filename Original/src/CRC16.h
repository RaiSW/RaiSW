//---------------------------------------------------------------------------

#ifndef CRC16H
#define CRC16H
//---------------------------------------------------------------------------
class CRC16
{
   private:
     #define GENERATOR_POLYNOM ((0x8005 / 4)|0x8000)

   public:
      unsigned short usSum;

      CRC16(void);
      CRC16(unsigned short);
      virtual ~CRC16();
      unsigned short __fastcall Add(unsigned char *pucStart, unsigned char *pucEnd);
};
#endif
