/*
 * Timer.h
 *
 *  Created on: May 6, 2024
 *      Author: Eng. Nada Elsadany
 */


#ifndef INC_STM32_F103C6_TIME_DRIVER_H_
#define INC_STM32_F103C6_TIME_DRIVER_H_



#include"stdlib.h"
#include<stdint.h>

#define RCC_Base        			           0x40021000
#define TIM2_timer_Base                        0x40000000

#define RCC_APB1ENR                           *( volatile uint32_t *)(RCC_Base+0x1C)
#define RCC_APB2ENR                           *( volatile uint32_t *)(RCC_Base+0x18)





#define TIM2_CNT                              *( volatile uint32_t *)(TIM2_timer_Base+0x24)
#define TIM2_CR1                              *( volatile uint32_t *)(TIM2_timer_Base+0x00)
#define TIM2_PSC                              *( volatile uint32_t *)(TIM2_timer_Base+0x28)
#define TIM2_SR                               *( volatile uint32_t *)(TIM2_timer_Base+0x10)
#define TIM2_DIER                             *( volatile uint32_t *)(TIM2_timer_Base+0x0c)
#define TIM2_ARR                              *( volatile uint32_t *)(TIM2_timer_Base+0x2c)
#define RCC_APB1ENR                           *( volatile uint32_t *)(RCC_Base+0x1C)



/*=================Timer2======================*/
void Timer2_init(void);
void dus(int us);
void dms(int ms);

#endif /* INC_STM32_F103C6_TIME_DRIVER_H_ */
