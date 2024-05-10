/*
 * Stm32_F103C6_UART.c
 *
 *  Created on: Sep 8, 2023
 *      Author: Eng. Nada Elsadany
 */
#include"Stm32_F103C6_UART_driver.h"


/*==========================================================*/
/*Generic Variables*/
/*==========================================================*/


uint32_t PCLK ,BRR;
USART_Config * GP_USART_Config = NULL ;


/*==========================================================*/
/*Generic Functions*/
/*==========================================================*/







/* ==============================================================
 *              APIs Supported by "MCAL USART DRIVER"
 * ===============================================================*/


/**=================================================================
 * @Fn -                    -UART_MCAL_GPIO_Set_Pins
 * @brief -                 -select the pins based on the num of uart and config them
 * @param [in] -            -USARTx : where x can be (1,2,3)
 * @retval -                -none
 * note -                   -none
 */
/**================================================================= */

void MCAL_UART_GPIO_Set_Pins(USART_Typedef * USARTx)
{

	GPIO_PIN_Configer_t PinCfg;

	if(USARTx == USART1)
	{
		//PA9  TX
		//PA10 RX
		//PA11 CTS
		//PA12 RTS

		//TX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_9 ;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//RX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_10 ;
		PinCfg.GPIO_MODE = GPIO_MODE_AF_INPUT ;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		if(GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS ||GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS_AND_RTS )

		{
			//CTX PIN
			PinCfg.GPIO_PinNumber =GPIO_PIN_11 ;
			PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

		if(GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_RTS ||GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS_AND_RTS )

		{
		//RTX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_12 ;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
		MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}
	else if(USARTx == USART2)
	{
		//PA2  TX
		//PA3  RX
		//PA0  CTS
		//PA1  RTS

		//TX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_2 ;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//RX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_3 ;
		PinCfg.GPIO_MODE = GPIO_MODE_AF_INPUT ;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		if(GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS ||GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS_AND_RTS )

		{
			//CTX PIN
			PinCfg.GPIO_PinNumber =GPIO_PIN_0 ;
			PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

		if(GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_RTS ||GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS_AND_RTS )

		{
		//RTX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_1 ;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
		MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

	}
	else if(USARTx == USART3)
	{
		//PB10 TX
		//PB11 RX
		//PB13 CTS
		//PB14 RTS

		//TX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_10 ;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		//RX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_11 ;
		PinCfg.GPIO_MODE = GPIO_MODE_AF_INPUT ;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		if(GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS ||GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS_AND_RTS )

		{
			//CTX PIN
			PinCfg.GPIO_PinNumber =GPIO_PIN_13 ;
			PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}

		if(GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_RTS ||GP_USART_Config->HW_FlowCtrl == USART_HW_FlowCtrl_CTS_AND_RTS )

		{
		//RTX PIN
		PinCfg.GPIO_PinNumber =GPIO_PIN_14 ;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
		}

	}

}

/**=================================================================
 * @Fn -                    -MCAL_UART_INIT
 * @brief -                 -Initialize the USARTx according specified parameters in USART_Config
 * @param [in] -            -USARTx : where x can be (1,2,3 depending on device used)
 * @param [in] -            -UART_Config pointer to a UART_Config structure that contains
 * @retval -                -none
 * note -                   -none
 *
 */
/**================================================================= */

void MCAL_UART_Init(USART_Typedef * USARTx ,USART_Config * USART_Config)
{

	GP_USART_Config = USART_Config ;

	//Enable Clock For Given USART Peripheral
	if (USARTx == USART1)
	{
		RCC_USART1_CLK_EN();
	}
	else if (USARTx == USART2)
	{
		RCC_USART2_CLK_EN();
	}
	else if (USARTx == USART3)
	{
		RCC_USART3_CLK_EN();
	}


	//Enable UART Module Bit 13 UE: USART enable
	USARTx->CR1 |=(1<<13);

	//Enable TX RX according to the USART_MODE configuration item
	USARTx->CR1 |= USART_Config->USART_MODE;

	//PayLoad Length
	USARTx->CR1 |= USART_Config->PayLoad_Length;

	//BaudRate
	USARTx->CR1 |= USART_Config->BaudRate;

	//Parity
	USARTx->CR1 |= USART_Config->Parity ;

	//Stop_Length
	USARTx->CR1 |= USART_Config->Stop_Length ;


	//HW_FlowCtrl
	USARTx->CR1 |= USART_Config->HW_FlowCtrl ;


	//IRQ_Enable
	USARTx->CR1 |= USART_Config->IRQ_Enable ;


	/*Configuration of BRR(BaudRate Register)*/
	//PCLK1 for USART2,3
	//PCLK2 for USART1
	if (USARTx == USART1)
	{
		PCLK = MCAL_RCC_GetPCLK2_freq();
	}
	else
	{
		PCLK = MCAL_RCC_GetPCLK1_freq();
	}


	BRR = USART_BRR_REGISTER(PCLK,USART_Config->BaudRate);
	USARTx->BRR = BRR ;
	/*Enable/Disable interrupt*/
	//USART_CR1
	if( USART_Config->IRQ_Enable != USART_IRQ_Enable_NONE ) //this mean user enable interrupt
	{
		USARTx->CR1 |= USART_Config->IRQ_Enable;
		//Enable interrupt for NVIC for USARTx IRQ
		if(USARTx == USART1)
		{
			NVIC_IRQ37_USART1_ENABLE();

		}
		else if(USARTx == USART2)
		{
			NVIC_IRQ38_USART2_ENABLE();
		}
		else if(USARTx == USART3)
		{
			NVIC_IRQ39_USART3_ENABLE();
		}

	}
	MCAL_UART_GPIO_Set_Pins( USARTx);

}


/**=================================================================
 * @Fn -                    -MCAL_UART_DeINIT
 * @brief -                 -Reset All USART Registers
 * @param [in] -            -USARTx : where x can be (1,2,3)
 * @retval -                -none
 * note -                   -none
 */
/**================================================================= */
void MCAL_UART_DeInit(USART_Typedef * USARTx)
{

	if(USARTx == USART1)
	{
		RCC_USART1_RESET();
		NVIC_IRQ37_USART1_DISABLE();

	}
	else if(USARTx == USART2)
	{
		RCC_USART2_RESET();
		NVIC_IRQ38_USART2_DISABLE();
	}
	else if(USARTx == USART3)
	{
		RCC_USART3_RESET();
		NVIC_IRQ39_USART3_DISABLE();
	}
}
/**=================================================================
 * @Fn -                    -MCAL_UART_INIT
 * @brief -                 -Initialize the USARTx according specified parameters in USART_Config
 * @param [in] -            -USARTx : where x can be (1,2,3 depending on device used)
 * @param [in] -            -pTXBuffer pointer to a UART_Buffer to store data in it
 * @param [in]              -PollingEn :to check which kind of transmitting  data polling or interrupt
 * @retval -                -none
 * note -                   -none
 *
 */
/**================================================================= */

void MCAL_UART_Send_Data(USART_Typedef * USARTx , uint16_t* pTXBuffer ,enum PollingMechism PollingEn)
{
	//Wait until TXE flag is set in SR
	if(PollingEn == PollingEnable)
		while(! (USARTx->SR & (1<<7)) );
	//When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
	//the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
	//because it is replaced by the parity.
	//When receiving with the parity enabled, the value read in the MSB bit is the received parity
	//bit.

	if (GP_USART_Config->PayLoad_Length == USART_PayLoad_Length_9B)
	{
		USARTx->DR = (*pTXBuffer & (uint16_t)0x01FF) ;
	}
	else
	{
		USARTx->DR = (*pTXBuffer & (uint8_t)0xFF) ;
	}

}
/**=================================================================
 * @Fn -                    -MCAL_UART_INIT
 * @brief -                 -Initialize the USARTx according specified parameters in USART_Config
 * @param [in] -            -USARTx : where x can be (1,2,3 depending on device used)
 * @param [in] -            -pTXBuffer pointer to a UART_Buffer to store data in it
 * @param [in]              -PollingEn :to check which kind of receiving data polling or interrupt
 * @retval -                -none
 * note -                   -none
 *
 */
/**================================================================= */
void MCAL_UART_Recieve_Data(USART_Typedef * USARTx , uint16_t* pTXBuffer ,enum PollingMechism PollingEn)
{

	//Wait until RXE flag is set in SR
	if(PollingEn == PollingEnable)
		while(! (USARTx->SR & (1<<5)) );

	if (GP_USART_Config->PayLoad_Length == USART_PayLoad_Length_9B)
	{
		if (GP_USART_Config->Parity == USART_Parity_NONE)
		{
			*((uint16_t*)pTXBuffer ) = USARTx->DR ;
		}
		else
		{
			*((uint16_t*) pTXBuffer ) = (USARTx->DR & (uint8_t)0xFF) ;
		}


	}
	else if (GP_USART_Config->PayLoad_Length == USART_PayLoad_Length_8B)
	{
		if (GP_USART_Config->Parity == USART_Parity_NONE)
		{
			*((uint16_t*)pTXBuffer ) =(USARTx->DR & (uint8_t)0xFF) ;
		}
		else
		{
			*((uint16_t*) pTXBuffer ) = (USARTx->DR & (uint8_t)0x7F) ;
		}

	}

}
void MCAL_UART_WAIT_TC(USART_Typedef* USARTx)
{
	//check tc flag tell it become 1 to can transmite  again
	while(! (USARTx->SR & (1<<6)) );
}


void USART1_IRQHandler(void)
{
	GP_USART_Config->P_IRQ_Call_Back();
}

void USART2_IRQHandler(void)
{
	GP_USART_Config->P_IRQ_Call_Back();
}

void USART3_IRQHandler(void)
{
	GP_USART_Config->P_IRQ_Call_Back();
}
