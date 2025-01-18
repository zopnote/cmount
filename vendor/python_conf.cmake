
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

add_library(PYTHON INTERFACE)

target_include_directories(PYTHON INTERFACE ${Python3_INCLUDE_DIRS})

target_link_libraries(PYTHON INTERFACE ${Python3_LIBRARIES})
