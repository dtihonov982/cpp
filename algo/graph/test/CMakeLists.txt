cmake_minimum_required(VERSION 3.25)
add_executable(io io.c)
target_link_libraries(io PRIVATE graph)
target_include_directories(graph PUBLIC "${PROJECT_SOURCE_DIR}/src")
