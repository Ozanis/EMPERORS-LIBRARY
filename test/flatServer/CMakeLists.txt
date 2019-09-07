cmake_minimum_required(VERSION 3.13)

project(Flat)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#target_link_libraries(Flat handler.h linkedlist.h sockWrapper.h asyncServer.h)

add_executable(Flat flatServer.cpp)

