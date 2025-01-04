cmake_minimum_required(VERSION 3.13)
project(cmount)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}/cmount)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB FILES_MAIN_EXECUTABLE
    ${CMAKE_SOURCE_DIR}/src/*.c
)
add_executable(cmount ${FILES_MAIN_EXECUTABLE})

include(${CMAKE_SOURCE_DIR}/vendor.cmake)

target_link_libraries(cmount
    LLVM CURL CJSON YAML CTHREADS KLIB
)

install(TARGETS cmount DESTINATION bin)