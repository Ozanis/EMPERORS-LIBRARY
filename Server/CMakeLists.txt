cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.65.1 COMPONENTS iostreams)
add_executable(Server Server.cpp)
target_link_libraries(Server ${Boost_LIBRARIES})