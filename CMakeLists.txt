cmake_minimum_required(VERSION 3.13)
project(Telemetry)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(test/flatServer)
add_subdirectory(test/simpleClient)
add_subdirectory(test/cryptSockServer)
add_subdirectory(test/cryptSockClient)



