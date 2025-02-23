include(ExternalProject)

ExternalProject_Add(
        ext_dmon
        GIT_REPOSITORY https://github.com/zopnote/dmon.git
        GIT_TAG master
        PREFIX ${PROJECT_BINARY_DIR}/_deps/dmon
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)

add_library(dmon INTERFACE)

target_include_directories(dmon INTERFACE ${PROJECT_BINARY_DIR}/_deps/dmon)