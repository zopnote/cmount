if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(PLATFORM_DYLIB_ENDING .dll)
    set(PLATFORM_STLIB_ENDING .lib)
    set(PLATFORM_LIB_PREFIX "")
elseif(CMAKE_SYSTEM_NAME STREQUAL APPLE)
    set(PLATFORM_DYLIB_ENDING .dylib)
    set(PLATFORM_STLIB_ENDING .a)
    set(PLATFORM_LIB_PREFIX lib)

elseif(CMAKE_SYSTEM_NAME STREQUAL UNIX)
    set(PLATFORM_DYLIB_ENDING .so)
    set(PLATFORM_STLIB_ENDING .a)
    set(PLATFORM_LIB_PREFIX lib)

else()
    message(FATAL_ERROR "Operating system isn't supported.")
endif()


include(${CMAKE_SOURCE_DIR}/vendor/llvm_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/curl_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/yaml_conf.cmake)
include(${CMAKE_SOURCE_DIR}/vendor/dmon_conf.cmake)