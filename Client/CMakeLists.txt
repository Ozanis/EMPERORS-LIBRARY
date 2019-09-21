cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(OPENSSL_SSL_LIBRARY "/usr/lib/libssl.so")
set(OPENSSL_CRYPTO_LIBRARY "/usr/lib/libcrypto.so")
set(PENSSL_MSVC_STATIC_RT TRUE)

find_package(OpenSSL REQUIRED)

#add_executable(CryptSock cServer.cpp)
add_executable(Client Client.cpp)
target_link_libraries(Client ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})



