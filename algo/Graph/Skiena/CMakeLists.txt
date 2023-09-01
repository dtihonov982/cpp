cmake_minimum_required(VERSION 3.25)
project(graph)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_subdirectory(src)
add_subdirectory(test)

