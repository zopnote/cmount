cmake_minimum_required(VERSION 3.13)

find_program(CLANG_COMPILER clang)
if(NOT CLANG_COMPILER)
    message(FATAL_ERROR "Clang wurde nicht gefunden!")
endif()
set(CMAKE_C_COMPILER "${CLANG_COMPILER}")

set(CMAKE_C_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x -Wno-microsoft -Wno-deprecated-declarations")

project(cmount VERSION 0.1)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/.out/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}" CACHE PATH "Install path" FORCE)

set(SRC_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

if(EXISTS ${SRC_INCLUDE_DIRECTORY})
    install(DIRECTORY ${SRC_INCLUDE_DIRECTORY}/ DESTINATION include FILES_MATCHING PATTERN "*.h")
endif()

foreach(file structure.cmake vendor.cmake components.cmake)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${file}")
        include("${CMAKE_SOURCE_DIR}/${file}")
    else()
        message(FATAL_ERROR "Fehlende Datei: ${file}")
    endif()
endforeach()

if(EXISTS "${CMAKE_SOURCE_DIR}/resources/CMakeLists.txt")
    add_subdirectory(resources)
else()
    message(WARNING "Kein resources/CMakeLists.txt gefunden. Überspringe resources.")
endif()
