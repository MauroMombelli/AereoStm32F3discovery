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
int main(void)
{
	//uint32_t ii;

	/* Example use SysTick timer and read System core clock */
	SysTick_Config(72);  /* 1 us if clock frequency 72 MHz */

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

	USART2_Init(115200, 1);//1 == interrupt, 0 == no interrupt
    printf("\fUSART2 initialized\n\r");

    printf("\fstarting pwm\n\r");
    init_pwm_tim4(); //50Hz pwm

    printf("\fstarting read PPM\n\r");
    external_interrupt_init();


    printf("\fstarting loop\n\r");
	while (1)
	{

		int i = 0;
		for (i=0; i < 6; i++){
			printf("PPM readed: %" PRIu32 " \n\r", ppm_value[i]);
		}

		setPwm(ppm_value[0], ppm_value[1], ppm_value[2], ppm_value[3]);


		/* Toggle LD3; means everything is ok :) */
		STM_EVAL_LEDToggle(LED3);
		_delay_ms(DELAY);
	}

	/* Program will never run to this line */
	return 0;
}
