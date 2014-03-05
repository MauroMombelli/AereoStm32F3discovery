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

void handle_pwm_change(uint8_t i){
	uint8_t stato = GPIOD->IDR & (1<<i);

	i--;//to match array index

	if (stato){//pin == HIGH
		if (high_start[i] == 0){
			high_start[i] = micros();
		}
	}else{//pin == LOW
		if (high_start[i] != 0){
			ppm_value[i] = micros() - high_start[i];
			high_start[i] = 0;
		}
	}
}

void EXTI1_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line1) == SET) {
		handle_pwm_change(1);
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_TS_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line2) == SET) {
		handle_pwm_change(2);
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line3) == SET) {
		handle_pwm_change(3);
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line4) == SET) {
		handle_pwm_change(4);
	}
	//we need to clear line pending bit manually
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void) {
	//Check if EXTI_Line0 is asserted
	if (EXTI_GetITStatus(EXTI_Line5) == SET) {
		handle_pwm_change(5);
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
	if (EXTI_GetITStatus(EXTI_Line6) == SET) {
		handle_pwm_change(6);
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
}
