#ifndef __LED_H
#define __LED_H

#include "delay.h"


#define	LED_AHBP 	RCC_AHBPeriph_GPIOA


#define LED1				GPIOA
#define LED1_PIN		GPIO_Pin_1

#define LED1_OFF                GPIO_ResetBits(LED1,LED1_PIN)
#define LED1_ON                 GPIO_SetBits(LED1,LED1_PIN)

void LED_Init(void);
#endif
