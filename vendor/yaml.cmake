include(FetchContent)

FetchContent_Declare(
        yaml
        GIT_REPOSITORY https://github.com/zopnote/libyaml.git
        EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(yaml)