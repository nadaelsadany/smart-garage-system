/*
 * LCD.c
 *
 * Created: 6/30/2021 8:56:53 PM
 *  Author: Mostafa El-Flall
 */
#define BITSET(reg,bit) (reg |=(1<< bit))
#define BITCLEAR(reg,bit) (reg &= ~(1<<bit))
#include "LCD_ME.h"
#define ClEAR_SCREEN			0x01
#define RETURN_HOME				0x02
#define ENTRY_MODE_SET			0x04
#define	DISPLAY_CONTROL			0x08
#define CURSOR_DISPLAY_SHIFT	0x10
#define FUNCTION_SET			0x20
#define SET_CG_ADDRESS			0x40
#define SET_DD_ADDRESS			0x80


static void LCD_delay(int ms){
	volatile uint32_t counter =0;
	for(int i =0;i<ms;i++){
		for(;counter<1500;counter++);
	}
}
//void MCAL_GPIO_WritePin(GPIO_TypeDef * GPIOx,uint16_t PinNumber,uint8_t Vlaue);

void LCD_kick(s_LCD_Config_t  * LCD){
	//BITSET(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.enpin);									//enable pulse
	MCAL_GPIO_WritePin(LCD->s_LCDCtrlPins.GPIOX,LCD->s_LCDCtrlPins.enpin,GPIO_PIN_TRUE);
	LCD_delay(1);
	MCAL_GPIO_WritePin(LCD->s_LCDCtrlPins.GPIOX,LCD->s_LCDCtrlPins.enpin,GPIO_PIN_FALSE);
}

void LCD_init(s_LCD_Config_t  *  LCD ){
	//*(LCD->s_LCDCtrlPins.p_DataDireReg) |= (1<<LCD->s_LCDCtrlPins.enpin) | (1<<LCD->s_LCDCtrlPins.rspin) | (1<<LCD->s_LCDCtrlPins.rwpin);
	GPIO_PIN_Configer_t pin = {LCD->s_LCDCtrlPins.enpin ,GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10M};
	MCAL_GPIO_Init(LCD->s_LCDCtrlPins.GPIOX,&pin);
	pin.GPIO_PinNumber = LCD->s_LCDCtrlPins.rspin;
	MCAL_GPIO_Init(LCD->s_LCDCtrlPins.GPIOX,&pin);
	LCD->Ch_Counter = 0;
	if(LCD->e_OperMode < EIGHTBIT_1LINE_7DOT)	// 4 bits
	{
		//*(LCD->s_LCDDataPins.p_DataDireReg) |= (0x0F << LCD->s_LCDDataPins.DataPinStart);
		for(uint8_t i =0;i<4;i++){
			pin.GPIO_PinNumber = LCD->s_LCDDataPins.DataPinStart << i;
			MCAL_GPIO_Init(LCD->s_LCDDataPins.GPIOX,&pin);
		}
	}
	else
	{
		for(uint8_t i =0;i<8;i++){
			pin.GPIO_PinNumber = LCD->s_LCDDataPins.DataPinStart << i;
			MCAL_GPIO_Init(LCD->s_LCDDataPins.GPIOX,&pin);
		}
	}
	LCD_sendchar(LCD ,RETURN_HOME , COMMAND );
	LCD_sendchar(LCD ,FUNCTION_SET + LCD->e_OperMode , COMMAND );
	LCD_sendchar(LCD ,DISPLAY_CONTROL + LCD->e_DispCtrl , COMMAND );
	LCD_sendchar(LCD ,ENTRY_MODE_SET + LCD->e_EntryMode , COMMAND );
	LCD_clearscreen(LCD);
	LCD_returnhome(LCD);
	LCD_sendchar(LCD ,SET_DD_ADDRESS , COMMAND );
}
void LCD_sendchar(s_LCD_Config_t  *  LCD ,unsigned char data ,e_LCDCharType_t e_LCDCharType)
{
	LCDIsbusy(LCD);
	if(LCD->e_OperMode < EIGHTBIT_1LINE_7DOT)	// 4 bits
	{
		//void MCAL_GPIO_WriteGroup(GPIO_TypeDef * GPIOx,uint16_t Vlaue,uint16_t mask);
		//*(LCD->s_LCDDataPins.p_PortReg) &= ~(0x0F << LCD->s_LCDDataPins.DataPinStart);						// clear the data pins
		//*(LCD->s_LCDDataPins.p_PortReg) = (((data & 0xF0) >> 4) << LCD->s_LCDDataPins.DataPinStart);		//the high nibble
		MCAL_GPIO_WriteGroup(LCD->s_LCDDataPins.GPIOX
				,((data & 0xF0) >> 4)*LCD->s_LCDDataPins.DataPinStart
				,0x0F*LCD->s_LCDDataPins.DataPinStart);
		if(e_LCDCharType == COMMAND)
			//BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			MCAL_GPIO_WritePin(LCD->s_LCDCtrlPins.GPIOX,LCD->s_LCDCtrlPins.rspin,GPIO_PIN_FALSE);
		else{
			//BITSET(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			MCAL_GPIO_WritePin(LCD->s_LCDCtrlPins.GPIOX,LCD->s_LCDCtrlPins.rspin,GPIO_PIN_TRUE);
		}

		LCD_kick(LCD);

		//*(LCD->s_LCDDataPins.p_PortReg) &= ~(0x0F << LCD->s_LCDDataPins.DataPinStart);
		//*(LCD->s_LCDDataPins.p_PortReg) |= ((data & 0x0F) << LCD->s_LCDDataPins.DataPinStart);
		MCAL_GPIO_WriteGroup(LCD->s_LCDDataPins.GPIOX
				,(data & 0x0F)*LCD->s_LCDDataPins.DataPinStart
				,0x0F*LCD->s_LCDDataPins.DataPinStart);

		LCD_kick(LCD);
		LCD_delay(1);
	}
	else
	{						//8bit mode
		//*(LCD->s_LCDDataPins.p_PortReg) &= ~(0xFF << LCD->s_LCDDataPins.DataPinStart);
		//*(LCD->s_LCDDataPins.p_PortReg) |= (data << LCD->s_LCDDataPins.DataPinStart);
		MCAL_GPIO_WriteGroup(LCD->s_LCDDataPins.GPIOX
				,(data & 0xFF)*LCD->s_LCDDataPins.DataPinStart
				,0xFF*LCD->s_LCDDataPins.DataPinStart);
		if(e_LCDCharType == COMMAND)
			//BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			MCAL_GPIO_WritePin(LCD->s_LCDCtrlPins.GPIOX,LCD->s_LCDCtrlPins.rspin,GPIO_PIN_FALSE);
		else{
			//BITSET(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			MCAL_GPIO_WritePin(LCD->s_LCDCtrlPins.GPIOX,LCD->s_LCDCtrlPins.rspin,GPIO_PIN_TRUE);

		}
		LCD_kick(LCD);
	}
	if(e_LCDCharType == DATA){
		LCD->Ch_Counter ++;
			if(LCD->Ch_Counter==LINE_DIGITS)
				LCD_gotoxy(LCD,0,1);
			else if (LCD->Ch_Counter==2*LINE_DIGITS)
				LCD_gotoxy(LCD,0,2);
			else if (LCD->Ch_Counter==3*LINE_DIGITS)
				LCD_gotoxy(LCD,0,3);
		}
}
void LCD_sendstring(s_LCD_Config_t  * LCD , const char *  data)
{
	for(;*data != '\0';data++){
		LCD_sendchar(LCD,*data,DATA);
	}
}
void LCDIsbusy(s_LCD_Config_t  * LCD)
{
	//currently I'm working on reading the actual busy flag
	//for the end of the dev process xD let it simply a delay.
	LCD_delay(1);
}

void LCD_clearscreen(s_LCD_Config_t  * LCD)
{
	LCD_sendchar(LCD , ClEAR_SCREEN , COMMAND );
	LCD->Ch_Counter=0;
}
void LCD_returnhome(s_LCD_Config_t  * LCD)
{
	LCD_sendchar(LCD , RETURN_HOME , COMMAND );
	LCD->Ch_Counter=0;
}
void LCD_gotoxy(s_LCD_Config_t  * LCD , unsigned char x , unsigned char y)
{
	uint32_t address;
	switch(y)
	{
	case 0:
		address = x;
		break;
	case 1:
		address = 0x40+x;
		break;
	case 2:
		address = 0x14+x;
		break;
	case 3:
		address = 0x54+x;
		break;
	}
	LCD->Ch_Counter=y*LINE_DIGITS+x;
	LCD_sendchar(LCD,SET_DD_ADDRESS+address,COMMAND);
}
void LCD_buildcustom(s_LCD_Config_t  * LCD ,unsigned char location ,const unsigned char * pattern )
{
	unsigned char i=0;
	if(location<8){
		LCD_sendchar(LCD,0x40+(location*8),COMMAND);
		for(i=0;i<8;i++)
			LCD_sendchar(LCD,pattern[ i ],DATA);
	}
}
void LCD_dispcustom(s_LCD_Config_t  * LCD ,unsigned char location){
	LCD_sendchar(LCD,location,DATA);
}
void LCD_shift(s_LCD_Config_t  * LCD ,e_ShiftDir_t e_ShiftDir){
	LCD_sendchar(LCD ,CURSOR_DISPLAY_SHIFT + e_ShiftDir , COMMAND );
}

void Wait()
{
	int i=0;
	for(i=0;i<25000;i++);
}

