cmake_minimum_required(VERSION 3.5)

#set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)
#set(PICO_BOARD $ENV{PICO_BOARD})
#set(FAMILY $ENV{FAMILY})
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
#set(PICO_TINYUSB_PATH ${PICO_SDK_PATH}/lib/tinyusb)

# Pull in SDK (must be before project)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(cxadc-clock-generator-audio-adc C CXX ASM)

# Initialize the SDK
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

#add_compile_options(-Wall
#        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
#        -Wno-unused-function # we have some for the docs that aren't called
#        -Wno-maybe-uninitialized
#        )


# USB specific

#set(FAMILY rp2040)
#set(BOARD pico_sdk)
#set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")
#add_subdirectory(${PICO_TINYUSB_PATH}/examples/device tinyusb_device_examples)

add_subdirectory("src" "build")
