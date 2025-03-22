
find_package(LLVM REQUIRED CONFIG)

if(NOT LLVM_BINARY_DIR)
    message(FATAL_ERROR "LLVM not found. Specify LLVM_BINARY_DIR and LLVM_LIBRARY_DIR directly.")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(LLVM_SHARED_LIB_BINARIES
        ${LLVM_BINARY_DIR}/bin/LLVM-C.dll
        ${LLVM_BINARY_DIR}/bin/libclang.dll
    )
    file(GLOB LLVM_LIBRARIES ${LLVM_LIBRARY_DIR}/*.lib)

elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(LLVM_SHARED_LIB_BINARIES
        ${LLVM_LIBRARY_DIR}/libLLVM.dylib
        ${LLVM_LIBRARY_DIR}/libclang.dylib
    )
    file(GLOB LLVM_LIBRARIES ${LLVM_LIBRARY_DIR}/lib*.a)
else ()
    set(LLVM_SHARED_LIB_BINARIES
            ${LLVM_LIBRARY_DIR}/libLLVM.so
    )
    file(GLOB LLVM_LIBRARIES ${LLVM_LIBRARY_DIR}/lib*.a)
    install(FILES ${LLVM_LIBRARY_DIR}/libclang.so.1
            DESTINATION ${OUT_BINARY_DIRECTORY}
            RENAME libclang.so)
endif()

add_library(llvm INTERFACE)

target_include_directories(llvm INTERFACE ${LLVM_INCLUDE_DIRS})

target_link_libraries(llvm INTERFACE ${LLVM_LIBRARIES})

install(FILES
        ${LLVM_SHARED_LIB_BINARIES} DESTINATION ${OUT_BINARY_DIRECTORY}
)