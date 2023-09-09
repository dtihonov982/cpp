# https://www.tutorialspoint.com/design_pattern/design_pattern_overview.htm

cmake_minimum_required (VERSION 3.25)
project (DesignPatterns)

set (SRCS 
            abstract_factory.cpp
            adapter.cpp
            bridge.cpp
            builder.cpp
            chain.cpp
            command.cpp
            composite.cpp
            decorator.cpp
            facade.cpp
            factory.cpp
            filter.cpp
            flyweight.cpp
            interpreter.cpp
            iterator.cpp
            mediator.cpp
            memento.cpp
            MVC.cpp
            null_object.cpp
            observer.cpp
            prototype.cpp
            proxy.cpp
            singletone.cpp
            state.cpp
            strategy.cpp
            template.cpp
            visitor.cpp
)

foreach (SRC IN LISTS SRCS)
    string (REPLACE ".cpp" "" TGT ${SRC})
    add_executable(${TGT} ${SRC})
    target_compile_options (${TGT} PUBLIC -fsanitize=address -Wall)
    target_link_options (${TGT} PUBLIC -fsanitize=address)
endforeach ()
