if(NOT CURL_DIR OR CURL_DIR STREQUAL null)
    message(FATAL_ERROR "CURL_DIR is not set.")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CURL_BINARY
        ${CURL_DIR}/bin/libcurl-x64.dll
    )
    file(GLOB CURL_LIBRARIES ${CURL_DIR}/lib/*.a)


else()
    if(APPLE)
        set(CURL_BINARY
            ${CURL_DIR}/bin/libcurl-x64.dll
        )
        file(GLOB CURL_LIBRARIES ${CURL_DIR}/lib/*.a)

    elseif(UNIX)
        set(CURL_BINARY
            ${CURL_DIR}/bin/libcurl-x64.dll
        )
        file(GLOB CURL_LIBRARIES ${CURL_DIR}/lib/*.a)

    else()
        message(FATAL_ERROR "Operating system isn't supported.")
    endif()
endif()


add_library(CURL_ALL INTERFACE)

target_include_directories(CURL_ALL INTERFACE ${CURL_DIR}/include)

target_link_libraries(CURL_ALL INTERFACE ${CURL_LIBRARIES})

install(FILES ${CURL_BINARY} DESTINATION runtime/lib)