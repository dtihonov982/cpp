cmake_minimum_required(VERSION 3.25)
set (SRCS queue_test.c
          bfs.c
          dfs.c
          path.c
          twocolor.c
)

foreach (SRC IN LISTS SRCS)
    string (REPLACE ".c" "" TGT ${SRC})
    message (STATUS ${TGT})
    add_executable(${TGT} ${SRC})
    target_link_libraries(${TGT} PRIVATE graph)
    target_include_directories(${TGT} PUBLIC "${PROJECT_SOURCE_DIR}/src")
endforeach ()
