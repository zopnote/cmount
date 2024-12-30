set(DARTM_SOURCE ${CMAKE_SOURCE_DIR}/dart-server)
set(DARTM_BINARY ${CMAKE_BINARY_DIR}/dart-server)

file(GLOB DARTM_FILES ${DARTM_SOURCE}/*)
file(COPY ${DARTM_FILES} DESTINATION ${DARTM_BINARY})

file(GLOB DARTM_SOURCES ${DARTM_SOURCE}/src/***.c)
add_library(dartserv SHARED ${DARTM_SOURCES})

target_include_directories(dartserv PRIVATE ${DARTM_BINARY}/include)

target_link_libraries(dartserv PRIVATE LLVM_ALL)

file(GLOB DARTM_HEADER ${DARTM_BINARY}/include/*)
target_include_directories(jit PRIVATE DARTM_HEADER)

install(FILES ${DARTM_HEADER} DESTINATION include)

install(TARGETS dartserv
LIBRARY DESTINATION bin)
