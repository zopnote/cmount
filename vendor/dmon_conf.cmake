if(NOT DMON_DIRECTORY)
    message(FATAL_ERROR "DMON_DIRECTORY is not set.")
endif()

add_library(DMON INTERFACE)

target_include_directories(DMON INTERFACE ${DMON_DIRECTORY})