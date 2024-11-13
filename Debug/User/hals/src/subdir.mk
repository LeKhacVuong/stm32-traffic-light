################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/hals/src/stm_hal_adc.c \
../User/hals/src/stm_hal_delay.c \
../User/hals/src/stm_hal_uart.c 

OBJS += \
./User/hals/src/stm_hal_adc.o \
./User/hals/src/stm_hal_delay.o \
./User/hals/src/stm_hal_uart.o 

C_DEPS += \
./User/hals/src/stm_hal_adc.d \
./User/hals/src/stm_hal_delay.d \
./User/hals/src/stm_hal_uart.d 


# Each subdirectory must supply rules for building sources it contributes
User/hals/src/%.o User/hals/src/%.su User/hals/src/%.cyclo: ../User/hals/src/%.c User/hals/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/Personal_projects/stm32-traffic-light/Core/Inc" -I"D:/Personal_projects/stm32-traffic-light/USB_DEVICE/Target" -I"D:/Personal_projects/stm32-traffic-light/USB_DEVICE/App" -I"D:/Personal_projects/stm32-traffic-light/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/Personal_projects/stm32-traffic-light/Drivers/CMSIS/Include" -I"D:/Personal_projects/stm32-traffic-light/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/Personal_projects/stm32-traffic-light/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/Personal_projects/stm32-traffic-light/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"D:/Personal_projects/stm32-traffic-light/User/common/data" -I"D:/Personal_projects/stm32-traffic-light/User/common/http_server" -I"D:/Personal_projects/stm32-traffic-light/User/common/mqtt_client" -I"D:/Personal_projects/stm32-traffic-light/User/common/tcp_server" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/at-modem" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/gps" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lora" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/wifi" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lte/inc" -I"D:/Personal_projects/stm32-traffic-light/User/hals/include" -I"D:/Personal_projects/stm32-traffic-light/User/libs/json-maker" -I"D:/Personal_projects/stm32-traffic-light/User/libs/nanoModbus" -I"D:/Personal_projects/stm32-traffic-light/User/libs/tiny-json" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_datetime" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_gps" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_iot" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_lora" -I"D:/Personal_projects/stm32-traffic-light/User/utils/inc" -I"D:/Personal_projects/stm32-traffic-light/User/utils" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_slave" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_master" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-hals-2f-src

clean-User-2f-hals-2f-src:
	-$(RM) ./User/hals/src/stm_hal_adc.cyclo ./User/hals/src/stm_hal_adc.d ./User/hals/src/stm_hal_adc.o ./User/hals/src/stm_hal_adc.su ./User/hals/src/stm_hal_delay.cyclo ./User/hals/src/stm_hal_delay.d ./User/hals/src/stm_hal_delay.o ./User/hals/src/stm_hal_delay.su ./User/hals/src/stm_hal_uart.cyclo ./User/hals/src/stm_hal_uart.d ./User/hals/src/stm_hal_uart.o ./User/hals/src/stm_hal_uart.su

.PHONY: clean-User-2f-hals-2f-src

