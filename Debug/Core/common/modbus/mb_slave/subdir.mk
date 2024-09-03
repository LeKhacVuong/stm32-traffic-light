################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/common/modbus/mb_slave/sm_mb_slave_impl.c 

OBJS += \
./Core/common/modbus/mb_slave/sm_mb_slave_impl.o 

C_DEPS += \
./Core/common/modbus/mb_slave/sm_mb_slave_impl.d 


# Each subdirectory must supply rules for building sources it contributes
Core/common/modbus/mb_slave/%.o Core/common/modbus/mb_slave/%.su Core/common/modbus/mb_slave/%.cyclo: ../Core/common/modbus/mb_slave/%.c Core/common/modbus/mb_slave/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xB -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/libs/nanoModbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus/mb_slave" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/common/modbus/mb_master" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/light_slave/Core/utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-common-2f-modbus-2f-mb_slave

clean-Core-2f-common-2f-modbus-2f-mb_slave:
	-$(RM) ./Core/common/modbus/mb_slave/sm_mb_slave_impl.cyclo ./Core/common/modbus/mb_slave/sm_mb_slave_impl.d ./Core/common/modbus/mb_slave/sm_mb_slave_impl.o ./Core/common/modbus/mb_slave/sm_mb_slave_impl.su

.PHONY: clean-Core-2f-common-2f-modbus-2f-mb_slave

