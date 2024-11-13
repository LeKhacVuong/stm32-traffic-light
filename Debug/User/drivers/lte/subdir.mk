################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/drivers/lte/sm_ec200.c \
../User/drivers/lte/sm_ec200_mqtt.c 

OBJS += \
./User/drivers/lte/sm_ec200.o \
./User/drivers/lte/sm_ec200_mqtt.o 

C_DEPS += \
./User/drivers/lte/sm_ec200.d \
./User/drivers/lte/sm_ec200_mqtt.d 


# Each subdirectory must supply rules for building sources it contributes
User/drivers/lte/%.o User/drivers/lte/%.su User/drivers/lte/%.cyclo: ../User/drivers/lte/%.c User/drivers/lte/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/Personal_projects/stm32-traffic-light/User/drivers/gps" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"D:/Personal_projects/stm32-traffic-light/User/common/mqtt_client" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Personal_projects/stm32-traffic-light/User/hals/include" -I"D:/Personal_projects/stm32-traffic-light/User" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lora" -I"D:/Personal_projects/stm32-traffic-light/User/utils" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_lora" -I"D:/Personal_projects/stm32-traffic-light/User/libs/nanoModbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_slave" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/at-modem" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-drivers-2f-lte

clean-User-2f-drivers-2f-lte:
	-$(RM) ./User/drivers/lte/sm_ec200.cyclo ./User/drivers/lte/sm_ec200.d ./User/drivers/lte/sm_ec200.o ./User/drivers/lte/sm_ec200.su ./User/drivers/lte/sm_ec200_mqtt.cyclo ./User/drivers/lte/sm_ec200_mqtt.d ./User/drivers/lte/sm_ec200_mqtt.o ./User/drivers/lte/sm_ec200_mqtt.su

.PHONY: clean-User-2f-drivers-2f-lte

