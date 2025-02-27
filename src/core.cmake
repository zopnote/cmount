
set(CORE_SOURCE_FILES
        ${SRC_COMPONENT_DIRECTORY}/core/logger.c
        ${SRC_COMPONENT_DIRECTORY}/core/platform.c
        ${SRC_COMPONENT_DIRECTORY}/core/common.c
)

add_library(core STATIC ${CORE_SOURCE_FILES})

target_include_directories(core PUBLIC ${SRC_INCLUDE_DIRECTORY})

install(TARGETS core ARCHIVE DESTINATION lib)
