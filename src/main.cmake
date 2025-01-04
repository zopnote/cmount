
file(GLOB_RECURSE FILES_MAIN_EXECUTABLE
    ${COMPONENT_DIRECTORY}/main/*.c
)

add_executable(cmount ${FILES_MAIN_EXECUTABLE})

target_link_libraries(cmount
    LLVM CURL CJSON YAML CTHREADS KLIB
)




install(TARGETS cmount DESTINATION bin)