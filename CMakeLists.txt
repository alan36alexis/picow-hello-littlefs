# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picow-hello-littlefs C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Añadir los archivos fuente de LittleFS

set(LITTLEFS_SOURCES 
    littlefs/lfs.c
    littlefs/lfs_util.c
    )

# Add executable. Default name is the project name, version 0.1

add_executable(picow-hello-littlefs 
    picow-hello-littlefs.c 
    ${LITTLEFS_SOURCES}
    )

pico_set_program_name(picow-hello-littlefs "picow-hello-littlefs")
pico_set_program_version(picow-hello-littlefs "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picow-hello-littlefs 1)
pico_enable_stdio_usb(picow-hello-littlefs 0)

# Add the standard library to the build
target_link_libraries(picow-hello-littlefs
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(picow-hello-littlefs PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/littlefs
)

# Add any user requested libraries
target_link_libraries(picow-hello-littlefs 
        pico_cyw43_arch_none
        pico_stdlib
        hardware_flash
        hardware_sync
        )

pico_add_extra_outputs(picow-hello-littlefs)

