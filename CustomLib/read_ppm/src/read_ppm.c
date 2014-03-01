#include "stm32f30x_exti.h"
#include "common.h"

/*
 thanks to clive1
 */

uint32_t high_start[] = { 0, 0, 0, 0, 0, 0 };
__IO uint8_t ppm_value[] = { 0, 0, 0, 0, 0, 0 };

void external_interrupt_init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the pin Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //or TIM

	int i = 0;
	for (i = 0; i < 6; i++) {

		/* Configure pin as input */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = 1<<(i+10);
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		/* Connect Button EXTI Line to GPIO Pin */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, i + 10);

		/* Configure EXTI line */
		EXTI_InitStructure.EXTI_Line = i + 10;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void) {
	int i;
	for (i = 0; i < 6; i++) {
		if (EXTI_GetITStatus(i + 10) == SET){ //IF pin is now HIGH
			if (high_start[i] == 0){
				high_start[i] = micros();
			}
		}else{ //IF pin is now LOW
			if (high_start[i] != 0){
				ppm_value[i] = micros() - high_start[i];
				high_start[i] = 0;
			}
		}
		EXTI_ClearITPendingBit(i+10); //clear pending interrupt
	}

}
