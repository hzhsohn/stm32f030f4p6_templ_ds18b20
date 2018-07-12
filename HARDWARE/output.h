#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "stm32f0xx.h"

#define	OUTPUT_AHBP 	RCC_AHBPeriph_GPIOA


#define OUTP1		GPIOA
#define OUTP1_PIN		GPIO_Pin_3

#define OUTP1_OFF                GPIO_ResetBits(OUTP1,OUTP1_PIN)
#define OUTP1_ON                 GPIO_SetBits(OUTP1,OUTP1_PIN)

void OUTPUT_Init(void);

#endif
