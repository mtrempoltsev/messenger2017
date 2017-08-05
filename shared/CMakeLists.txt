cmake_minimum_required(VERSION 3.4)
project(${SHARED_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)


find_package(openssl REQUIRED)
if( OpenSSL_FOUND )
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif(OpenSSL_FOUND)

find_package(Boost REQUIRED)
if ( Boost_FOUND )
    message(STATUS "Using Boost ${Boost_VERSION}")
endif(Boost_FOUND)

find_package(CURL REQUIRED)



# -------------------------   sources   -------------------------

include_directories(
        ${OPENSSL_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
        ${CURL_INCLUDE_DIRS}
)

link_directories(${m2crypto_BINARY_DIR} ${OPENSSL_ROOT_DIR})

file(GLOB_RECURSE M2_CRYPTO_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/src/m2/crypto/*.c*"
)
file(GLOB_RECURSE M2_CRYPTO_H
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)
set(SOURCES
    src/http_client.cpp
    src/http_connection.cpp
    ${M2_CRYPTO_CPP}
)

set(HEADERS
    include/http_client.h
    include/http_connection.h
    include/perform_result.h
    ${M2_CRYPTO_H}
)

set(SOURCES_FILES ./examples/crypto_example.cc)

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})




# ---------------- enforcing static linkage ----------

set(OPENSSL_USE_STATIC_LIBS TRUE)
set(USE_STATIC_LIBS)
add_definitions(-DLIBSSH_STATIC=1)
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif(MINGW)
set(OPENSSL_USE_STATIC_LIBS TRUE)


