cmake_minimum_required (VERSION 3.25)
add_library (ProxyServer Socket.cpp Acceptor.cpp)
#target_include_directories (ProxyServer PUBLIC "${PROJECT_SOURCE_DIR}/lib")
