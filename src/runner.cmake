
set(RUNNER_SOURCE_FILES
        ${SRC_COMPONENT_DIRECTORY}/runner/module.cpp
)

add_library(runner SHARED ${RUNNER_SOURCE_FILES})

target_include_directories(runner PUBLIC ${SRC_INCLUDE_DIRECTORY})

target_link_libraries(runner
        core
        llvm
        curl
)

install(TARGETS runner
        COMPONENT DESTINATION ${OUT_BINARY_DIRECTORY}
        RUNTIME DESTINATION ${OUT_BINARY_DIRECTORY}
        ARCHIVE DESTINATION ${OUT_LIBRARY_DIRECTORY}
)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    install(
            FILES $<TARGET_PDB_FILE:runner>
            DESTINATION bin
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif ()