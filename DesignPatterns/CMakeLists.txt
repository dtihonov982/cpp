cmake_minimum_required (VERSION 3.25)
set (SRCS adapter.cpp
          bridge.cpp
          builder.cpp
          chain.cpp
          decorator.cpp
          flyweight.cpp
          observer.cpp
          prototype.cpp
          visitor.cpp
)

foreach (SRC IN LISTS SRCS)
    string (REPLACE ".cpp" "" TGT ${SRC})
    add_executable(${TGT} ${SRC})
endforeach ()
