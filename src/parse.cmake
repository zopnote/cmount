
file(GLOB_RECURSE PARSE_SOURCE_FILES ${SRC_COMPONENT_DIRECTORY}/parse/*.c)

add_library(parse SHARED ${PARSE_SOURCE_FILES})

target_include_directories(parse PUBLIC ${SRC_INCLUDE_DIRECTORY})

target_link_libraries(parse
        core
        yaml
)

install(TARGETS parse
        COMPONENT DESTINATION ${OUT_BINARY_DIRECTORY}
        RUNTIME DESTINATION ${OUT_BINARY_DIRECTORY}
        ARCHIVE DESTINATION ${OUT_LIBRARY_DIRECTORY}
)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    install(
            FILES $<TARGET_PDB_FILE:parse>
            DESTINATION bin
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif ()