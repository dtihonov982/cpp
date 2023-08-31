cmake_minimum_required(VERSION 3.25)
find_package(GTest REQUIRED)

set (SRCS MovRM.cpp)

add_executable(test ${SRCS})
target_link_libraries(test PRIVATE GTest::GTest GTest::Main)
target_include_directories(test PUBLIC "${PROJECT_SOURCE_DIR}/lib")
