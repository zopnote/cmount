if(NOT CURL_DIRECTORY)
    message(FATAL_ERROR "CURL_DIR is not set.")
endif()

file(GLOB CURL_BINARIES ${CURL_DIRECTORY}/bin/*${PLATFORM_DYLIB_ENDING})

file(GLOB CURL_LIBRARIES ${CURL_DIRECTORY}/lib/*.a)

add_library(CURL INTERFACE)

target_include_directories(CURL INTERFACE ${CURL_DIRECTORY}/include)

target_link_libraries(CURL INTERFACE ${CURL_LIBRARIES})

install(FILES ${CURL_BINARIES} DESTINATION runtime/lib)