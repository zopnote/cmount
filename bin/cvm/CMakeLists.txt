set(BIN_CVM_SOURCE ${CMAKE_SOURCE_DIR}/bin/cvm)
set(BIN_CVM_BINARY ${CMAKE_BINARY_DIR}/bin/cvm)

file(GLOB BIN_CVM_FILES ${BIN_CVM_SOURCE}/*)
file(COPY ${BIN_CVM_FILES} DESTINATION ${BIN_CVM_BINARY})

add_executable(cvm ${BIN_CVM_BINARY}/src/main.c)

target_link_libraries(cvm PRIVATE LLVM_ALL)

install(TARGETS cvm DESTINATION bin)
