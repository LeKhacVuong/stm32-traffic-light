################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/drivers/lora/LoRa.c 

OBJS += \
./User/drivers/lora/LoRa.o 

C_DEPS += \
./User/drivers/lora/LoRa.d 


# Each subdirectory must supply rules for building sources it contributes
User/drivers/lora/%.o User/drivers/lora/%.su User/drivers/lora/%.cyclo: ../User/drivers/lora/%.c User/drivers/lora/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/drivers/lora" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/utils" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/services/sv_lora" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/libs/nanoModbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/services/modbus" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/services/modbus/mb_slave" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-drivers-2f-lora

clean-User-2f-drivers-2f-lora:
	-$(RM) ./User/drivers/lora/LoRa.cyclo ./User/drivers/lora/LoRa.d ./User/drivers/lora/LoRa.o ./User/drivers/lora/LoRa.su

.PHONY: clean-User-2f-drivers-2f-lora

