cmake_minimum_required (VERSION 3.25)
project (DesignPatterns)

set (SRCS adapter.cpp
          bridge.cpp
          factory.cpp
          abstract_factory.cpp
          state.cpp
          builder.cpp
          interpreter.cpp
          chain.cpp
          decorator.cpp
          flyweight.cpp
          observer.cpp
          prototype.cpp
          visitor.cpp
          MVC.cpp
          facade.cpp
          null_object.cpp
)

foreach (SRC IN LISTS SRCS)
    string (REPLACE ".cpp" "" TGT ${SRC})
    add_executable(${TGT} ${SRC})
    target_compile_options (${TGT} PUBLIC -fsanitize=address)
    target_link_options (${TGT} PUBLIC -fsanitize=address)
endforeach ()
