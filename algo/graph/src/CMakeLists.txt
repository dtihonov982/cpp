cmake_minimum_required(VERSION 3.25)
set(GRAPH_SOURCE graph.c graph.h)
add_library(graph STATIC ${GRAPH_SOURCE})
