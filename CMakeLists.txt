cmake_minimum_required(VERSION 3.13)
project(Telemetry)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(cryptClient)            # 1
add_subdirectory(asyncServer)            # 2

