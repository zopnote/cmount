if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(PLATFORM_DYLIB_ENDING .dll)

elseif(CMAKE_SYSTEM_NAME STREQUAL APPLE)
    set(PLATFORM_DYLIB_ENDING .dylib)

elseif(CMAKE_SYSTEM_NAME STREQUAL UNIX)
    set(PLATFORM_DYLIB_ENDING .so)

else()
    message(FATAL_ERROR "Operating system isn't supported.")
endif()

set(LINK_DIRECTORIES ${CMAKE_BINARY_DIR}/runtime/lib)

include(${CMAKE_SOURCE_DIR}/vendor/cjson_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/llvm_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/curl_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/yaml_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/cthreads_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/klib_conf.cmake)