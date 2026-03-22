################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/menu/eventManaget.c \
../Core/Src/menu/menu.c 

OBJS += \
./Core/Src/menu/eventManaget.o \
./Core/Src/menu/menu.o 

C_DEPS += \
./Core/Src/menu/eventManaget.d \
./Core/Src/menu/menu.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/menu/%.o Core/Src/menu/%.su: ../Core/Src/menu/%.c Core/Src/menu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-menu

clean-Core-2f-Src-2f-menu:
	-$(RM) ./Core/Src/menu/eventManaget.d ./Core/Src/menu/eventManaget.o ./Core/Src/menu/eventManaget.su ./Core/Src/menu/menu.d ./Core/Src/menu/menu.o ./Core/Src/menu/menu.su

.PHONY: clean-Core-2f-Src-2f-menu

