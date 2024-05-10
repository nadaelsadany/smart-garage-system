/*
 * Keypad.h
 *
 *  Created on: Oct 28, 2021
 *      Author: Mostafa Mahmoud Elshiekh
 */


#include "Stm32_F103C6_Gpio_driver.h"

#ifndef KEYPAD_KEYPAD_H_
#define KEYPAD_KEYPAD_H_

void keypad_Init();
char keypad_Read(void);

#endif /* KEYPAD_KEYPAD_H_ */
