set(CVM_SOURCE ${CMAKE_SOURCE_DIR}/jit)
set(CVM_BINARY ${CMAKE_BINARY_DIR}/jit)

file(GLOB CVM_FILES ${CVM_SOURCE}/*)
file(COPY ${CVM_FILES} DESTINATION ${CVM_BINARY})

add_executable(jit ${CVM_BINARY}/src/main.c)

target_link_libraries(jit PRIVATE LLVM_ALL)

install(TARGETS jit DESTINATION bin)
