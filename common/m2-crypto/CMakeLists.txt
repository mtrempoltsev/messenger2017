cmake_minimum_required(VERSION 3.2)
project(m2crypto)
set(CMAKE_CXX_STANDARD 14)
set(OPENSSL_USE_STATIC_LIBS TRUE)
set(USE_STATIC_LIBS)
add_definitions(-DLIBSSH_STATIC=1)
add_definitions(-DOPENSSL_STATIC=1)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")


find_package(openssl REQUIRED)
if( OpenSSL_FOUND )
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif(OpenSSL_FOUND)

find_package(Boost REQUIRED)
if ( Boost_FOUND )
    message(STATUS "Using Boost ${Boost_VERSION}")
endif(Boost_FOUND)
#find_package(libssh REQUIRED)
#if(LIBSSH_FOUND)
#    message(STATUS "Using LibSSH ${LIBSSH_VERSION}")
#else()
#    message(STATUS "LIBSSH not found")
#endif(LIBSSH_FOUND)

include_directories(
        ${OPENSSL_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
#        ${LIBSSH_INCLUDE_DIR}
        "./include/"
)

link_directories(${m2crypto_BINARY_DIR} ${OPENSSL_ROOT_DIR})

file(GLOB_RECURSE M2_CRYPTO_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/src/m2/crypto/*.c*"
        )
file(GLOB_RECURSE M2_CRYPTO_H
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        )
set(SOURCE_FILES example.cc)

add_library(libm2crypto STATIC ${M2_CRYPTO_CPP} ${M2_CRYPTO_H})
add_executable(libm2crypto_example ${SOURCE_FILES})
add_dependencies(libm2crypto_example libm2crypto)
#find_library(SSH_LIBRARY libssh.a REQUIRED)


target_link_libraries(libm2crypto ${SSH_LIBRARY} ${OPENSSL_LIBRARIES})
if (WIN32)
    target_link_libraries(libm2crypto ws2_32)
endif(WIN32)

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif(MINGW)

target_link_libraries(libm2crypto_example libm2crypto)