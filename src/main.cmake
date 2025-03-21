
set(SRC_FILES_MAIN_EXECUTABLE
        ${SRC_COMPONENT_DIRECTORY}/main/main.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/build.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/config.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/create.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/doctor.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/get.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/install.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/run.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/upgrade.c
        ${SRC_COMPONENT_DIRECTORY}/main/tasks/version.c
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