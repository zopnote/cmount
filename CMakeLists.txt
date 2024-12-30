cmake_minimum_required(VERSION 3.13)
project(cmount)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")
set(LINK_DIRECTORIES ${CMAKE_BINARY_DIR}/runtime/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_SOURCE_DIR}/vendor/cjson_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/llvm_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/curl_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/yaml_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/cthreads_conf.cmake)

add_executable(cmount ${CMAKE_SOURCE_DIR}/src/main.c)

target_link_libraries(cmount LLVM_ALL CURL_ALL CJSON_ALL YAML_ALL CTHREADS_ALL dartserv jit)

install(TARGETS cmount DESTINATION bin)

add_subdirectory(jit)
add_subdirectory(dart-server)