/*
 * Stm32_F103C6_EXTI_driver.c
 *
 *  Created on: feb 8, 2023
 *      Author: Eng. Nada Elsadany
 */

#include"Stm32_F103C6_RCC_driver.h"

/*Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
	Set and cleared by software to control the division factor of the APB low-speed clock
	(PCLK1).
	Warning: the software has to set correctly these bits to not exceed 36 MHz on this domain.
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16*/
const uint8_t APBPreSctable [8U]= {0,0,0,0,1,2,3,4};
/*Bits 7:4 HPRE: AHB prescaler
	Set and cleared by software to control the division factor of the AHB clock.
	0xxx: SYSCLK not divided
	1000: SYSCLK divided by 2
	1001: SYSCLK divided by 4
	1010: SYSCLK divided by 8
	1011: SYSCLK divided by 16
	1100: SYSCLK divided by 64
	1101: SYSCLK divided by 128
	1110: SYSCLK divided by 256
	1111: SYSCLK divided by 512*/
const uint8_t AHBPreSctable [16U]= {0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9};
uint32_t MCAL_RCC_GetSys_freq(void)
{
	/*Bits 1:0 SW: System clock switch
		Set and cleared by software to select SYSCLK source.
		Set by hardware to force HSI selection when leaving Stop and Standby mode or in case of
		failure of the HSE oscillator used directly or indirectly as system clock (if the Clock Security
		System is enabled).
		00: HSI selected as system clock
		01: HSE selected as system clock
		10: PLL selected as system clock
		11: not allowed*/
	switch ((RCC->CFGR>>2) & 0b11)
	{
	case 0:
		return HSI_RC_CLK;
		break;
	case 1 :
		return HSE_CLK;  //need to be calculated again
		break;
	case 2 :
		return 16000000; //need to be calculated again
		break;
	}


}
uint32_t MCAL_RCC_GetHCLK_freq(void)
{
	/*Bits 7:4 HPRE: AHB prescaler
		Set and cleared by software to control the division factor of the AHB clock.
		0xxx: SYSCLK not divided
		1000: SYSCLK divided by 2
		1001: SYSCLK divided by 4
		1010: SYSCLK divided by 8
		1011: SYSCLK divided by 16
		1100: SYSCLK divided by 64
		1101: SYSCLK divided by 128
		1110: SYSCLK divided by 256
		1111: SYSCLK divided by 512*/
	//bit 4: 7 of cfgr give me the prescaler of the hcl (ahb prescaler)
	 return (MCAL_RCC_GetSys_freq()>>AHBPreSctable[(RCC->CFGR>>4) & 0xf] );
}
uint32_t MCAL_RCC_GetPCLK1_freq(void)
{
	/*Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
	Set and cleared by software to control the division factor of the APB low-speed clock
	(PCLK1).
	Warning: the software has to set correctly these bits to not exceed 36 MHz on this domain.
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16*/
	//bit 8 : 10 of cfgr give me the prescaler of the clock1
	 return (MCAL_RCC_GetHCLK_freq()>>APBPreSctable[(RCC->CFGR>>8) & 0b111] );
}
uint32_t MCAL_RCC_GetPCLK2_freq(void)
{
	/*Bits 13:11 PPRE2: APB high-speed prescaler (APB2)
	Set and cleared by software to control the division factor of the APB high-speed clock
	(PCLK2).
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16*/
	//bit 11 :13  of cfgr give me the prescaler of the clock2
	 return (MCAL_RCC_GetHCLK_freq()>>APBPreSctable[(RCC->CFGR>>11) & 0b111] );
}

