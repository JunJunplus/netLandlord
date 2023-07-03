cmake_minimum_required(VERSION 3.12.2)

set (CMAKE_C_COMPILER "/usr/arm/5.4.0/usr/bin/arm-linux-gcc")

project(client)

include_directories(./inc)

link_directories(./lib)

aux_source_directory(./src s)

add_executable(client ${s})

target_link_libraries(client pthread font m)