/*
 * read_ppm.h
 *
 *  Created on: Mar 1, 2014
 *      Author: mauro
 */

#ifndef READ_PWM_H_
#define READ_PWM_H_

extern uint16_t ppm_value[];
extern void init_read_pwm(void);
void handle_pwm_change(uint8_t);

#endif /* READ_PPM_H_ */
