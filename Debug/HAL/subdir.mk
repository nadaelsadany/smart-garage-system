################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/EEP_ROM_Slave.c \
../HAL/Keypad.c \
../HAL/Servo_Motor.c \
../HAL/lcd_me.c 

OBJS += \
./HAL/EEP_ROM_Slave.o \
./HAL/Keypad.o \
./HAL/Servo_Motor.o \
./HAL/lcd_me.o 

C_DEPS += \
./HAL/EEP_ROM_Slave.d \
./HAL/Keypad.d \
./HAL/Servo_Motor.d \
./HAL/lcd_me.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/EEP_ROM_Slave.o: ../HAL/EEP_ROM_Slave.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/embedded/u10/Second_Term_Project/CMSIS_V5" -I../Inc -I"D:/embedded/u10/Second_Term_Project/HAL/Inc" -I"D:/embedded/u10/Second_Term_Project/Stm32_F103C6_drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/EEP_ROM_Slave.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL/Keypad.o: ../HAL/Keypad.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/embedded/u10/Second_Term_Project/CMSIS_V5" -I../Inc -I"D:/embedded/u10/Second_Term_Project/HAL/Inc" -I"D:/embedded/u10/Second_Term_Project/Stm32_F103C6_drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/Keypad.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL/Servo_Motor.o: ../HAL/Servo_Motor.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/embedded/u10/Second_Term_Project/CMSIS_V5" -I../Inc -I"D:/embedded/u10/Second_Term_Project/HAL/Inc" -I"D:/embedded/u10/Second_Term_Project/Stm32_F103C6_drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/Servo_Motor.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL/lcd_me.o: ../HAL/lcd_me.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"D:/embedded/u10/Second_Term_Project/CMSIS_V5" -I../Inc -I"D:/embedded/u10/Second_Term_Project/HAL/Inc" -I"D:/embedded/u10/Second_Term_Project/Stm32_F103C6_drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL/lcd_me.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

