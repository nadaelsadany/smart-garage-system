/*
 * Stm32_F103C6_UART.c
 *
 *  Created on: Sep 8, 2023
 *      Author: Eng. Nada Elsadany
 */
#include"Stm32_F103C6_SPI_driver.h"

/*==========================================================*/
/*Generic Variables*/
/*==========================================================*/
SPI_Config_t * Global_SPI_Config[2]={	NULL,NULL };
SPI_Config_t  Global_SPI_Config1 ;
SPI_Config_t  Global_SPI_Config2 ;

/*==========================================================*/
/*Generic Macros*/
/*==========================================================*/

#define SPI1_INDEX               0
#define SPI2_INDEX               1

#define SPI_SR_TXE               1<<1
#define SPI_SR_RXNE              1<<0





/* ==============================================================
 *              APIs Supported by "MCAL SPI DRIVER"
 * ===============================================================*/

/**=================================================================
 * @Fn -                    -MCAL_SPI_INIT
 * @brief -                 -INIT SPI Protocol Registers according SPI_Config_t
 * @param [in] -            -SPIx : where x can be (1..2 depending on device used)
 * @param [in] -            -SPI_Config : This is configuration which user will pass it
 * @retval -                -none
 * note -                   -Support for SPI Full Dublex Master/Slave Only & NSS HW/SW
 */
/**================================================================= */

void MCAL_SPI_INIT(SPI_Typedef* SPIx,SPI_Config_t* SPI_Config)
{

	uint16_t TempReg_CR1 = 0;
	uint16_t TempReg_CR2 = 0;

	//Enable Clock For Given SPI Peripheral
	if (SPIx == SPI1)
	{
		Global_SPI_Config1 = * SPI_Config ;
		Global_SPI_Config[SPI1_INDEX]=&Global_SPI_Config1;
		RCC_SPI1_CLK_EN();
	}
	else if (SPIx == SPI2)
	{
		Global_SPI_Config2 = * SPI_Config ;
		Global_SPI_Config[SPI2_INDEX]=&Global_SPI_Config2;
		RCC_SPI2_CLK_EN();
	}
	// Enable SPI CR1 : Bit 6 SPE :SPI Enable
	TempReg_CR1 =(0x1U<< 6) ;

	//master or slave
	TempReg_CR1 |= SPI_Config->Device_Mode ;

	//Communication_Mode
	TempReg_CR1 |= SPI_Config->Communication_Mode ;

	//Frame_Format
	TempReg_CR1 |= SPI_Config->Frame_Format ;

	//Date_Size
	TempReg_CR1 |= SPI_Config->Date_Size ;

	//CLK_Polarity
	TempReg_CR1 |= SPI_Config->CLK_Polarity ;

	//CLK_Phase
	TempReg_CR1 |= SPI_Config->CLK_Phase ;

	/****************************NSS(chip selector )***************************/

	if (SPI_Config->NSS==SPI_NSS_Hard_Master_SS_Output_En)
	{
		TempReg_CR2 |= SPI_Config->NSS ;

	}
	else if (SPI_Config->NSS==SPI_NSS_Hard_Master_SS_Output_Dis)
	{
		TempReg_CR2 &= SPI_Config->NSS ;
	}
	else
	{
		TempReg_CR1 |= SPI_Config->NSS ;
	}

	//Baud_Rate_Prescaler
	TempReg_CR1 |= SPI_Config->Baud_Rate_Prescaler;

	//IQR
	if (SPI_Config->IRQ_Enable != SPI_IRQ_Enable_NONE)
	{
		TempReg_CR2 |= SPI_Config->IRQ_Enable ;
		if (SPIx == SPI1)
		{

			NVIC_IRQ35_SPI1_ENABLE();
		}
		else if (SPIx == SPI2)
		{
			NVIC_IRQ36_SPI2_ENABLE();
		}


	}

	SPIx->SPI_CR1 = TempReg_CR1;
	SPIx->SPI_CR2 = TempReg_CR2;


}
void MCAL_SPI_DeINIT(SPI_Typedef* SPIx)
{

	if (SPIx == SPI1)
	{

		NVIC_IRQ35_SPI1_DISABLE();
		RCC_SPI1_RESET();
	}
	else if (SPIx == SPI2)
	{
		NVIC_IRQ36_SPI2_DISABLE();
		RCC_SPI2_RESET();
	}

}
void MCAL_SPI_GPIO_SET_PINS(SPI_Typedef* SPIx)
{
	GPIO_PIN_Configer_t Config_pin;
	if(SPIx == SPI1)
	{
		//PA4 : SPI1_NSS
		//PA5 : SPI1_SCK
		//PA6 : SPI1_MISO
		//PA7 : SPI1_MOSI
		if(Global_SPI_Config[SPI1_INDEX]->Device_Mode == SPI_Device_Mode_Master )
		{
			//PA4 : SPI1_NSS
			switch( Global_SPI_Config[SPI1_INDEX]->NSS )
			{
			case  SPI_NSS_Hard_Master_SS_Output_Dis :
				Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
				Config_pin.GPIO_PinNumber = GPIO_PIN_4 ;
				MCAL_GPIO_Init(GPIOA,&Config_pin);
				break ;

			case  SPI_NSS_Hard_Master_SS_Output_En :
				Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
				Config_pin.GPIO_PinNumber = GPIO_PIN_4 ;
				Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
				MCAL_GPIO_Init(GPIOA,&Config_pin);
				break ;

			}
			//PA5 : SPI1_SCK
			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_5 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOA,&Config_pin);

			//PA6  : SPIx_MOSI

			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_6 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOA,&Config_pin);


			//PA7  : SPIx_MISO

			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_7 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOA,&Config_pin);




		}//SLAVE
		else if(Global_SPI_Config[SPI1_INDEX]->Device_Mode == SPI_Device_Mode_Slave)
		{
			//PA4 : SPI1_NSS
			if( Global_SPI_Config[SPI1_INDEX]->NSS == SPI_NSS_Hard_Slave )
			{
				Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
				Config_pin.GPIO_PinNumber = GPIO_PIN_4 ;
				MCAL_GPIO_Init(GPIOA,&Config_pin);

			}
			//PA5 : SPI1_SCK //input_floating
			Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_5 ;
			MCAL_GPIO_Init(GPIOA,&Config_pin);

			//PA6 : SPI1_MISO
			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_6 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOA,&Config_pin);

			//PA7 : SPI1_MOSI
			Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_7 ;
			MCAL_GPIO_Init(GPIOA,&Config_pin);

		}
	}
	else if(SPIx == SPI2)
	{
		//PB12 : SPI1_NSS
		//PB13 : SPI1_SCK
		//PB14 : SPI1_MISO
		//PB15 : SPI1_MOSI
		if(Global_SPI_Config[SPI1_INDEX]->Device_Mode == SPI_Device_Mode_Master )
		{
			//PB12 : SPI1_NSS
			switch( Global_SPI_Config[SPI1_INDEX]->NSS )
			{
			case  SPI_NSS_Hard_Master_SS_Output_Dis :
				Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
				Config_pin.GPIO_PinNumber = GPIO_PIN_12 ;
				MCAL_GPIO_Init(GPIOB,&Config_pin);
				break ;

			case  SPI_NSS_Hard_Master_SS_Output_En :
				Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
				Config_pin.GPIO_PinNumber = GPIO_PIN_12 ;
				Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
				MCAL_GPIO_Init(GPIOB,&Config_pin);
				break ;

			}
			//PB13 : SPI1_SCK
			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_13 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOB,&Config_pin);

			//PB14  : SPIx_MOSI

			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_14 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOB,&Config_pin);


			//PB15  : SPIx_MISO

			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_15 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOB,&Config_pin);




		}//SLAVE
		else if(Global_SPI_Config[SPI1_INDEX]->Device_Mode == SPI_Device_Mode_Slave)
		{
			//PB12 : SPI1_NSS
			if( Global_SPI_Config[SPI1_INDEX]->NSS == SPI_NSS_Hard_Slave )
			{
				Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
				Config_pin.GPIO_PinNumber = GPIO_PIN_12 ;
				MCAL_GPIO_Init(GPIOB,&Config_pin);

			}
			//PB13 : SPI1_SCK //input_floating
			Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_13 ;
			MCAL_GPIO_Init(GPIOB,&Config_pin);

			//PB14 : SPI1_MISO
			Config_pin.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_14 ;
			Config_pin.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M ;
			MCAL_GPIO_Init(GPIOB,&Config_pin);

			//PB15 : SPI1_MOSI
			Config_pin.GPIO_MODE = GPIO_MODE_INPUT_FLO ;
			Config_pin.GPIO_PinNumber = GPIO_PIN_15 ;
			MCAL_GPIO_Init(GPIOB,&Config_pin);

		}
	}

}


void MCAL_SPI_SEND_DATA(SPI_Typedef* SPIx,uint16_t* pTxBuffer,enum PollingMechism PollingEn)
{
	if (PollingEn == PollingEnable)
		while(! (SPIx->SPI_SR & SPI_SR_TXE) );
	SPIx->SPI_DR = *pTxBuffer;
}
void MCAL_SPI_RECEIVE_DATA(SPI_Typedef* SPIx,uint16_t* pTxBuffer,enum PollingMechism PollingEn)
{
	if (PollingEn == PollingEnable)
		while(! (SPIx->SPI_SR & SPI_SR_RXNE));
	*pTxBuffer = SPIx->SPI_DR;

}
void MCAL_SPI_TX_RX(SPI_Typedef* SPIx,uint16_t* pTxBuffer,enum PollingMechism PollingEn)
{
	if (PollingEn == PollingEnable)
		while(! (SPIx->SPI_SR & SPI_SR_TXE) );
	SPIx->SPI_DR = *pTxBuffer;

	if (PollingEn == PollingEnable)
		while(! (SPIx->SPI_SR & SPI_SR_RXNE));
	*pTxBuffer = SPIx->SPI_DR;

}

/*=================================================================
 *                     IRQs
 *=================================================================
 */

/*void SPI1_IRQHandler (void)
{
	struct S_IRQ_SRC irq_src;
	irq_src.TXE   = ( (SPI1->SPI_SR & (SPI_SR_TXE)>>1) );
	irq_src.RXNE  = ( (SPI1->SPI_SR & (SPI_SR_RXNE)>>0) );
	irq_src.ERROR = ( (SPI1->SPI_SR & (1<<4)>>4) );

	Global_SPI_Config[SPI1_INDEX]->P_IRQ_CallBack(irq_src);
}

void SPI2_IRQHandler (void)
{
	struct S_IRQ_SRC irq_src;
	irq_src.TXE   = ( (SPI2->SPI_SR & (SPI_SR_TXE)>>1) );
	irq_src.RXNE  = ( (SPI2->SPI_SR & (SPI_SR_RXNE)>>0) );
	irq_src.ERROR = ( (SPI2->SPI_SR & (1<<4)>>4) );

	Global_SPI_Config[SPI2_INDEX]->P_IRQ_CallBack(irq_src);
}*/
