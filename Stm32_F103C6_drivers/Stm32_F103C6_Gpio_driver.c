/*
 * Stm32_F103C6_Gpio_driver.c
 *
 *  Created on: Dec 27, 2022
 *      Author: Eng. Nada Elsadany
 */

#include"Stm32_F103C6_Gpio_driver.h"
//based on pin number get the position at the register (CRL || CRH ) to write its modes
uint8_t Get_CRLH_Position(uint16_t PinNumber)
{
	switch (PinNumber)
	{
	case GPIO_PIN_0:
		return 0;
		break;

	case GPIO_PIN_1:
		return 4;
		break;

	case GPIO_PIN_2:
		return 8;
		break;

	case GPIO_PIN_3:
		return 12;
		break;

	case GPIO_PIN_4:
		return 16;
		break;

	case GPIO_PIN_5:
		return 20;
		break;

	case GPIO_PIN_6:
		return 24;
		break;

	case GPIO_PIN_7:
		return 28;
		break;

	case GPIO_PIN_8:
		return 0;
		break;

	case GPIO_PIN_9:
		return 4;
		break;

	case GPIO_PIN_10:
		return 8;
		break;

	case GPIO_PIN_11:
		return 12;
		break;

	case GPIO_PIN_12:
		return 16;
		break;

	case GPIO_PIN_13:
		return 20;
		break;

	case GPIO_PIN_14:
		return 24;
		break;

	case GPIO_PIN_15:
		return 28;
		break;
	default:
		return 0;
		break;

	}
}


/**=================================================================
 * @Fn -                    -MCAL_GPIO_INIT
 * @brief -                 -Initialize the GPIOx PINy according specified parameters in PinConfig
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -PinConfig pointer to a GPIO_PinConfig_t structure that contains
 * @retval -                -none
 * note -                   -stmf103c6 MCU has GPIO A,B,C,D,E Modules
 *                           but LQFP48 Package has only GPIO A,B, Part of C/D exported external pins
 */
/**================================================================= */
void MCAL_GPIO_Init (GPIO_Typedef* GPIOX ,GPIO_PIN_Configer_t* PinConfig)
{
	//To Init Any Pins Must Write On Port configuration register low (GPIOx_CRL) 0>>>7
	//Or register low (GPIOx_CRH) 8>>>15
	//We Determine The Position Should Write On It In  configuration register By Determine First Position Pin from "Get_CRLH_Position" function

	volatile uint32_t *ConfigureRegister = NULL;
	uint8_t   ConfigurePin=0;     //In This Variable We Store The Configure Pin
	ConfigureRegister = (PinConfig->GPIO_PinNumber < GPIO_PIN_8)? &GPIOX->CRL : &GPIOX->CRH ; // to take the address of CRL or CRH and store it in ConfigureRegister

	//Clear CNF & MODE for this pin only (0xf = 1111)
	*(ConfigureRegister) &= ~(0xf<<Get_CRLH_Position(PinConfig->GPIO_PinNumber));
	if (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_PP || PinConfig->GPIO_MODE ==GPIO_MODE_OUTPUT_OD || PinConfig->GPIO_MODE ==GPIO_MODE_OUTPUT_AF_PP || PinConfig->GPIO_MODE ==GPIO_MODE_OUTPUT_AF_OD)
		{

			//I sure The User Want Make The Pin As Output & in Case Output Should Configure Mode(Speed) & CNF(State)
			ConfigurePin = ((( (PinConfig->GPIO_MODE - 4)<<2) |(PinConfig->GPIO_OUTPUT_SPEED)) & 0x0f); //Shift 2 to Set The CNF Bits

		}

	else  //I sure The User Want Make The Pin As Input & in Case input Should Configure Mode(Speed)=00 & CNF(State)
		{
			if (PinConfig->GPIO_MODE == GPIO_MODE_Analog ||PinConfig->GPIO_MODE ==GPIO_MODE_INPUT_FLO ||  PinConfig->GPIO_MODE == GPIO_MODE_AF_INPUT)
			{
				ConfigurePin= (( ((PinConfig->GPIO_MODE )<<2) | 0x0) & 0x0f);
			}
			else  //PullUp Or PullDown
			{
				ConfigurePin= (( ((GPIO_MODE_INPUT_PU)<<2) | 0x0) & 0x0f);
				if (PinConfig->GPIO_MODE == GPIO_MODE_INPUT_PU)
				{
					GPIOX->ODR |= PinConfig->GPIO_PinNumber;
				}
				else
				{
					GPIOX->ODR &= ~(PinConfig->GPIO_PinNumber);
				}
			}
		}
	//Write On CRH Or CLH
	*(ConfigureRegister) |=( ConfigurePin << Get_CRLH_Position(PinConfig->GPIO_PinNumber) );

}


/**=================================================================
 * @Fn -                    -MCAL_GPIO_Deinit
 * @brief -                 -Reset All GPIO Registers
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @retval -                -none
 * note -                   -none
 */
/**================================================================= */
void MCAL_GPIO_Deinit(GPIO_Typedef * GPIOx)
{
	/*If MicroController Not Include Reset Controller You Should Make Reset Any Peripheral Manual By Way
	Go To All Registers In This Peripheral And Put On Reset Value
	GPIOx->CRL  |= 0x44444444;
	GPIOx->CRH  |= 0x44444444;
    //GPIOx->IDR  = (Read Only);
	GPIOx->ODR  |= 0x00000000;
	GPIOx->BRR  |= 0x00000000;
	GPIOx->LCKR |= 0x00000000;
	GPIOx->BSRR |= 0x00000000;*/
	//If MicroController Include Reset Controller We Reset Any Peripheral By It

	if(GPIOx == GPIOA)
	{
		RCC->APB2RSTR |=(1<<2);
		RCC->APB2RSTR &=~(1<<2);
	}
	else if(GPIOx == GPIOB)
	{
		RCC->APB2RSTR |=(1<<3);
		RCC->APB2RSTR &=~(1<<3);
	}
	else if(GPIOx == GPIOC)
	{
		RCC->APB2RSTR |=(1<<4);
		RCC->APB2RSTR &=~(1<<4);
	}
	else if(GPIOx == GPIOD)
	{
		RCC->APB2RSTR |=(1<<5);
		RCC->APB2RSTR &=~(1<<5);
	}
	else if(GPIOx == GPIOE)
	{
		RCC->APB2RSTR |=(1<<5);
		RCC->APB2RSTR &=~(1<<5);
	}



}

/**=================================================================
 * @Fn -                    - MCAL_GPIO_Read_Pin
 * @brief -                 -Read Specific PIN
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -PinNumber :
 * @retval -                -the input pin value (tow values based on @ref GPIO_PIN_State)
 * note -                   -none
 */
/**================================================================= */
uint8_t MCAL_GPIO_Read_Pin(GPIO_Typedef * GPIOx,uint16_t PinNumber)
{
	uint8_t PinState =0;
	if( (GPIOx->IDR & PinNumber) != (uint32_t)GPIO_PIN_FALSE )      //(GPIOx->IDR & PinNumber) :to get the value from IDR for this pin
	{
		PinState = GPIO_PIN_TRUE;
	}
	else
	{
		PinState = GPIO_PIN_FALSE;
	}
	return PinState;
}
/**=================================================================
 * @Fn -                    - MCAL_GPIO_ReadPort
 * @brief -                 -Read the input port value
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -PinNumber :
 * @retval -                -the input pin value (tow values based on @ref GPIO_PIN_State)
 * note -                   -none
 */
/**================================================================= */
uint16_t MCAL_GPIO_ReadPort(GPIO_Typedef * GPIOx)
{
	uint16_t PortState =0 ;
	PortState = (uint16_t)GPIOx->IDR;
	return PortState;

}
/**=================================================================
 * @Fn -                    - MCAL_GPIO_WritePin
 * @brief -                 -Write on specific input pin
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -PinNumber :
 * @param [in] -            -Value to write on this bit
 * @retval -                -the input pin value (tow values based on @ref GPIO_PIN_State)
 * note -                   -none
 */
/**================================================================= */
void MCAL_GPIO_WritePin(GPIO_Typedef * GPIOx,uint16_t PinNumber,uint8_t Value)
{
	if (Value !=(uint32_t)GPIO_PIN_FALSE )
	{
		GPIOx->ODR |= (uint32_t)PinNumber ;
	}
	else
	{
		GPIOx->ODR &= ~(uint32_t)PinNumber;
	}

}
/**=================================================================
 * @Fn -                    - MCAL_GPIO_WritePort
 * @brief -                 -Write on specific port
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -Value to write on this port
 * @retval -                -none
 * note -                   -none
 */
/**================================================================= */
void MCAL_GPIO_WritePort(GPIO_Typedef * GPIOx,uint16_t Value)
{
	GPIOx->ODR = (uint32_t) Value ;

}

/**=================================================================
 * @Fn -                    - MCAL_GPIO_WriteGroup
 * @brief -                 -Write on specific portion of the port
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -Vlaue to write on this port
 * @param [in] -            -Mask : bit with 1 will be affected , bits with 0 won't be affected
 * @retval -                -none
 * note -                   -none
 */
/**================================================================= */
void MCAL_GPIO_WriteGroup(GPIO_Typedef * GPIOx,uint16_t Value,uint16_t Mask)
{
	GPIOx->ODR &= ~(Mask);				// clear the group
	GPIOx->ODR |= (Value & Mask);		// put the new value
}
/**=================================================================
 * @Fn -                    - MCAL_GPIO_TogglePin
 * @brief -                 -toggle specific pin
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -PinNumber :
 * @retval -                -none
 * note -                   -none
 */
/**================================================================= */
void MCAL_GPIO_Toggle_pin(GPIO_Typedef *GPIOx , uint16_t PinNumber)
{

	GPIOx->ODR ^= (PinNumber);
}

/**=================================================================
 * @Fn -                    - MCAL_GPIO_LockPin
 * @brief -                 -Lock specific pin to be frozen the configuration of this pin
 * @param [in] -            -GPIOx : where x can be (A..E depending on device used)
 * @param [in] -            -PinNumber :
 * @retval -                -Ok if pin config is locked or Error if pin not locked
 *                           (ok & Error are defined @ref GPIO_RETURN_LOCK)
 * note -                   -none
 */
/**================================================================= */
uint8_t MCAL_GPIO_LockPin(GPIO_Typedef * GPIOx,uint16_t PinNumber)
{
	/*This register is used to lock the configuration of the port bits when a correct write sequence
	is applied to bit 16 (LCKK). The value of bits [15:0] is used to lock the configuration of the
	GPIO. During the write sequence, the value of LCKR[15:0] must not change. When the
	LOCK sequence has been applied on a port bit it is no longer possible to modify the value of
	the port bit until the next reset.
	Each lock bit freezes the corresponding 4 bits of the control register (CRL, CRH). */
	GPIOx->LCKR |=(PinNumber);
	/*Bit 16 LCKK[16]: Lock key
	This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	0: Port configuration lock key not active
	1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	LOCK key writing sequence:
	Write 1
	Write 0
	Write 1
	Read 0
	Read 1 (this read is optional but confirms that the lock is active)
	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	Any error in the lock sequence will abort the lock.*/

	/*LOCK key writing sequence:*/
	//Write 1
	GPIOx->LCKR |=(1<<16);
	//Write 0
	GPIOx->LCKR &=~(1<<16);
	//Write 1
	GPIOx->LCKR |=(1<<16);
   //Read 0
    if((uint16_t)(GPIOx->LCKR & 1<<16 ))
    {
    	return GPIO_RETURN_LOCK_DONE;
    }
    else
    {
    	return GPIO_RETURN_LOCK_ERROR;
    }

}

/* to make it work at hal
 * 1- init clock
 * 2- init gpio
 * 3- then call 2 functions at the main by the same sequence
 *
 *
 * example :
void clock_init()
{
	//enable GPIOA clock
	RCC_GPIOA_CLK_EN() ;

	//enable GPIOB clock
	RCC_GPIOB_CLK_EN() ;
	//enable GPIOC clock
	RCC_GPIOC_CLK_EN() ;

	//enable GPIOD clock
	RCC_GPIOD_CLK_EN() ;

}

void gpio_init()
{
	GPIO_PIN_Configer_t pincfg ;

	//properties of the pin
	pincfg.GPIO_PinNumber = GPIO_PIN_1 ;
	pincfg.GPIO_MODE = GPIO_MODE_INPUT_FLO ; //choose the mode from Stm32_F103C6_Gpio_driver.h
	//pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;  // if it was output pin

	MCAL_GPIO_Init (GPIOA,&pincfg);

}

 */


