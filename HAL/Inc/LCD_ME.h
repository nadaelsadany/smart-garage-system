/*
 * LCD.h
 *
 * Created: 6/30/2021 8:57:06 PM
 *  Author: Mostafa El-Flall
 */
#define LINE_DIGITS 16
#include "stdint.h"
#include "Stm32_F103C6_Gpio_driver.h"
#ifndef LCD_H_
#define LCD_H_
typedef enum{
	CURSOR_LEFT  = 0x00,
	CURSOR_RIGHT = 0x04,
	DISP_LEFT	 = 0x08,
	DISP_RIGHT	 = 0x0c
	}e_ShiftDir_t;
typedef enum{
	NOSHIFT_DECREMENT=0,
	SHIFT_DECREMENT=1,
	NOSHIFT_INCREMENT=2,
	SHIFT_INCREMENT=3
	}e_EntryMode_t;
typedef enum{
	OFFDISP_OFFCURSOR	=	0x00,
	ONDISP_OFFCURSOR	=	0x04,
	ONDISP_ONCURSOR		=	0x06,
	ONDISP_BLINKCURSOR	=	0x07
	}e_DispCtrl_t;
typedef enum{
	FOURBIT_1LINE_7DOT	=	0x00,
	FOURBIT_1LINE_10DOT	=	0x04,
	FOURBIT_2LINE_7DOT	=	0x08,
	FOURBIT_2LINE_10DOT	=	0x0C,
	EIGHTBIT_1LINE_7DOT	=	0x10,
	EIGHTBIT_1LINE_10DOT=	0x14,
	EIGHTBIT_2LINE_7DOT	=	0x18,
	EIGHTBIT_2LINE_10DOT=	0x1C
	}e_OperMode_t;
	// in this version i separated the struct of the control pins from the struct of data pins in case if the the control pins were in another port
typedef struct {
	GPIO_Typedef 	*GPIOX;
	uint16_t 		DataPinStart;				//the pin number of the lowest data pin
	}s_LCDDataPins_t;
typedef struct {
	GPIO_Typedef 	*GPIOX;
	uint16_t 		rspin;						//register select pin
	uint16_t 		enpin;						//enable pin
	}s_LCDCtrlPins_t;
typedef struct{
	s_LCDCtrlPins_t s_LCDCtrlPins;
	s_LCDDataPins_t	s_LCDDataPins ;
	e_EntryMode_t	e_EntryMode;
	e_DispCtrl_t	e_DispCtrl;
	e_OperMode_t	e_OperMode;
	uint8_t 		Ch_Counter;
	}s_LCD_Config_t;
typedef enum {
	COMMAND,
	DATA
}e_LCDCharType_t;

// used to initialize the LCD , the config must be done first
void LCD_init(s_LCD_Config_t  *  LCD );
//displaying character or sending command
void LCD_sendchar(s_LCD_Config_t  *  LCD ,unsigned char data ,e_LCDCharType_t e_LCDCharType);
//the whole string is displayed
void LCD_sendstring(s_LCD_Config_t  * LCD , const char *  data);
//it's not complete yet , but it's suppose to check the LCD busy flag but my code didn't work in 4bit mode
void LCDIsbusy(s_LCD_Config_t  * LCD);
// clear
void LCD_clearscreen(s_LCD_Config_t  * LCD);
// return to 0, 0
void LCD_returnhome(s_LCD_Config_t  * LCD);
// well, you got that
void LCD_gotoxy(s_LCD_Config_t  * LCD , unsigned char x , unsigned char y);
//used to build your custom character
void LCD_buildcustom(s_LCD_Config_t  * LCD ,unsigned char location ,const unsigned char * pattern );
// display it now
void LCD_dispcustom(s_LCD_Config_t  * LCD ,unsigned char location);
//shift display and/or cursor
void LCD_shift(s_LCD_Config_t  * LCD ,e_ShiftDir_t e_ShiftDir);
#endif /* LCD_H_ */
