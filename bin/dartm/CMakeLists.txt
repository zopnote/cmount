set(BIN_DARTM_SOURCE ${CMAKE_SOURCE_DIR}/bin/dartm)
set(BIN_DARTM_BINARY ${CMAKE_BINARY_DIR}/bin/dartm)

file(GLOB BIN_DARTM_FILES ${BIN_DARTM_SOURCE}/*)
file(COPY ${BIN_DARTM_FILES} DESTINATION ${BIN_DARTM_BINARY})

add_executable(dartm ${BIN_DARTM_BINARY}/src/main.c)

target_link_libraries(dartm PRIVATE LLVM_ALL)

install(TARGETS dartm DESTINATION bin)
