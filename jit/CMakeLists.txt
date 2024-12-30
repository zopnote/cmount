set(CVM_SOURCE ${CMAKE_SOURCE_DIR}/jit)
set(CVM_BINARY ${CMAKE_BINARY_DIR}/jit)

file(GLOB CVM_FILES ${CVM_SOURCE}/*)
file(COPY ${CVM_FILES} DESTINATION ${CVM_BINARY})

file(GLOB CVM_SOURCE_FILES ${CVM_SOURCE}/src/***.c)

add_library(jit SHARED ${CVM_SOURCE_FILES})

file(GLOB CVM_HEADER ${CVM_BINARY}/include/*)
target_include_directories(jit PRIVATE CVM_HEADER)

target_link_libraries(jit PRIVATE LLVM_ALL)
install(FILES ${CVM_HEADER} DESTINATION include)

install(TARGETS jit
LIBRARY DESTINATION bin)
