cmake_minimum_required(VERSION 3.12)
project(Telematry)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Client)            # 1
add_subdirectory(Server)            # 2

add_subdirectory(cryptClient)       # 1
add_subdirectory(asyncServer)       # 2
