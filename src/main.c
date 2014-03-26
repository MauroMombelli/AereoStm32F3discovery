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
//#define USE_RX
#define USE_QUATERNION
#define DELAY	(1000)
#define gyroToRad (2293.76f/32768)*0.0174532925f


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
	/* SysTick end of count event each 1us */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config((RCC_Clocks.HCLK_Frequency / 1000000UL));

	SystemCoreClockUpdate();

	/* Initialize LEDs and User Button available on STM32F3-Discovery board */
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDInit(LED7);
	STM_EVAL_LEDInit(LED8);
	STM_EVAL_LEDInit(LED9);
	STM_EVAL_LEDInit(LED10);

	//LED4 == init phase
	STM_EVAL_LEDToggle(LED4);

	//printf("\fstarting pwm\n\r");
	init_pwm_tim4();
	init_pwm_tim8();

#ifdef USE_QUATERNION
	Gyro_Config();
	Compass_Config();

	int16_t gyro[3] = {0}, acc[3] = {0}, magne[3] = {0};
	float quaternion[4];
	float ypr[3];
#endif

	//printf("\fstarting read PPM\n\r");
#ifdef USE_RX
	init_read_pwm();
#endif

	//printf("\fstarting loop\n\r");

	uint32_t tempo_heart_beat = 0, tempo_pwm = 0;

	//end init phase
	STM_EVAL_LEDToggle(LED4);

	while (1) {

		/* Toggle LD3 every second; means everything is ok :) */
		if (micros() - tempo_heart_beat > DELAY * 1000UL) {
			STM_EVAL_LEDToggle(LED3);
			tempo_heart_beat = micros();
		}

#ifdef USE_QUATERNION
		Compass_ReadAcc(acc);
		Compass_ReadMag(magne);
		if (Gyro_ReadAngRate(gyro)){
			freeIMUUpdate(-gyro[0]*gyroToRad,-gyro[1]*gyroToRad, gyro[2]*gyroToRad, -acc[1], acc[0], acc[2], -magne[2], magne[0], magne[1]);
		}
#endif

		if (micros() - tempo_pwm > 50000UL) {//every 10ms
			uint32_t pwmSx = 1500, pwmDx = 1500, pwmEngine = ppm_value[4];

			//QUATERNION CORRECION
#ifdef USE_QUATERNION
			getQuaternion(quaternion);
			quaternionToYawPitchRoll(ypr);

			//TODO: test

			int16_t tmp = ypr[1] * 300; //ROLL 0 = horizontal
			pwmSx += tmp;
			pwmDx += tmp;

			tmp = ypr[2] * 300; //PITCH 0 = horizontal
			pwmSx += tmp;
			pwmDx -= tmp;

#endif

			//RX command
#ifdef USE_RX
			if (ppm_value[0] != 0 && ppm_value[2] != 0){
				uint32_t tmp =ppm_value[0]-1500; // SX/DX

				pwmSx += tmp;
				pwmDx += tmp;

				ppm_value[0] = 0;

				tmp = ppm_value[2]-1500UL; // UP/DOWN
				pwmSx += tmp;
				pwmDx -= tmp;

				ppm_value[2] = 0;
			}
#endif

			setServoSx(pwmSx);//because servo are mountet specular!! yay!
			setServoDx(pwmDx);
			setEngine(pwmEngine);
			tempo_pwm = micros();
		}
	}

	/* Program will never run to this line */
	return 0;
}
