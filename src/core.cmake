file(GLOB_RECURSE CORE_FILES
    ${COMPONENT_DIRECTORY}/core/*.c
)

add_library(cmountcore SHARED ${CORE_FILES})

target_include_directories(cmountcore PRIVATE ${INCLUDE_DIRECTORY}/core)

target_include_directories(cmountcore INTERFACE ${INCLUDE_DIRECTORY})

target_link_libraries(cmountcore PUBLIC YAML KLIB)

set(CORE_BINARY_DYLIB ${CMAKE_BINARY_DIR}/libcmountcore${PLATFORM_DYLIB_ENDING})

install(FILES ${CORE_BINARY_DYLIB} DESTINATION bin)

install(FILES ${CORE_BINARY_DYLIB}.a DESTINATION lib)
