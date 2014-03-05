#include "stm32f30x_exti.h"
#include "common.h"

/*
 thanks to clive1
 */

uint32_t high_start[] = { 0, 0, 0, 0, 0, 0 };
__IO uint16_t ppm_value[] = { 0, 0, 0, 0, 0, 0 };

void init_read_pwm() {

	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIOA clock */
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
			| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Enable SYSCFG clock */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Connect EXTI0 Line to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);

	int i;
	for (i = 0; i < 6; i++) {
		/* Configure EXTI0 line */
		EXTI_InitStructure.EXTI_Line = EXTI_Line1 + i;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_TS_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void EXTI1_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line1) == SET) {
		if (GPIOD->IDR & GPIO_Pin_1) {
			ppm_value[0] = 1;
		} else {
			ppm_value[0] = 0;
		}
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_TS_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line2) == SET) {
		if (GPIOD->IDR & GPIO_Pin_2) {
			ppm_value[1] = 1;
		} else {
			ppm_value[1] = 0;
		}
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line3) == SET) {
		if (GPIOD->IDR & GPIO_Pin_3) {
			ppm_value[2] = 1;
		} else {
			ppm_value[2] = 0;
		}
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line4) == SET) {
		if (GPIOD->IDR & GPIO_Pin_4) {
			ppm_value[3] = 1;
		} else {
			ppm_value[3] = 0;
		}
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line5) == SET) {
		if (GPIOD->IDR & GPIO_Pin_5) {
			ppm_value[4] = 1;
		} else {
			ppm_value[4] = 0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
	if (EXTI_GetITStatus(EXTI_Line6) == SET) {
		if (GPIOD->IDR & GPIO_Pin_6) {
			ppm_value[5] = 1;
		} else {
			ppm_value[5] = 0;
		}

	}
	EXTI_ClearITPendingBit(EXTI_Line6);
}
