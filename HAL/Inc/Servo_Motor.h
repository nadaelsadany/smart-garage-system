/*
 * Servo_Motor.h
 *
 *  Created on: Nov 12, 2021
 *      Author:  Mostafa Mahmoud Elshiekh
 */



#ifndef SERVO_MOTOR_SERVO_MOTOR_H_
#define SERVO_MOTOR_SERVO_MOTOR_H_

#include "Stm32_F103C6_Gpio_driver.h"
#include "Stm32_F103C6_Time_driver.h"

#define UP   1
#define Down 2


void Servo1_Entry_Gate_Init(void);
void Servo1_Entry_Gate_Up90(void);
void Servo1_Entry_Gate_Down90(void);

void Servo2_Exit_Gate_Init(void);
void Servo2_Exit_Gate_Up90(void);
void Servo2_Exit_Gate_Down90(void);




#endif /* SERVO_MOTOR_SERVO_MOTOR_H_ */
