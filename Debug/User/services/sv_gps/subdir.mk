################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/services/sv_gps/sm_gps_l76x.c 

OBJS += \
./User/services/sv_gps/sm_gps_l76x.o 

C_DEPS += \
./User/services/sv_gps/sm_gps_l76x.d 


# Each subdirectory must supply rules for building sources it contributes
User/services/sv_gps/%.o User/services/sv_gps/%.su User/services/sv_gps/%.cyclo: ../User/services/sv_gps/%.c User/services/sv_gps/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Personal_projects/stm32-traffic-light/User" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lora" -I"D:/Personal_projects/stm32-traffic-light/User/utils" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_lora" -I"D:/Personal_projects/stm32-traffic-light/User/libs/nanoModbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_slave" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-services-2f-sv_gps

clean-User-2f-services-2f-sv_gps:
	-$(RM) ./User/services/sv_gps/sm_gps_l76x.cyclo ./User/services/sv_gps/sm_gps_l76x.d ./User/services/sv_gps/sm_gps_l76x.o ./User/services/sv_gps/sm_gps_l76x.su

.PHONY: clean-User-2f-services-2f-sv_gps

