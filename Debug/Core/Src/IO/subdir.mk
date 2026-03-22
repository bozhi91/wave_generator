################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/IO/gpio.c \
../Core/Src/IO/i2c.c \
../Core/Src/IO/keypad.c \
../Core/Src/IO/pwm.c \
../Core/Src/IO/signalGenerator.c 

OBJS += \
./Core/Src/IO/gpio.o \
./Core/Src/IO/i2c.o \
./Core/Src/IO/keypad.o \
./Core/Src/IO/pwm.o \
./Core/Src/IO/signalGenerator.o 

C_DEPS += \
./Core/Src/IO/gpio.d \
./Core/Src/IO/i2c.d \
./Core/Src/IO/keypad.d \
./Core/Src/IO/pwm.d \
./Core/Src/IO/signalGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/IO/%.o Core/Src/IO/%.su: ../Core/Src/IO/%.c Core/Src/IO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-IO

clean-Core-2f-Src-2f-IO:
	-$(RM) ./Core/Src/IO/gpio.d ./Core/Src/IO/gpio.o ./Core/Src/IO/gpio.su ./Core/Src/IO/i2c.d ./Core/Src/IO/i2c.o ./Core/Src/IO/i2c.su ./Core/Src/IO/keypad.d ./Core/Src/IO/keypad.o ./Core/Src/IO/keypad.su ./Core/Src/IO/pwm.d ./Core/Src/IO/pwm.o ./Core/Src/IO/pwm.su ./Core/Src/IO/signalGenerator.d ./Core/Src/IO/signalGenerator.o ./Core/Src/IO/signalGenerator.su

.PHONY: clean-Core-2f-Src-2f-IO

