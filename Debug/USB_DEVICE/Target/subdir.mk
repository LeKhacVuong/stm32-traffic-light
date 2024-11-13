################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_DEVICE/Target/usbd_conf.c 

OBJS += \
./USB_DEVICE/Target/usbd_conf.o 

C_DEPS += \
./USB_DEVICE/Target/usbd_conf.d 


# Each subdirectory must supply rules for building sources it contributes
USB_DEVICE/Target/%.o USB_DEVICE/Target/%.su USB_DEVICE/Target/%.cyclo: ../USB_DEVICE/Target/%.c USB_DEVICE/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/Personal_projects/stm32-traffic-light/Core/Inc" -I"D:/Personal_projects/stm32-traffic-light/USB_DEVICE/Target" -I"D:/Personal_projects/stm32-traffic-light/USB_DEVICE/App" -I"D:/Personal_projects/stm32-traffic-light/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/Personal_projects/stm32-traffic-light/Drivers/CMSIS/Include" -I"D:/Personal_projects/stm32-traffic-light/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/Personal_projects/stm32-traffic-light/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/Personal_projects/stm32-traffic-light/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"D:/Personal_projects/stm32-traffic-light/User/common/data" -I"D:/Personal_projects/stm32-traffic-light/User/common/http_server" -I"D:/Personal_projects/stm32-traffic-light/User/common/mqtt_client" -I"D:/Personal_projects/stm32-traffic-light/User/common/tcp_server" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/at-modem" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/gps" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lora" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/wifi" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lte/inc" -I"D:/Personal_projects/stm32-traffic-light/User/hals/include" -I"D:/Personal_projects/stm32-traffic-light/User/libs/json-maker" -I"D:/Personal_projects/stm32-traffic-light/User/libs/nanoModbus" -I"D:/Personal_projects/stm32-traffic-light/User/libs/tiny-json" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_datetime" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_gps" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_iot" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_lora" -I"D:/Personal_projects/stm32-traffic-light/User/utils/inc" -I"D:/Personal_projects/stm32-traffic-light/User/utils" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_slave" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_master" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-USB_DEVICE-2f-Target

clean-USB_DEVICE-2f-Target:
	-$(RM) ./USB_DEVICE/Target/usbd_conf.cyclo ./USB_DEVICE/Target/usbd_conf.d ./USB_DEVICE/Target/usbd_conf.o ./USB_DEVICE/Target/usbd_conf.su

.PHONY: clean-USB_DEVICE-2f-Target

