/*
 * Stm32_F103C6_EXTI_driver.h
 *
 *  Created on: feb 8, 2023
 *      Author: Eng. Nada Elsadany
 */

#ifndef INC_STM32_F103C6_RCC_DRIVER_H_
#define INC_STM32_F103C6_RCC_DRIVER_H_


#include"Stm32_F103C6_Gpio_driver.h"

#define HSI_RC_CLK (uint32_t)8000000
#define HSE_CLK    (uint32_t)16000000 //need to be calculated


uint32_t MCAL_RCC_GetSys_freq(void);
uint32_t MCAL_RCC_GetHCLK_freq(void);
uint32_t MCAL_RCC_GetPCLK1_freq(void);
uint32_t MCAL_RCC_GetPCLK2_freq(void);


#endif /* INC_STM32_F103C6_EXIT_DRIVER_H_ */
