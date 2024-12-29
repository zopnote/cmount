if(NOT YAML_DIR OR YAML_DIR STREQUAL null)
    message(FATAL_ERROR "YAML_DIR is not set.")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(YAML_BINARY
        ${YAML_DIR}/bin/libyaml-0-2.dll
    )
    file(GLOB YAML_LIBRARIES ${YAML_DIR}/lib/*.a)


else()
    if(APPLE)
        set(YAML_BINARY
            ${YAML_DIR}/bin/libyaml-0-2.dylib
        )
        file(GLOB YAML_LIBRARIES ${YAML_DIR}/lib/*.a)

    elseif(UNIX)
        set(YAML_BINARY
            ${YAML_DIR}/bin/libyaml-0-2.so
        )
        file(GLOB YAML_LIBRARIES ${YAML_DIR}/lib/*.a)

    else()
        message(FATAL_ERROR "Operating system isn't supported.")
    endif()
endif()


add_library(YAML_ALL INTERFACE)

target_include_directories(YAML_ALL INTERFACE ${YAML_DIR}/include)

target_link_libraries(YAML_ALL INTERFACE ${YAML_LIBRARIES})

install(FILES ${YAML_BINARY} DESTINATION runtime/lib)