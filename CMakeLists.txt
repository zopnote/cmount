cmake_minimum_required(VERSION 3.13)

project(cmount
        DESCRIPTION "Unifying application for efficient and C-dependent program development."
        VERSION 0.1
        LANGUAGES C
)


# Language related
# -----------------------------------------------------------------------------
set(CMAKE_C_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wformat -Wformat-security -Wstack-protector -fstack-protector-strong -Wno-missing-braces")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-stack-protector")
endif ()
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-microsoft -Wno-deprecated-declarations")
endif()
# -----------------------------------------------------------------------------




# Global config
# -----------------------------------------------------------------------------
string(TOLOWER
        "${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}"
        LOWER_CMAKE_BUILD_NAME
)
set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/out/${LOWER_CMAKE_BUILD_NAME}/${PROJECT_NAME}"
)

set(SRC_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

if(EXISTS ${SRC_INCLUDE_DIRECTORY})
    install(DIRECTORY
            ${SRC_INCLUDE_DIRECTORY}/ DESTINATION include FILES_MATCHING PATTERN "*.h"
    )
endif()
# -----------------------------------------------------------------------------




# Variables which defines the structure of the output, installed, binaries.
# -----------------------------------------------------------------------------

set(MAIN_EXECUTABLE_NAME ${PROJECT_NAME})

# The binary and internal paths are hard coded in the program
set(OUT_BINARY_DIRECTORY bin)
set(OUT_INTERNAL_DIRECTORY ${OUT_BINARY_DIRECTORY}/internal)
set(OUT_CACHE_DIRECTORY ${OUT_BINARY_DIRECTORY}/cache)

set(OUT_DOCUMENTATION_DIRECTORY docs)
set(OUT_LIBRARY_DIRECTORY lib)
set(OUT_INCLUDE_DIRECTORY include)

set(OUT_RUNTIME_DIRECTORY runtime)
set(OUT_UNIT_RUNTIME_DIRECTORY ${OUT_RUNTIME_DIRECTORY}/unit)
set(OUT_CONFIG_DIRECTORY ${OUT_RUNTIME_DIRECTORY}/config)
set(OUT_SCRIPTS_DIRECTORY ${OUT_RUNTIME_DIRECTORY}/scripts)
set(OUT_RESOURCE_DIRECTORY ${OUT_RUNTIME_DIRECTORY}/resources)
set(OUT_TEMPLATES_DIRECTORY ${OUT_RUNTIME_DIRECTORY}/templates)

# -----------------------------------------------------------------------------




# Configurations of third party projects/deps
# -----------------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/vendor/llvm.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/libcurl.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/yaml.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/dmon.cmake)
# -----------------------------------------------------------------------------



# Actual source configuration of CMount
# -----------------------------------------------------------------------------
set(SRC_COMPONENT_DIRECTORY "${CMAKE_SOURCE_DIR}/src")
include("${SRC_COMPONENT_DIRECTORY}/core.cmake")
include("${SRC_COMPONENT_DIRECTORY}/parse.cmake")
include("${SRC_COMPONENT_DIRECTORY}/main.cmake")
# -----------------------------------------------------------------------------


# Runtime resources that should be installed
add_subdirectory(runtime)
