set(RES_SRC_DIRECTORIES
    config
    internal
)
set(RES_OUT_DIRECTORIES
    ${OUT_CONFIG_DIRECTORY}
    ${OUT_INTERNAL_DIRECTORY}
)

foreach(INDEX RANGE 0 1)

    list(GET RES_SRC_DIRECTORIES ${INDEX} DIRECTORY)
    list(GET RES_OUT_DIRECTORIES ${INDEX} OUT_DIRECTORY)

    file(GLOB_RECURSE ENTITIES
        ${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY}/*
    )

    foreach(ENTITY ${ENTITIES})
        get_filename_component(ENTITY_EXTENSION ${ENTITY} EXT)

        if(ENTITY_EXTENSION STREQUAL .yaml OR ENTITY_EXTENSION STREQUAL .json)
            install(FILES ${ENTITY} DESTINATION ${OUT_DIRECTORY})
        else()

            get_filename_component(ENTITY_FILE_NAME ${ENTITY} NAME)

            string(REPLACE ".in" "" BASE_NAME ${ENTITY_FILE_NAME})

            if(${ENTITY} MATCHES ".*\\.in$")
                configure_file(${ENTITY} ${CMAKE_BINARY_DIR}/${BASE_NAME} @ONLY)
                install(FILES ${CMAKE_BINARY_DIR}/${BASE_NAME} DESTINATION ${OUT_DIRECTORY})
            endif()

        endif()
    endforeach()
endforeach()

install(DIRECTORY templates DESTINATION ${OUT_RESOURCE_DIRECTORY})
