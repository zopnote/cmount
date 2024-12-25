
add_executable(cmount ${CMAKE_SOURCE_DIR}/bin/src/main.c)

target_link_libraries(cmount LLVM_ALL CURL_ALL CJSON_ALL YAML_ALL CTHREADS_ALL)

install(TARGETS cmount DESTINATION bin)

add_subdirectory(c-jit)
