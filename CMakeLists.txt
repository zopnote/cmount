cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(cmount VERSION 0.1)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

string(TOLOWER
        "${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_NAME
)

set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/out/${LOWER_CMAKE_SYSTEM_NAME}/${PROJECT_NAME}"
)

set(SRC_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

if(EXISTS ${SRC_INCLUDE_DIRECTORY})
    install(DIRECTORY ${SRC_INCLUDE_DIRECTORY}/ DESTINATION include FILES_MATCHING PATTERN "*.h")
endif()

# Variables which defines the structure of the output, installed, binaries.
include(${CMAKE_SOURCE_DIR}/structure.cmake)

# Configurations of third party projects
include(${CMAKE_SOURCE_DIR}/vendor.cmake)

# Actual source configuration of CMount
include(${CMAKE_SOURCE_DIR}/components.cmake)

# Runtime resources that should be installed
add_subdirectory(runtime)
