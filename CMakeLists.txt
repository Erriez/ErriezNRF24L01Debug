cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

#-------------------------------------------------------------------------------
# User configuration
#-------------------------------------------------------------------------------
# Set Arduino sketchbook directory
if(WIN32)
    set(SKETCHBOOK_DIR  C:/Users/$ENV{USERNAME}/Documents/Arduino)
elseif(UNIX)
    set(SKETCHBOOK_DIR  $ENV{HOME}/Arduino/libraries)
endif()

# Configure one sketch
set(SKETCH      examples/DumpRegisters/DumpRegisters.ino)

# Configure board, default Arduino UNO
set(BOARD       uno)
#set(BOARD       nano)

if(${BOARD} STREQUAL nano)
    set(nano.upload.maximum_size 30720)
    set(nano.upload.maximum_data_size 2048)
    set(nano.upload.speed 57600)
    set(nano.build.mcu atmega328p)
endif()

# Configure serial port:
if(WIN32)
    set(SERIAL_PORT     COM1)
elseif(UNIX)
    set(SERIAL_PORT     /dev/ttyUSB0)
endif()

#-------------------------------------------------------------------------------
# Project configuation
#-------------------------------------------------------------------------------
# Project name
set(PROJECT_NAME        ErriezNRF24L01Iface)

# Set library directories
set(NRF24L01_DEBUG_LIB_SRC_DIR  ${SKETCHBOOK_DIR}/libraries/ErriezNRF24L01Debug/src)
set(NRF24L01_IFACE_LIB_SRC_DIR  ${SKETCHBOOK_DIR}/libraries/ErriezNRF24L01Iface/src)
set(PRINTF_LIB_SRC_DIR          ${SKETCHBOOK_DIR}/libraries/ErriezPrintf/src)

# Configure project
project(${PROJECT_NAME})

# Configure include directories
include_directories(
        ${NRF24L01_DEBUG_LIB_SRC_DIR}
        ${NRF24L01_IFACE_LIB_SRC_DIR}
        ${PRINTF_LIB_SRC_DIR}
)

# Generate library
generate_arduino_library(
        NRF24L01DebugLib
        SRCS
            ${NRF24L01_DEBUG_LIB_SRC_DIR}/nRF24L01Debug.cpp
        HDRS
            ${NRF24L01_DEBUG_LIB_SRC_DIR}/nRF24L01Debug.h
)

# Generate library
generate_arduino_library(
        NRF24L01IfaceLib
        SRCS
            ${NRF24L01_IFACE_LIB_SRC_DIR}/nRF24L01Iface.cpp
        HDRS
            ${NRF24L01_IFACE_LIB_SRC_DIR}/nRF24L01Iface.h
)

# Configure project
set(${CMAKE_PROJECT_NAME}_SKETCH    ${SKETCH})
set(${CMAKE_PROJECT_NAME}_LIBS      NRF24L01DebugLib NRF24L01IfaceLib)
set(${CMAKE_PROJECT_NAME}_BOARD     ${BOARD})
set(${CMAKE_PROJECT_NAME}_PORT      ${SERIAL_PORT})
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
