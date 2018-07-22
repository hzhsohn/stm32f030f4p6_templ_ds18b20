#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "output.h"
#include "USART1.h"
#include "ds18b20.h"
#include "key.h"
#include "flash_rw.h"

//------------------------------------------------------
//按键
EzhKeyEvent ev;
EzhKeyState GPIOAStatus1;//一个GPIO对应一个EzhKeyState对象
EzhKeyState GPIOAStatus2;
//-----------------------------------------------------
//温度
int ntmp;
s16 rWaterTemperature=0,rWaterTemp=0;
int isCheckWaterSensorErr=0; 
u32 gettemp=0;
//监控温度值
int monitor_temperat_val=0;
//-----------------------------------------------------
void udoTemperature_cb(s16 temperature)
{
	printf("{\"temperature\":%0.1f,\"trigger\":%0.1f}",temperature*0.1f,monitor_temperat_val*0.1f);
	//继电器控制
	if(temperature<monitor_temperat_val)
	{
		OUTP1_SET(1);
	}
	else
	{
		OUTP1_SET(0);
	}
	//风扇控制
	if(temperature<monitor_temperat_val)
	{		
		OUTP2_SET(1);
	}
	else	
	{
		//风扇定时周期转动
		static int nnn=0;
		nnn++;
		if(nnn<10*3)
		{
			OUTP2_SET(0);
		}		
		else
		{
			OUTP2_SET(1);
			if(nnn>10*4)
			{
				nnn=0;			
			}
		}
	}
}
void udoTemperatureErr_cb()
{
	printf("temperature err\r\n");
	OUTP1_SET(0);
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
  USART1_Init(115200);	
	//初始化按键
	zhSCM_GPIOConfig(GPIOA, GPIO_Pin_3); 
	zhSCM_GPIOConfig(GPIOA, GPIO_Pin_4); 
	zhSCM_initKeyState(&GPIOAStatus1);
	zhSCM_initKeyState(&GPIOAStatus2);
	//触发温度
	FLASH_ReadByte(STARTADDR ,(uint8_t*)&monitor_temperat_val,sizeof(monitor_temperat_val));
	if(monitor_temperat_val<10 || monitor_temperat_val>1000)
	{
		//温度不在正常范围就设为默认值
		monitor_temperat_val=380; //默认触发温度38度
		FLASH_WriteByte(STARTADDR,(uint8_t*)&monitor_temperat_val,sizeof(monitor_temperat_val));
	}
	//printf("monitor_temperat_val=%d",monitor_temperat_val);
	//
	printf("{\"system:\":\"startup\"}");
		
  while (1)
  {
		//温度
    if(gettemp>200000)
		{
			LED1_SET(1);//LED灯
			TemperatureProc();
			gettemp=0;
		}		
		LED1_SET(0);//LED灯
		gettemp++;
		
		//按键1
		ev=zhSCM_keyState(&GPIOAStatus1,GPIOA,GPIO_Pin_3);
    switch(ev)
    {
			case ZH_KEY_EVENT_NONE:
				break;
      case ZH_KEY_EVENT_DOWN:
				break;
      case ZH_KEY_EVENT_PRESS:
				break;
      case ZH_KEY_EVENT_UP:
					monitor_temperat_val-=1; 
					FLASH_WriteByte(STARTADDR,(uint8_t*)&monitor_temperat_val,sizeof(monitor_temperat_val));
        break;
    }
		//按键2
		ev=zhSCM_keyState(&GPIOAStatus2,GPIOA,GPIO_Pin_4);
    switch(ev)
    {
			case ZH_KEY_EVENT_NONE:
				break;
      case ZH_KEY_EVENT_DOWN:
        break;
      case ZH_KEY_EVENT_PRESS:
        break;
      case ZH_KEY_EVENT_UP:
					monitor_temperat_val+=1; 
					FLASH_WriteByte(STARTADDR,(uint8_t*)&monitor_temperat_val,sizeof(monitor_temperat_val));
        break;
    }
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
