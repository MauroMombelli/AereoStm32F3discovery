/**
 *****************************************************************************
 **
 **  File        : main.c
 **
 **  Abstract    : main function.
 **
 **  Functions   : main
 **
 **  Environment : Atollic TrueSTUDIO(R)
 **
 **  Distribution: The file is distributed ���as is,��� without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. Distribution of this file (unmodified or modified) is not
 **  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
 **  rights to distribute the assembled, compiled & linked contents of this
 **  file as part of an application binary file, provided that it is built
 **  using the Atollic TrueSTUDIO(R) toolchain.
 **
 *****************************************************************************
 */

/* Includes */
#include <main.h>

/* Private typedef */

/* Private define  */
#define DELAY	(1000)

/* Private macro */

/* Private variables */
float a, b, c, d;
RCC_ClocksTypeDef RCC_Clocks;
TIM_ICInitTypeDef TIM_ICInitStructure;
__IO uint32_t CCR1;

/* Private function prototypes */

/* Private functions */

/* Global variables */

/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
int main(void) {
	//uint32_t ii;

	/* SysTick end of count event each 1us */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config((RCC_Clocks.HCLK_Frequency / 1000000UL));

	SystemCoreClockUpdate();
	//ii = SystemCoreClock;   /* This is a way to read the System core clock */

	/* Initialize LEDs and User Button available on STM32F3-Discovery board */
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDInit(LED7);
	STM_EVAL_LEDInit(LED8);
	STM_EVAL_LEDInit(LED9);
	STM_EVAL_LEDInit(LED10);

	//USART2_Init(115200, 1);//1 == interrupt, 0 == no interrupt
	//printf("\fUSART2 initialized\n\r");

	STM_EVAL_LEDToggle(LED4);

	//printf("\fstarting pwm\n\r");
	init_pwm_tim4(); //50Hz pwm
	init_pwm_tim8();

	STM_EVAL_LEDToggle(LED5);

	//printf("\fstarting read PPM\n\r");
	init_read_pwm();

	//printf("\fstarting loop\n\r");
	uint16_t pwmTest = PWM_MIN;

	uint32_t tempo_heart_beat = 0, tempo_pwm = 0;

	while (1) {
		/*
		 int i = 0;
		 for (i=0; i < 6; i++){
		 printf("PPM readed: %" PRIu32 " \n\r", ppm_value[i]);
		 }
		 */
		/*
		 setPwm(pwmTest, ppm_value[1], ppm_value[2], ppm_value[3]);
		 */
		/*
		 int i;
		 for (i=0; i < 6; i++){
		 if (ppm_value[i] == 1){
		 STM_EVAL_LEDOn(LED4+i);
		 }else{
		 STM_EVAL_LEDOff(LED4+i);
		 }
		 }
		 */

		/* Toggle LD3; means everything is ok :) */
		if (micros() - tempo_heart_beat > DELAY * 1000) {
			STM_EVAL_LEDToggle(LED3);
			tempo_heart_beat = micros();
		}

		if (micros() - tempo_pwm > 1000000UL) {
			pwmTest += 100;

			if (pwmTest > PWM_MAX) {
				pwmTest = PWM_MIN;
			}

			setServoSx(pwmTest);
			setServoDx(pwmTest);
			setEngine(pwmTest);
			tempo_pwm = micros();
		}
	}

	/* Program will never run to this line */
	return 0;
}
