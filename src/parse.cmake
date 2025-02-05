file(GLOB_RECURSE PARSE_SOURCE_FILES
        ${SRC_COMPONENT_DIRECTORY}/parse/**.c
)

add_library(parse SHARED ${PARSE_SOURCE_FILES})

target_link_libraries(parse PRIVATE YAML)

target_include_directories(parse PUBLIC ${SRC_INCLUDE_DIRECTORY})


install(TARGETS parse
        COMPONENT DESTINATION bin
        ARCHIVE DESTINATION lib
)