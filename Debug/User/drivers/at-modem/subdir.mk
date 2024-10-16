################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/drivers/at-modem/sm_modem.c 

OBJS += \
./User/drivers/at-modem/sm_modem.o 

C_DEPS += \
./User/drivers/at-modem/sm_modem.d 


# Each subdirectory must supply rules for building sources it contributes
User/drivers/at-modem/%.o User/drivers/at-modem/%.su User/drivers/at-modem/%.cyclo: ../User/drivers/at-modem/%.c User/drivers/at-modem/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Personal_projects/stm32-traffic-light/User" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lora" -I"D:/Personal_projects/stm32-traffic-light/User/utils" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_lora" -I"D:/Personal_projects/stm32-traffic-light/User/libs/nanoModbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_slave" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-drivers-2f-at-2d-modem

clean-User-2f-drivers-2f-at-2d-modem:
	-$(RM) ./User/drivers/at-modem/sm_modem.cyclo ./User/drivers/at-modem/sm_modem.d ./User/drivers/at-modem/sm_modem.o ./User/drivers/at-modem/sm_modem.su

.PHONY: clean-User-2f-drivers-2f-at-2d-modem

