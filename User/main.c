#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "USART1.h"
int main(void)
{
  delay_init();
  LED_Init();
  USART1_Init(9600);	
	printf("STM32F030F4P6 USART TEST\r\n");
  while (1)
  {
	  delay_ms(50);
    LED_TURN;   //LED·­×ª
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
