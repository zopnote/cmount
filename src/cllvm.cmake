
set(CLLVM_SOURCE_FILES
        ${SRC_COMPONENT_DIRECTORY}/cllvm/module.cpp
)

add_library(cllvm SHARED ${CLLVM_SOURCE_FILES})

target_include_directories(cllvm PUBLIC ${SRC_INCLUDE_DIRECTORY})

target_link_libraries(cllvm
        core
        llvm
)

install(TARGETS cllvm
        COMPONENT DESTINATION ${OUT_BINARY_DIRECTORY}
        RUNTIME DESTINATION ${OUT_BINARY_DIRECTORY}
        ARCHIVE DESTINATION ${OUT_LIBRARY_DIRECTORY}
)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    install(
            FILES $<TARGET_PDB_FILE:cllvm>
            DESTINATION bin
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif ()