cmake_minimum_required(VERSION 3.25)
set(GRAPH_SOURCE graph.c)
add_library(graph STATIC ${GRAPH_SOURCE})

set(QUEUE_SOURCE queue.c)
add_library(queue STATIC ${QUEUE_SOURCE})

target_link_libraries(graph queue)
target_include_directories(graph PUBLIC ${PROJECT_SOURCE_DIR}/src)
