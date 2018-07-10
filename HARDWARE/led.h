#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

#define LED_OFF                       GPIOA->BSRR = 1<<1
#define LED_ON                        GPIOA->BRR = 1<<1
#define LED_TURN                      GPIOA->ODR ^= 1<<1

void LED_Init(void);

#endif
