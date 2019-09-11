cmake_minimum_required(VERSION 3.12)
project(CryptClientSock)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

set(OPENSSL_USE_STATIC_LIBS TRUE)

#reserved variables wroks in depends of platform
#set(OPENSSL_INCLUDE_DIR "/usr/include/openssl/")
#set(OPENSSL_ROOT_DIR "/usr/include/openssl/")
#set(OPENSSL_LIBRARIES "/usr/include/openssl")

set(OPENSSL_SSL_LIBRARY "/usr/lib/libssl.so")
set(OPENSSL_CRYPTO_LIBRARY "/usr/lib/libcrypto.so")
set(PENSSL_MSVC_STATIC_RT TRUE)

find_package(OpenSSL REQUIRED)

add_executable(CryptClientSock cClient.cpp)
target_link_libraries(CryptClientSock ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})


#also platform deoendent options
#link_directories("/usr/lib/libssl.so")
#add_library(Crypt SHARED ${SOURCE_FILES} ${HEADER_FILES})
#target_include_directories(Crypt PUBLIC ${OPENSSL_INCLUDE_LIBRARY})
#add_library(CryptSock ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})


#Options to manual configuration compilation settings
#1. cmake -DOPENSSL_ROOT_DIR=/usr/local/ssl -DOPENSSL_LIBRARIES=/usr/local/ssl/lib
#2. g++ cServer -o cServer -lssl -lcrypto  linking via pure  g++
