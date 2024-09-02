cmake_minimum_required(VERSION 3.29)
project(light_slave)

set(CMAKE_CXX_STANDARD 20)

include_directories(utils
        utils/algorithms
        utils/timing
        utils/unix
        common/modbus
        common/modbus/mb_slave
        common/modbus/mb_master
        libs/nanoModbus
)

add_executable(light_slave main.c)
target_link_libraries(light_slave sdk_utils mb_slave)

add_subdirectory(utils)
add_subdirectory(common/modbus/mb_slave)
add_subdirectory(common/modbus/mb_master)
add_subdirectory(libs/nanoModbus)
