cmake_minimum_required (VERSION 3.25)
add_executable (server server.cpp)
target_include_directories (server PUBLIC "${PROJECT_SOURCE_DIR}/lib")
