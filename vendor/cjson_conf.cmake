
if (NOT CJSON_DIR OR CJSON_DIR STREQUAL null)
    message(FATAL_ERROR "CJSON_DIR is not set!")
endif()

set(CJSON_BINARY ${CMAKE_BINARY_DIR}/cJSON)

file(COPY ${CJSON_DIR}/cJSON.c DESTINATION ${CJSON_BINARY}/cjson)
file(COPY ${CJSON_DIR}/cJSON.h DESTINATION ${CJSON_BINARY}/cjson)

add_library(cjson SHARED ${CJSON_BINARY}/cjson/cJSON.c)

add_library(CJSON_ALL INTERFACE)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CJSON_BINARY_DYLIB
        ${CMAKE_BINARY_DIR}/libcjson.dll
    )
else()
    if(APPLE)
        set(CJSON_BINARY_DYLIB
            ${CMAKE_BINARY_DIR}/libcjson.dylib
        )
    elseif(UNIX)
        set(CJSON_BINARY_DYLIB
            ${CMAKE_BINARY_DIR}/libcjson.so
        )
    else()
        message(FATAL_ERROR "Operating system isn't supported.")
    endif()
endif()

target_include_directories(CJSON_ALL INTERFACE ${CJSON_BINARY})

target_link_libraries(CJSON_ALL INTERFACE ${CJSON_BINARY_DYLIB}.a)

install(FILES ${CJSON_BINARY_DYLIB} DESTINATION bin)


