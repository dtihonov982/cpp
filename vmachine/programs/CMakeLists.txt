cmake_minimum_required (VERSION 3.25)
add_executable (fib fib.cpp)
target_include_directories (fib PUBLIC "${PROJECT_SOURCE_DIR}/lib/")
