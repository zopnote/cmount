set(BINJIT_SOURCE ${CMAKE_SOURCE_DIR}/bin/c-jit)
set(BINJIT_BINARY ${CMAKE_BINARY_DIR}/bin/c-jit)

file(GLOB BINJIT_FILES ${BINJIT_SOURCE}/*)
file(COPY ${BINJIT_FILES} DESTINATION ${BINJIT_BINARY})

add_executable(cjit ${BINJIT_BINARY}/src/main.c)

target_link_libraries(cjit PRIVATE CURL_ALL)

install(TARGETS cjit DESTINATION bin)
