if(NOT CURL_DIRECTORY)
    message(FATAL_ERROR "CURL_DIR is not set.")
endif()

file(GLOB CURL_LIBRARY ${CURL_DIRECTORY}/lib/libcurl.a)

add_library(CURL INTERFACE)

target_include_directories(CURL INTERFACE ${CURL_DIRECTORY}/include)

target_link_libraries(CURL INTERFACE ${CURL_LIBRARY})