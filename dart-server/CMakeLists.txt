set(DARTM_SOURCE ${CMAKE_SOURCE_DIR}/dart-server)
set(DARTM_BINARY ${CMAKE_BINARY_DIR}/dart-server)

file(GLOB DARTM_FILES ${DARTM_SOURCE}/*)
file(COPY ${DARTM_FILES} DESTINATION ${DARTM_BINARY})

add_executable(dartserv ${DARTM_BINARY}/src/main.c)

target_link_libraries(dartserv PRIVATE LLVM_ALL)

install(TARGETS dartserv DESTINATION bin)
