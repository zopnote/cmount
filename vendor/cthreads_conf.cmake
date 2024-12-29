
if (NOT CTHREADS_DIR OR CTHREADS_DIR STREQUAL null)
    message(FATAL_ERROR "CTHREADS_DIR is not set!")
endif()

set(CTHREADS_BINARY ${CMAKE_BINARY_DIR}/CThreads)

file(COPY ${CTHREADS_DIR}/cthreads.c DESTINATION ${CTHREADS_BINARY}/cthreads)
file(COPY ${CTHREADS_DIR}/cthreads.h DESTINATION ${CTHREADS_BINARY}/cthreads)

add_library(cthreads SHARED ${CTHREADS_BINARY}/cthreads/cthreads.c)
target_compile_options(cthreads PRIVATE -w)

add_library(CTHREADS_ALL INTERFACE)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CTHREADS_BINARY_DYLIB
        ${CMAKE_BINARY_DIR}/libcthreads.dll
    )
else()
    if(APPLE)
        set(CTHREADS_BINARY_DYLIB
            ${CMAKE_BINARY_DIR}/libcthreads.dylib
        )
    elseif(UNIX)
        set(CTHREADS_BINARY_DYLIB
            ${CMAKE_BINARY_DIR}/libcthreads.so
        )
    else()
        message(FATAL_ERROR "Operating system isn't supported.")
    endif()
endif()

target_include_directories(CTHREADS_ALL INTERFACE ${CTHREADS_BINARY})

target_link_libraries(CTHREADS_ALL INTERFACE ${CTHREADS_BINARY_DYLIB}.a)

install(FILES ${CTHREADS_BINARY_DYLIB} DESTINATION runtime/lib)


