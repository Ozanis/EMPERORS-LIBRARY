cmake_minimum_required(VERSION 3.12)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled1 main.cpp)
if(UNIX)
    target_link_libraries(untitled1 OPENSSL_LIBRARIES openssl)
endif()