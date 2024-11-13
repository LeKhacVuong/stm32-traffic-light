################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/main.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/Personal_projects/stm32-traffic-light/Core/Inc" -I"D:/Personal_projects/stm32-traffic-light/USB_DEVICE/Target" -I"D:/Personal_projects/stm32-traffic-light/USB_DEVICE/App" -I"D:/Personal_projects/stm32-traffic-light/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/Personal_projects/stm32-traffic-light/Drivers/CMSIS/Include" -I"D:/Personal_projects/stm32-traffic-light/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/Personal_projects/stm32-traffic-light/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/Personal_projects/stm32-traffic-light/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"D:/Personal_projects/stm32-traffic-light/User/common/data" -I"D:/Personal_projects/stm32-traffic-light/User/common/http_server" -I"D:/Personal_projects/stm32-traffic-light/User/common/mqtt_client" -I"D:/Personal_projects/stm32-traffic-light/User/common/tcp_server" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/at-modem" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/gps" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lora" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/wifi" -I"D:/Personal_projects/stm32-traffic-light/User/drivers/lte/inc" -I"D:/Personal_projects/stm32-traffic-light/User/hals/include" -I"D:/Personal_projects/stm32-traffic-light/User/libs/json-maker" -I"D:/Personal_projects/stm32-traffic-light/User/libs/nanoModbus" -I"D:/Personal_projects/stm32-traffic-light/User/libs/tiny-json" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_datetime" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_gps" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_iot" -I"D:/Personal_projects/stm32-traffic-light/User/services/sv_lora" -I"D:/Personal_projects/stm32-traffic-light/User/utils/inc" -I"D:/Personal_projects/stm32-traffic-light/User/utils" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_slave" -I"D:/Personal_projects/stm32-traffic-light/User/services/modbus/mb_master" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src

