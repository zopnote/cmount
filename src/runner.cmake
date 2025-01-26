file(GLOB_RECURSE RUNNER_SOURCE_FILES
    ${SRC_COMPONENT_DIRECTORY}/runner/*.c
)
add_library(runner SHARED ${RUNNER_SOURCE_FILES})

target_include_directories(runner PUBLIC ${SRC_INCLUDE_DIRECTORY}/runner)

install(TARGETS runner
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
)