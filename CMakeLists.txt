cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_COMPILER CLANG)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    set(CUSTOM_FLAGS " -static -fno-stack-protector -Wno-unknown-pragmas -Wno-deprecated-declarations -Wno-microsoft")
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    set(CUSTOM_FLAGS " -g -fstack-protector -Wno-unknown-pragmas -Wno-deprecated-declarations -Wno-microsoft")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}${CUSTOM_FLAGS} -std=c2x")


project(cmount VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/.out/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})

set(SRC_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

install(DIRECTORY ${SRC_INCLUDE_DIRECTORY} DESTINATION ./)

# Central definitions of output project structure
include(${CMAKE_SOURCE_DIR}/structure.cmake)

# Builds all third party libraries
include(${CMAKE_SOURCE_DIR}/vendor.cmake)

# Builds first party libraries and the executable
include(${CMAKE_SOURCE_DIR}/components.cmake)

add_subdirectory(resources)
