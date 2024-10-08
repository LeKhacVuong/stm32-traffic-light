################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/services/sv_lora/sv_lora.c 

OBJS += \
./User/services/sv_lora/sv_lora.o 

C_DEPS += \
./User/services/sv_lora/sv_lora.d 


# Each subdirectory must supply rules for building sources it contributes
User/services/sv_lora/%.o User/services/sv_lora/%.su User/services/sv_lora/%.cyclo: ../User/services/sv_lora/%.c User/services/sv_lora/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/drivers/lora" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/services/sv_lora" -I"/home/lekhacvuong/STM32CubeIDE/workspace_1.14.0/traffic_light/User/utils" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-services-2f-sv_lora

clean-User-2f-services-2f-sv_lora:
	-$(RM) ./User/services/sv_lora/sv_lora.cyclo ./User/services/sv_lora/sv_lora.d ./User/services/sv_lora/sv_lora.o ./User/services/sv_lora/sv_lora.su

.PHONY: clean-User-2f-services-2f-sv_lora

