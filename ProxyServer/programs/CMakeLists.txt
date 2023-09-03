cmake_minimum_required (VERSION 3.25)
add_executable (proxy proxy.cpp)
target_link_libraries (proxy PRIVATE ProxyServer)
target_include_directories (proxy PUBLIC "${PROJECT_SOURCE_DIR}/lib")
