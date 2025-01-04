cmake_minimum_required(VERSION 3.13)
project(cmount)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/out/app)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cmount ${CMAKE_SOURCE_DIR}/src/main.c)

include(${CMAKE_SOURCE_DIR}/vendor.cmake)

target_link_libraries(cmount
    LLVM CURL CJSON YAML CTHREADS
)

install(TARGETS cmount DESTINATION bin)