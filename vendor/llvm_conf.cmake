find_package(LLVM REQUIRED CONFIG)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LLVM_BINARIES
        ${LLVM_BINARY_DIR}/bin/LLVM-C.dll
        ${LLVM_BINARY_DIR}/bin/libclang.dll
        ${LLVM_BINARY_DIR}/bin/LLVM-C.dll
    )
    file(GLOB LLVM_LIBRARIES ${LLVM_LIBRARY_DIR}/*.lib)


else()
    if(APPLE)
    file(GLOB LLVM_ALL_BIN ${LLVM_LIBRARY_DIR}/libLLVM*.a)
    set(LLVM_BINARIES
        ${LLVM_BINARY_DIR}/bin/libclang.dylib
        LLVM_ALL_BIN
    )
    file(GLOB LLVM_LIBRARIES ${LLVM_LIBRARY_DIR}/*.lib)


elseif(UNIX)
    file(GLOB LLVM_ALL_BIN ${LLVM_LIBRARY_DIR}/libLLVM*.a)
    set(LLVM_BINARIES
        ${LLVM_BINARY_DIR}/bin/libclang.so
        LLVM_ALL_BIN
    )
    file(GLOB LLVM_LIBRARIES ${LLVM_LIBRARY_DIR}/*.lib)


endif()
    message(FATAL_ERROR "Operating system isn't supported.")
endif()


add_library(LLVM_ALL INTERFACE)

target_include_directories(LLVM_ALL INTERFACE ${LLVM_INCLUDE_DIRS})

target_link_libraries(LLVM_ALL INTERFACE ${LLVM_LIBRARIES})

install(FILES ${LLVM_BINARIES} DESTINATION runtime/lib)