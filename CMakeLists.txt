cmake_minimum_required(VERSION 3.13)
project(cmount)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_SOURCE_DIR}/vendor/cjson_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/llvm_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/curl_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/yaml_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/cthreads_conf.cmake)

add_subdirectory(bin)