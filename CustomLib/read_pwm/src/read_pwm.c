#include "stm32f30x_exti.h"
#include "common.h"

/*
 thanks to clive1
 */

uint32_t high_start[] = { 0, 0, 0, 0, 0, 0 };
__IO uint8_t ppm_value[] = { 0, 0, 0, 0, 0, 0 };

void init_read_pwm() {

	EXTI_InitTypeDef   EXTI_InitStructure;
	  GPIO_InitTypeDef   GPIO_InitStructure;
	  NVIC_InitTypeDef   NVIC_InitStructure;

	  /* Enable GPIOA clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	  /* Configure PA0 pin as input floating */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* Enable SYSCFG clock */
	  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	  /* Connect EXTI0 Line to PA0 pin */
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);

	  /* Configure EXTI0 line */
	  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  /* Enable and set EXTI0 Interrupt to the lowest priority */
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

}

