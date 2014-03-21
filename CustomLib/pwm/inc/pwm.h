/*
 * pwm.h
 *
 *  Created on: 06/set/2013
 *      Author: mauro
 */


#ifndef PWM_H_
#define PWM_H_

#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "stm32f30x_conf.h"

//BECAUSE 800 will stop engines
#define PWM_STOP 800UL
#define PWM_MIN 1000UL
#define PWM_MAX 2500UL

void init_pwm_tim4(void);
void init_pwm_tim8(void);

//extern void setPwm(uint16_t, uint16_t, uint16_t, uint16_t);
void setEngine(uint16_t msPwm);
void setServoSx(uint16_t msPwm);
void setServoDx(uint16_t msPwm);

#endif /* PWM_H_ */
