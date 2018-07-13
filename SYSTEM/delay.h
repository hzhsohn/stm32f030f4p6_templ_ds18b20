#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f0xx.h"

#define u8_t 			unsigned char
#define u8 				unsigned char
#define u32_t 		unsigned int
#define u16_t 		unsigned short
#define s16 			short
#define u16 			unsigned short
#define u32				unsigned int

void delay_init(void);
void delay_us(int32_t nus);
void delay_ms(int16_t nms);
#endif
