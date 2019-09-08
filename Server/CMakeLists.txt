cmake_minimum_required(VERSION 3.13)

project(Server)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#target_link_libraries(Async handler.h linkedlist.h sockWrapper.h asyncServer.h)

find_package(Threads)

add_executable(Server Server.cpp)

target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT} )
