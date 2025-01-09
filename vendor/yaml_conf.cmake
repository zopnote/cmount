if(NOT YAML_DIRECTORY)
    message(FATAL_ERROR "YAML_DIRECTORY is not set.")
endif()

file(GLOB YAML_LIBRARIY ${YAML_DIRECTORY}/lib/libyaml.a)

add_library(YAML INTERFACE)

target_include_directories(YAML INTERFACE ${YAML_DIRECTORY}/include)

target_link_libraries(YAML INTERFACE ${YAML_LIBRARIY})
