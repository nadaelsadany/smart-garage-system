/*
 * Stm32_F103C6_EXTI_driver.c
 *
 *  Created on: feb 8, 2023
 *      Author: Eng. Nada Elsadany
 */

#include"Stm32_F103C6_EXTI_driver.h"
#include"Stm32_F103C6_Gpio_driver.h"

/**================================================================= */

/* GENARIC MACROS */
/**================================================================= */

void(* GP_IRQ_CALLBACK[15])(void) ; /*Array Of Pointer To function Tack void and return void
                                   (This Pointer Will Receive The Function From user To Execute when Interrupt in happen */

#define AFIO_GPIO_EXTI_MAPPING(x) 		((x=GPIOA)?0:\
											(x=GPIOB)?1:\
												(x=GPIOC)?2:\
													(x=GPIOD)?3:0)

void ENABLE_NVIC(uint8_t IRQ)
{
	switch(IRQ)
	{
	case 0:
		NVIC_IRQ6_EXTI0_ENABLE();
		break;
	case 1:
		NVIC_IRQ7_EXTI1_ENABLE();
		break;
	case 2:
		NVIC_IRQ8_EXTI2_ENABLE();
		break;
	case 3:
		NVIC_IRQ9_EXTI3_ENABLE();
		break;
	case 4:
		NVIC_IRQ10_EXTI4_ENABLE();
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		 NVIC_IRQ23_EXTI9_ENABLE();
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI15_ENABLE()  ;
		break;

	}
}


void DISABLE_NVIC(uint8_t IRQ)
{
	switch(IRQ)
	{
	case 0:
		NVIC_IRQ6_EXTI0_DISABLE();
		break;
	case 1:
		NVIC_IRQ7_EXTI1_DISABLE();
		break;
	case 2:
		NVIC_IRQ8_EXTI2_DISABLE();
		break;
	case 3:
		NVIC_IRQ9_EXTI3_DISABLE();
		break;
	case 4:
		NVIC_IRQ10_EXTI4_DISABLE();
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI5_DISABLE();
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI10_DISABLE();
		break;

	}
}

void UPdate_EXTI(EXTI_PinConfig_t * EXTI_CONFIG)
{
	//configure GPIO pin to be AF(floating input)
	GPIO_PIN_Configer_t * pincfg=NULL ;
	pincfg->GPIO_PinNumber = EXTI_CONFIG->EXTI_PIN.GPIO_PIN_NUMBER ;
	pincfg->GPIO_MODE = GPIO_MODE_INPUT_FLO ;
	MCAL_GPIO_Init(EXTI_CONFIG->EXTI_PIN.GPIO_PORT,&pincfg);
	//=================================================================
	/*2-Update AFIO To Route Between EXTI Line With PORT A,B,C,D*/
	/*This Line Determine The Register For (EXTICR1,EXTICR2,EXTICR3,EXTICR4)*/
	uint8_t  AFIO_EXTICR_index  = EXTI_CONFIG->EXTI_PIN.GPIO_PIN_NUMBER /4 ;
	uint8_t  AFIO_EXTICR_positionn = (EXTI_CONFIG->EXTI_PIN.GPIO_PIN_NUMBER % 4)*4 ;

	/*Clear This Four Bits Which I Chose It*/
	AFIO->EXTICR[AFIO_EXTICR_index] &= ~(0xf <<AFIO_EXTICR_index);
	/*set This Four Bits Which I Chose It*/
	AFIO->EXTICR[AFIO_EXTICR_index] |=((AFIO_GPIO_EXTI_MAPPING(EXTI_CONFIG->EXTI_PIN.GPIO_PORT)&0xf)<<AFIO_EXTICR_index) ;

	/*=============================================================*/

	/*3- Update Rising && Falling Edge From (EXTI_FTSR & EXTI_SWIER)*/
	/*First Clear Falling & Rising Edge*/
	EXTI->FTSR &=~(1<< EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER );
	EXTI->RTSR &=~(1<< EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER );

	/*Update*/
	if(EXTI_CONFIG->TRIGGRE_CASE == EXTI_TRIGGRE_RISING)
	{
		EXTI->RTSR |=(1<< EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER );
	}
	else if(EXTI_CONFIG->TRIGGRE_CASE == EXTI_TRIGGRE_FALLING)
	{
		EXTI->FTSR |=(1<< EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER );
	}
	else if(EXTI_CONFIG->TRIGGRE_CASE == EXTI_TRIGGRE_RISING_AND_FALLING)
	{
		EXTI->FTSR |=(1<< EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER );
		EXTI->RTSR |=(1<< EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER );
	}
	/*=============================================================*/
	//4- Update IRQ Handling CallBack*/
	GP_IRQ_CALLBACK[EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER] = EXTI_CONFIG->P_IRQ_CALLBACK ;

	//5-Enable / Disable IRQ (Interrupt mask register (EXTI_IMR))  & NVIC
		/*MRx: Interrupt Mask on line x
		0: Interrupt request from Line x is masked
		1: Interrupt request from Line x is not masked*/

		//EXTI_IMR
		if(EXTI_CONFIG->IRQ_EN == EXTI_IRQ_ENABLE)
		{
			EXTI->IMR |=(1<<(EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER));
			ENABLE_NVIC(EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER);
		}
		else//EXTI_CONFIG->IRQ_EN == EXTI_IRQ_DISABLE
		{
			EXTI->IMR &=~(1<<(EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER));
			DISABLE_NVIC(EXTI_CONFIG->EXTI_PIN.EXTI_INPUT_LINE_NUMBER);
		}




}
/**=================================================================
 * @Fn -                    -MCAL_EXTI_GPIO_INIT
 * @brief -                 -Initialize the EXTI For Specific GPIO Pin and specify Mask/Trigger Condition and IRQ CallBack
 * @param [in] -            -EXTI_CONFIG : where this set by this reference @Ref EXTI_IRQ_EN_define and @Ref EXTI_TRIGGRE_define
 * @retval -                -none
 * note -                   -stmf103c6 MCU has GPIO A,B,C,D,E Modules
 *                           but LQFP48 Package has only GPIO A,B, Part of C/D exported external pins
 */
/**================================================================= */
void MCAL_EXTI_GPIO_INIT(EXTI_PinConfig_t * EXTI_CONFIG)
{
	UPdate_EXTI(&EXTI_CONFIG);

}

/**=================================================================
 * @Fn -                    - MCAL_EXTI_GPIO_DEINIT
 * @brief -                 -reset EXIT registers & NVIC corresponding IRQ mask
 * @param [in] -            -non
 * @retval -                -non
 * note -                   -none
 */
/**================================================================= */
void MCAL_EXTI_GPIO_DEINIT(void)
{
	//reset EXIT registers
	EXTI->IMR 	= 0x00000000;
	EXTI->EMR 	= 0x00000000;
	EXTI->FTSR 	= 0x00000000;
	EXTI->RTSR	= 0x00000000;
	EXTI->SWIER = 0x00000000;
	EXTI->PR 	= 0xffffffff; // write 1 to clear PR register

	//disable NVIC corresponding IRQ mask
	NVIC_IRQ6_EXTI0_DISABLE();
	NVIC_IRQ7_EXTI1_DISABLE();
	NVIC_IRQ8_EXTI2_DISABLE();
	NVIC_IRQ9_EXTI3_DISABLE();
	NVIC_IRQ10_EXTI4_DISABLE();
	NVIC_IRQ23_EXTI5_DISABLE();
	NVIC_IRQ40_EXTI10_DISABLE();
	// from 5 to 9 & from 10 to 15 have the same bit at registers

}

/**=================================================================
 * @Fn -                    -MCAL_EXTI_GPIO_UPDATE
 * @brief -                 -Initialize the EXTI For Specific GPIO Pin and specify Mask/Trigger Condition and IRQ CallBack
 * @param [in] -            -EXTI_CONFIG : where this set by this reference @Ref EXTI_IRQ_EN_define and @Ref EXTI_TRIGGRE_define
 * @retval -                -none
 * note -                   -stmf103c6 MCU has GPIO A,B,C,D,E Modules
 *                           but LQFP48 Package has only GPIO A,B, Part of C/D exported external pins
 */
/**================================================================= */

void MCAL_EXTI_GPIO_UPDATE(EXTI_PinConfig_t * EXTI_CONFIG)
{
	UPdate_EXTI(&EXTI_CONFIG);

}



/*
 * ===============================================================
 *
 *                      ISR Function Definitions
 *
 * ===============================================================
 */

void EXTI0_IRQHandler (void)
{
	// Clear Pending register
	EXTI->PR |=(1<<0);
	//Call function
	GP_IRQ_CALLBACK[0]();
}

void EXTI1_IRQHandler (void)
{
	// Clear Pending register
	EXTI->PR |=(1<<1);
	//Call function
	GP_IRQ_CALLBACK[1]();
}

void EXTI2_IRQHandler (void)
{
	// Clear Pending register
	EXTI->PR |=(1<<2);
	//Call function
	GP_IRQ_CALLBACK[2]();
}

void EXTI3_IRQHandler (void)
{
	// Clear Pending register
	EXTI->PR |=(1<<3);
	//Call function
	GP_IRQ_CALLBACK[3]();
}

void EXTI4_IRQHandler (void)
{
	// Clear Pending register
	EXTI->PR |=(1<<4);
	//Call function
	GP_IRQ_CALLBACK[4]();
}


void EXTI9_5_IRQHandler (void)
{
	if( EXTI->PR & 1<<5 )	{ EXTI->PR |=( 1 << 5 ) ;		GP_IRQ_CALLBACK[5]() ; }
	if( EXTI->PR & 1<<6 )	{ EXTI->PR |=( 1 << 6 ) ;		GP_IRQ_CALLBACK[6]() ; }
	if( EXTI->PR & 1<<7 )	{ EXTI->PR |=( 1 << 7 ) ;		GP_IRQ_CALLBACK[7]() ; }
	if( EXTI->PR & 1<<8 )	{ EXTI->PR |=( 1 << 8 ) ;		GP_IRQ_CALLBACK[8]() ; }
	if( EXTI->PR & 1<<9 )	{ EXTI->PR |=( 1 << 9 ) ;		GP_IRQ_CALLBACK[9]() ; }
}


void EXTI15_10_IRQHandler (void)
{
	if( EXTI->PR & 1<<10 )	{ EXTI->PR |=( 1 << 10 ) ;		GP_IRQ_CALLBACK[10]() ; }
	if( EXTI->PR & 1<<11 )	{ EXTI->PR |=( 1 << 11 ) ;		GP_IRQ_CALLBACK[11]() ; }
	if( EXTI->PR & 1<<12 )	{ EXTI->PR |=( 1 << 12 ) ;		GP_IRQ_CALLBACK[12]() ; }
	if( EXTI->PR & 1<<13 )	{ EXTI->PR |=( 1 << 13 ) ;		GP_IRQ_CALLBACK[13]() ; }
	if( EXTI->PR & 1<<14 )	{ EXTI->PR |=( 1 << 14 ) ;		GP_IRQ_CALLBACK[14]() ; }
	if( EXTI->PR & 1<<15 )	{ EXTI->PR |=( 1 << 15 ) ;		GP_IRQ_CALLBACK[15]() ; }
}
