
if (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    add_executable(logger_test ${SRC_COMPONENT_DIRECTORY}/test/logger_test.c)
    target_link_libraries(logger_test core)
    add_executable(parse_test ${SRC_COMPONENT_DIRECTORY}/test/parse_test.c)
    target_link_libraries(parse_test core parse)
endif ()