cmake_minimum_required(VERSION 3.4)
project(${SHARED_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

# -------------------------   sources   -------------------------
set(SOURCES
        src/http_client.cpp
        src/http_connection.cpp

        src/crypto_common.cpp
        src/crypto_hash.cpp
        src/crypto_pki.cpp
        src/crypto_sym.cpp
        )

set(HEADERS
        include/http_client.h
        include/http_connection.h
        include/perform_result.h

        include/crypto_common.h
        include/crypto_hash.h
        include/crypto_pki.h
        include/crypto_sym.h
        )

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

#-------------------------    open SSL    -----------------------

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})


add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

add_executable(libm2crypto_example examples/crypto_example.cpp)
target_link_libraries( ${PROJECT_NAME} ${OPENSSL_LIBRARIES})
target_link_libraries( libm2crypto_example ${PROJECT_NAME} ${OPENSSL_LIBRARIES})


if (WIN32)
    target_link_libraries(libm2crypto ws2_32)
endif(WIN32)

# ---------------- enforcing static linkage ----------
if(WIN32)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    set(USE_STATIC_LIBS)
    add_definitions(-DLIBSSH_STATIC=1)
    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
    endif(MINGW)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

