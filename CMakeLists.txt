cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

#-------------------------------------------------------------------------------
# User configuration
#-------------------------------------------------------------------------------
# Project name
set(PROJECT_NAME    ArduinoLibraryNRF24L01Debug)

# Library directory
set(LIBRARY_DIR     C:/Users/User/Documents/Arduino/libraries)

# Configure board, default Arduino UNO
set(BOARD           uno)

# Configure sketch
set(SKETCH          examples/DumpRegisters/DumpRegisters.ino)

# Configure serial port, such as:
#     Windows: COM1
#     Linux:   /dev/ttyUSB0
set(SERIAL_PORT     COM1)

#-------------------------------------------------------------------------------
# Project configuation
#-------------------------------------------------------------------------------
# Configure project
project(${PROJECT_NAME})

set(NRF24L01_DEBUG_LIBRARY_DIR  ${LIBRARY_DIR}/ArduinoLibraryNRF24L01Debug/src)
set(NRF24L01_IFACE_LIBRARY_DIR  ${LIBRARY_DIR}/ArduinoLibraryNRF24L01Iface/src)
set(PRINTF_LIBRARY_DIR          ${LIBRARY_DIR}/ArduinoLibraryPrintf/src)

include_directories(
        ${NRF24L01_DEBUG_LIBRARY_DIR}
        ${NRF24L01_IFACE_LIBRARY_DIR}
        ${PRINTF_LIBRARY_DIR}
)

generate_arduino_library(
        NRF24L01DebugLib
        SRCS
            ${NRF24L01_DEBUG_LIBRARY_DIR}/nRF24L01Debug.cpp
        HDRS
            ${NRF24L01_DEBUG_LIBRARY_DIR}/nRF24L01Debug.h
)

generate_arduino_library(
        NRF24L01IfaceLib
        SRCS
        ${NRF24L01_IFACE_LIBRARY_DIR}/nRF24L01Iface.cpp
        HDRS
        ${NRF24L01_IFACE_LIBRARY_DIR}/nRF24L01Iface.h
)

set(${CMAKE_PROJECT_NAME}_SKETCH    ${SKETCH})
set(${CMAKE_PROJECT_NAME}_LIBS      NRF24L01DebugLib NRF24L01IfaceLib)
set(${CMAKE_PROJECT_NAME}_BOARD     ${BOARD})
set(${CMAKE_PROJECT_NAME}_PORT      ${SERIAL_PORT})

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
