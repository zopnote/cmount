cmake_minimum_required(VERSION 3.13)

project(cmount)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}/cmount)

set(INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

install(DIRECTORY ${INCLUDE_DIRECTORY} DESTINATION ./)


# Builds all third party libraries
include(${CMAKE_SOURCE_DIR}/vendor.cmake)


# Builds first party libraries and the executable
include(${CMAKE_SOURCE_DIR}/components.cmake)

