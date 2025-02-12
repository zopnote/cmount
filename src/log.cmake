
file(GLOB_RECURSE LOG_SOURCE_FILES ${SRC_COMPONENT_DIRECTORY}/log/*.c)

add_library(log SHARED ${LOG_SOURCE_FILES})

target_include_directories(log PUBLIC ${SRC_INCLUDE_DIRECTORY})

install(TARGETS log
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
)