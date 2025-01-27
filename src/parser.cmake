file(GLOB_RECURSE PARSER_SOURCE_FILES
        ${SRC_COMPONENT_DIRECTORY}/parser/**.c
)

add_library(parser SHARED ${PARSER_SOURCE_FILES})

target_include_directories(parser PUBLIC ${SRC_INCLUDE_DIRECTORY}/parser)

target_link_libraries(parser PRIVATE YAML)

install(TARGETS parser
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
)