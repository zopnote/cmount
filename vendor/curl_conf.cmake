
if(NOT CURL_DIRECTORY)
    find_package(CURL REQUIRED)
    if (NOT CURL)
        message(FATAL_ERROR "CURL is not found. Consider to set it explicit by CURL_DIRECTORY var.")
    endif ()
endif()

file(GLOB CURL_LIBRARY ${CURL_DIRECTORY}/lib/libcurl.a)

add_library(CURL INTERFACE)

target_include_directories(CURL INTERFACE ${CURL_DIRECTORY}/include)

target_link_libraries(CURL INTERFACE ${CURL_LIBRARY})