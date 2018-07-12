#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "output.h"
#include "USART1.h"
#include "ds18b20.h"

int ntmp;
s16 rWaterTemperature=0,rWaterTemp=0;
int isCheckWaterSensorErr=0; 

void udoTemperature_cb(s16 temperature)
{
	printf("temperature=%0.1f\r\n",temperature*0.1f);
	
	if(temperature<310)
		OUTP1_ON;
	else
		OUTP1_OFF;
}
void udoTemperatureErr_cb()
{
	printf("temperature err\r\n");
	OUTP1_OFF;
}

void TemperatureProc()
{
	rWaterTemp=DS18B20_Get_Temp();
	//printf("%d\r\n",rWaterTemp);
	if(rWaterTemp<2000 && rWaterTemp> -200) //限制位
	{
		ntmp=rWaterTemp-rWaterTemperature;
		if(ntmp<20 && ntmp>-20)	//限制突变幅度
		{     
			udoTemperature_cb(rWaterTemp);
			isCheckWaterSensorErr=0;
		}
		rWaterTemperature=rWaterTemp;
	}
	else
	{
		isCheckWaterSensorErr++;						
		if(isCheckWaterSensorErr>10) //传感数据毛病太多关掉加热继电器
		{
			//传感器有毛病了
			udoTemperatureErr_cb();
		}
	}
}

int main(void)
{
  delay_init();
	DS18B20_Init();
  LED_Init();
	OUTPUT_Init();
  USART1_Init(9600);	
	
	printf("STM32F030F4P6 USART TEST\r\n");
		
  while (1)
  {	
		//LED灯
	  delay_ms(500);
    LED1_ON;  
		delay_ms(500);
		LED1_OFF; 
		//温度
    TemperatureProc();
  }
 
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
