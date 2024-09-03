################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/libs/nanoModbus/nanomodbus.c 

OBJS += \
./Core/libs/nanoModbus/nanomodbus.o 

C_DEPS += \
./Core/libs/nanoModbus/nanomodbus.d 


# Each subdirectory must supply rules for building sources it contributes
Core/libs/nanoModbus/%.o Core/libs/nanoModbus/%.su Core/libs/nanoModbus/%.cyclo: ../Core/libs/nanoModbus/%.c Core/libs/nanoModbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xB -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/libs/nanoModbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus/mb_slave" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus/mb_master" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-libs-2f-nanoModbus

clean-Core-2f-libs-2f-nanoModbus:
	-$(RM) ./Core/libs/nanoModbus/nanomodbus.cyclo ./Core/libs/nanoModbus/nanomodbus.d ./Core/libs/nanoModbus/nanomodbus.o ./Core/libs/nanoModbus/nanomodbus.su

.PHONY: clean-Core-2f-libs-2f-nanoModbus

