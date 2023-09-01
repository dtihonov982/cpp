cmake_minimum_required(VERSION 3.25)
add_library (Emulator Emulator.cpp Translate.cpp)
target_include_directories (Emulator PUBLIC "${PROJECT_SOURCE_DIR}/lib")
