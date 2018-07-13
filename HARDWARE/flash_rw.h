#ifndef __flash_rw_H
#define __flash_rw_H

#include "stm32f0xx_flash.h"
#include "delay.h"


#define STARTADDR 0x08006000

void FLASH_WriteByte(uint32_t addr , uint8_t *p , uint16_t Byte_Num);
void FLASH_ReadByte(uint32_t addr , uint8_t *p , uint16_t Byte_Num);

#endif
