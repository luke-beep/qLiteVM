cmake_minimum_required(VERSION 3.26)

project(qLiteVM C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_COMPILER gcc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

add_executable(qLiteVM main.c
        io_control.c
        io_control.h
        vm_memory.c
        vm_memory.h
        vm_utilities.c
        vm_utilities.h
        vm_core.c
        vm_core.h
        vm_image.c
        vm_image.h
        vm_registers.h
        vm_instructions.h
        vm_globals.h
        main.h)
