cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(cmount)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/.out/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})

set(SRC_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

install(DIRECTORY ${SRC_INCLUDE_DIRECTORY} DESTINATION ./)


# Central definitions of outpur project structure
include(${CMAKE_SOURCE_DIR}/structure.cmake)

# Builds all third party libraries
include(${CMAKE_SOURCE_DIR}/vendor.cmake)

# Builds first party libraries and the executable
include(${CMAKE_SOURCE_DIR}/components.cmake)

add_subdirectory(resources)