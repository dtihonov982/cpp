cmake_minimum_required(VERSION 3.25)
add_executable(io io.c)
target_link_libraries(io PRIVATE graph)
target_include_directories(graph PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(queue_test queue_test.c)
target_link_libraries(queue_test PRIVATE queue)
target_include_directories(queue_test PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(bfs bfs.c)
target_link_libraries(bfs PRIVATE graph)
target_include_directories(bfs PUBLIC "${PROJECT_SOURCE_DIR}/src")
