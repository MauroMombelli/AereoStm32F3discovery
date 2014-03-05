/*
 * common.c
 *
 *  Created on: Nov 14, 2012
 *      Author: andrei
 */


#include "common.h"


__IO uint32_t timer_us = 1000;


/**
 **===========================================================================
 **
 **  Abstract: SysTick interrupt handler
 **
 **===========================================================================
 */
void SysTick_Handler(void)
{
	timer_us ++;
}

uint32_t micros(){
	return timer_us;
}
