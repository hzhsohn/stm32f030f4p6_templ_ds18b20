#include "ds18b20.h"
#include "delay.h"

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init()
{
	/*
	GPIO_InitTypeDef GPIO_MyStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_MyStruct.GPIO_Pin = DS18B20_DQ_PIN;
  GPIO_MyStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_MyStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DS18B20_DQ_GPIO, &GPIO_MyStruct);
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(DS_RCC_AHB_GPIO, ENABLE);  //使能GPIOA的时钟
	
	GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_NOPULL
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	DS18B20_Rst();
	
	return DS18B20_Check();
}

void DS18B20_DQ_IN(void)
{
	/*
	GPIO_InitTypeDef GPIO_MyStruct;
	GPIO_MyStruct.GPIO_Pin = DS18B20_DQ_PIN;
  GPIO_MyStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_MyStruct.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(DS18B20_DQ_GPIO, &GPIO_MyStruct);
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}
//channel:0~7
//status:0---low,1---high
void DS18B20_DQ_OUT(void)
{
	/*
	GPIO_InitTypeDef GPIO_MyStruct;
	GPIO_MyStruct.GPIO_Pin = DS18B20_DQ_PIN;
  GPIO_MyStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_MyStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DS18B20_DQ_GPIO, &GPIO_MyStruct);
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//复位DS18B20
void DS18B20_Rst()	   
{     
	DS18B20_DQ_OUT();	//设置为输出
	DS18B20_DQ_STATE(0);	//拉低DQ
	delay_us(750);      		//拉低750us
	DS18B20_DQ_STATE(1);  	//DQ=1 
	delay_us(15);       		//15US
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check() 	   
{   
	u8 retry=0;
	DS18B20_DQ_IN();    //设置为输入
	while(DS18B20_DQ_GET()&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while(!DS18B20_DQ_GET()&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit() 
{
	u8 data;
	
	DS18B20_DQ_OUT();		//设置为输出
	DS18B20_DQ_STATE(0);	
	delay_us(2);
	DS18B20_DQ_STATE(1);	

	DS18B20_DQ_IN();    	//设置为输入
	delay_us(12);
	if(DS18B20_DQ_GET())data=1;
	else data=0;	 
	delay_us(50);
	
	return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte()   
{        
	u8 i,j,dat;
	
	dat=0;
	for(i=1;i<=8;i++) 
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}
	
	return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	 
	DS18B20_DQ_OUT();		//设置为输出
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // 写1
        {
			DS18B20_DQ_STATE(0);	
            delay_us(2);                            
			DS18B20_DQ_STATE(1);	
			delay_us(60);             
        }
        else            //写0
        {
			DS18B20_DQ_STATE(0);
            delay_us(60);             
			DS18B20_DQ_STATE(1);
            delay_us(2);                          
        }
    }
}
 
//开始温度转换
void DS18B20_Start()
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}



//从ds18b20得到温度值，未CRC校验
//精度：0.1C
//返回值：温度值 （-550~1250）,放大十倍保留一位小数点
//返回状态值：0x7FFF(自定义错误状态)---未接或DQ和电源GND断线，DQ与电源VCC,GND短路；
//			  850(从探头读回来值)---一直未变化则电源VCC断线,初次上电默认值；
//			  0x7FFE(自定义错误状态)---超出探头最高采集范围	
//			  0x7FFD(自定义错误状态)---低出探头最低采集范围	
s16 DS18B20_Get_Temp()
{
    u8 temp;
    u8 TL,TH;
    s16 tem;
	
    DS18B20_Start();           	//开始转换
    DS18B20_Rst();
    if(DS18B20_Check()) return 0x7FFF; 
    DS18B20_Write_Byte(0xcc);  // skip rom
    DS18B20_Write_Byte(0xbe);  // convert	    	
    TL=DS18B20_Read_Byte();     	// LSB   
    TH=DS18B20_Read_Byte();    	// MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
	temp += tem;
	
	if(temp)
	{
		if(tem > 1250) return 0x7FFE;
		return tem;
	}
	else
	{
		if(tem > 550) return 0x7FFD;
		return -tem;
	}
}
