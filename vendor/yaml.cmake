set(YAML_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/libyaml)

if (NOT EXISTS ${YAML_DIRECTORY})
    execute_process(
            COMMAND git clone https://github.com/zopnote/libyaml.git
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps
    )
endif ()



set (YAML_VERSION_MAJOR 0)
set (YAML_VERSION_MINOR 2)
set (YAML_VERSION_PATCH 5)
set (YAML_VERSION_STRING "${YAML_VERSION_MAJOR}.${YAML_VERSION_MINOR}.${YAML_VERSION_PATCH}")

set(config_h ${YAML_DIRECTORY}/include/config.h)

configure_file(
        ${YAML_DIRECTORY}/cmake/config.h.in
        ${config_h}
)

set(YAML_SOURCE_FILES
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/api.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/dumper.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/emitter.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/loader.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/parser.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/reader.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/scanner.c
        ${CMAKE_BINARY_DIR}/_deps/libyaml/src/writer.c
)

add_library(yaml SHARED ${YAML_SOURCE_FILES})

target_compile_definitions(yaml
        PRIVATE HAVE_CONFIG_H
        PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:YAML_DECLARE_STATIC>
        $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
)

target_compile_definitions(yaml
        PRIVATE HAVE_CONFIG_H
        PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:YAML_DECLARE_STATIC>
        $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
)

set_target_properties(yaml
        PROPERTIES DEFINE_SYMBOL YAML_DECLARE_EXPORT
)

target_include_directories(yaml PUBLIC ${CMAKE_BINARY_DIR}/_deps/libyaml/include)

install(TARGETS yaml
        COMPONENT DESTINATION ${OUT_BINARY_DIRECTORY}
        RUNTIME DESTINATION ${OUT_BINARY_DIRECTORY}
        ARCHIVE DESTINATION ${OUT_LIBRARY_DIRECTORY}
)