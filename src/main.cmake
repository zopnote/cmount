
set(SRC_FILES_MAIN_EXECUTABLE
        ${SRC_COMPONENT_DIRECTORY}/main/main.c
)
add_executable(${PROJECT_NAME} ${SRC_FILES_MAIN_EXECUTABLE})

target_link_libraries(${PROJECT_NAME} parse core)

install(TARGETS ${PROJECT_NAME} DESTINATION ${OUT_BINARY_DIRECTORY})

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    install(
            FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
            DESTINATION bin
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif ()