################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/utils/sm_logger.c 

OBJS += \
./Core/utils/sm_logger.o 

C_DEPS += \
./Core/utils/sm_logger.d 


# Each subdirectory must supply rules for building sources it contributes
Core/utils/%.o Core/utils/%.su Core/utils/%.cyclo: ../Core/utils/%.c Core/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xB -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/libs/nanoModbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus/mb_slave" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus/mb_master" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-utils

clean-Core-2f-utils:
	-$(RM) ./Core/utils/sm_logger.cyclo ./Core/utils/sm_logger.d ./Core/utils/sm_logger.o ./Core/utils/sm_logger.su

.PHONY: clean-Core-2f-utils

