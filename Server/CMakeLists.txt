cmake_minimum_required(VERSION 3.13)

project(Server)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#target_link_libraries(Async handler.h linkedlist.h sockWrapper.h asyncServer.h)

set(OPENSSL_SSL_LIBRARY "/usr/lib/libssl.so")
set(OPENSSL_CRYPTO_LIBRARY "/usr/lib/libcrypto.so")
set(PENSSL_MSVC_STATIC_RT TRUE)

find_package(OpenSSL REQUIRED)

#add_executable(CryptSock cServer.cpp)

find_package(Threads)

add_executable(Server Server.cpp)

target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT}  ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
