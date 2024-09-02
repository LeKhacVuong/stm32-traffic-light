cmake_minimum_required(VERSION 3.22)

project(mb_slave)

set(CMAKE_C_STANDARD 99)

add_library(mb_slave STATIC
        sm_mb_slave_if.h
        sm_mb_slave_impl.c
        sm_mb_slave_impl.h
        ../../../libs/nanoModbus/nanomodbus.h
        ../../../libs/nanoModbus/nanomodbus.c)

#add_executable(modbus_slave example.c)
#target_link_libraries(modbus_slave host sdk_utils)


