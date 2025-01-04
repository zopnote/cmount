if(NOT YAML_DIRECTORY)
    message(FATAL_ERROR "YAML_DIRECTORY is not set.")
endif()

file(GLOB YAML_BINARIES ${YAML_DIRECTORY}/bin/*${PLATFORM_DYLIB_ENDING})
file(GLOB YAML_LIBRARIES ${YAML_DIRECTORY}/lib/*.a)

add_library(YAML INTERFACE)

target_include_directories(YAML INTERFACE ${YAML_DIRECTORY}/include)

target_link_libraries(YAML INTERFACE ${YAML_LIBRARIES})

install(FILES ${YAML_BINARIES} DESTINATION runtime/lib)