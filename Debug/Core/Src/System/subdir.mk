################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/System/dac.c \
../Core/Src/System/dma.c \
../Core/Src/System/stm32f3xx_hal_msp.c \
../Core/Src/System/stm32f3xx_it.c \
../Core/Src/System/syscalls.c \
../Core/Src/System/sysmem.c \
../Core/Src/System/system_stm32f3xx.c \
../Core/Src/System/tim.c 

OBJS += \
./Core/Src/System/dac.o \
./Core/Src/System/dma.o \
./Core/Src/System/stm32f3xx_hal_msp.o \
./Core/Src/System/stm32f3xx_it.o \
./Core/Src/System/syscalls.o \
./Core/Src/System/sysmem.o \
./Core/Src/System/system_stm32f3xx.o \
./Core/Src/System/tim.o 

C_DEPS += \
./Core/Src/System/dac.d \
./Core/Src/System/dma.d \
./Core/Src/System/stm32f3xx_hal_msp.d \
./Core/Src/System/stm32f3xx_it.d \
./Core/Src/System/syscalls.d \
./Core/Src/System/sysmem.d \
./Core/Src/System/system_stm32f3xx.d \
./Core/Src/System/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/System/%.o Core/Src/System/%.su: ../Core/Src/System/%.c Core/Src/System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-System

clean-Core-2f-Src-2f-System:
	-$(RM) ./Core/Src/System/dac.d ./Core/Src/System/dac.o ./Core/Src/System/dac.su ./Core/Src/System/dma.d ./Core/Src/System/dma.o ./Core/Src/System/dma.su ./Core/Src/System/stm32f3xx_hal_msp.d ./Core/Src/System/stm32f3xx_hal_msp.o ./Core/Src/System/stm32f3xx_hal_msp.su ./Core/Src/System/stm32f3xx_it.d ./Core/Src/System/stm32f3xx_it.o ./Core/Src/System/stm32f3xx_it.su ./Core/Src/System/syscalls.d ./Core/Src/System/syscalls.o ./Core/Src/System/syscalls.su ./Core/Src/System/sysmem.d ./Core/Src/System/sysmem.o ./Core/Src/System/sysmem.su ./Core/Src/System/system_stm32f3xx.d ./Core/Src/System/system_stm32f3xx.o ./Core/Src/System/system_stm32f3xx.su ./Core/Src/System/tim.d ./Core/Src/System/tim.o ./Core/Src/System/tim.su

.PHONY: clean-Core-2f-Src-2f-System

