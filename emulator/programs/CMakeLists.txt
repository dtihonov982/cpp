cmake_minimum_required (VERSION 3.25)
add_executable (fib fib.cpp)
target_link_libraries (fib PUBLIC Emulator)
target_include_directories (fib PUBLIC "${PROJECT_SOURCE_DIR}/lib/")

add_executable (assembler assembler.cpp)
target_link_libraries (assembler PUBLIC Emulator)
target_include_directories (assembler PUBLIC "${PROJECT_SOURCE_DIR}/lib/")
