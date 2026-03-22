################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/display/display.c \
../Core/Src/display/fonts.c \
../Core/Src/display/ssd1306.c 

OBJS += \
./Core/Src/display/display.o \
./Core/Src/display/fonts.o \
./Core/Src/display/ssd1306.o 

C_DEPS += \
./Core/Src/display/display.d \
./Core/Src/display/fonts.d \
./Core/Src/display/ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/display/%.o Core/Src/display/%.su: ../Core/Src/display/%.c Core/Src/display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-display

clean-Core-2f-Src-2f-display:
	-$(RM) ./Core/Src/display/display.d ./Core/Src/display/display.o ./Core/Src/display/display.su ./Core/Src/display/fonts.d ./Core/Src/display/fonts.o ./Core/Src/display/fonts.su ./Core/Src/display/ssd1306.d ./Core/Src/display/ssd1306.o ./Core/Src/display/ssd1306.su

.PHONY: clean-Core-2f-Src-2f-display

