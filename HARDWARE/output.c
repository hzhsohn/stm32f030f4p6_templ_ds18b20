#include "output.h"

void OUTPUT_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(OUTPUT_AHBP, ENABLE);

  GPIO_InitStructure.GPIO_Pin = OUTP1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(OUTP1, &GPIO_InitStructure);
}
