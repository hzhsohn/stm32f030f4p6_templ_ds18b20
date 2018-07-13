#include "flash_rw.h"

/*
Àý
	uint8_t data[100];
	FLASH_WriteByte(STARTADDR , data , 100);
*/
void FLASH_WriteByte(uint32_t addr , uint8_t *p , uint16_t Byte_Num)
  {
          uint32_t HalfWord;
          Byte_Num = Byte_Num/2;
          FLASH_Unlock();
          FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
          FLASH_ErasePage(addr);
          while(Byte_Num --)
          {
                  HalfWord=*(p++);
                  HalfWord|=*(p++)<<8;
                  FLASH_ProgramHalfWord(addr, HalfWord);
                  addr += 2;
          }
          FLASH_Lock();
  }

	
/*
Àý
  uint8_t data[101];
  FLASH_ReadByte(STARTADDR , data , 101);
*/
void FLASH_ReadByte(uint32_t addr , uint8_t *p , uint16_t Byte_Num)
{
	while(Byte_Num--)
	{
	 *(p++)=*((uint8_t*)addr++);
	}
}

