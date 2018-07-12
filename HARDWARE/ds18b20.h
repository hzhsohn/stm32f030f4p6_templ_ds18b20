#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f0xx.h"

#define u8_t unsigned char
#define u8 unsigned char
#define u32_t unsigned int
#define u16_t unsigned short
#define s16 short


#define DS_RCC_AHB_GPIO	RCC_AHBPeriph_GPIOA
#define DS18B20_DQ_GPIO 	    GPIOA
#define DS18B20_DQ_PIN  	    GPIO_Pin_2

#define DS18B20_DQ_STATE(x) (x)>0?GPIO_SetBits(DS18B20_DQ_GPIO,DS18B20_DQ_PIN):GPIO_ResetBits(DS18B20_DQ_GPIO,DS18B20_DQ_PIN)
#define DS18B20_DQ_GET() GPIO_ReadInputDataBit(DS18B20_DQ_GPIO,DS18B20_DQ_PIN)
u8 DS18B20_Init(void);			//初始化DS18B20
s16 DS18B20_Get_Temp(void);		//获取温度
void DS18B20_Start(void);			//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20 

#endif
