cmake_minimum_required(VERSION 3.4)

SET(SHARED_PROJECT_NAME "m2-shared" CACHE STRING "")
message(STATUS "SHARED PROJECT NAME : ${SHARED_PROJECT_NAME}")

project(${SHARED_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

# ------------------------  3rd-parties  ------------------------
get_filename_component(M2_ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)
set(M2_GLOBAL_3RD_PARTY "${M2_ROOT}/../3rd-party")
set(M2_3RD_PARTY "${M2_ROOT}/3rd-party")

#----------- precompiled -----------
list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cotire-1.7.10/CMake")
list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cmake")

message("${CMAKE_MODULE_PATH}")

# -------------------------   sources   -------------------------
set(SOURCES
    src/http_client.cpp
    src/http_connection.cpp

    src/base64.cpp

    src/crypto_common.cpp
    src/crypto_hash.cpp
    src/crypto_pki.cpp
    src/crypto_sym.cpp
    src/safe_log.cpp
)

set(HEADERS
        include/http_client.h
        include/http_connection.h
        include/perform_result.h

        include/base64.h

        include/crypto_common.h
        include/crypto_hash.h
        include/crypto_pki.h
        include/crypto_sym.h
        )

include_directories(include)

if(MSVC)
    set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})
endif()


# ---------------- enforcing static linkage ----------
if(WIN32)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    set(USE_STATIC_LIBS)
    add_definitions(-DOPENSSL_STATIC=1)
    add_definitions(-DLIBCRYPTO_STATIC=1)
    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
    endif(MINGW)
    set(OPENSSL_USE_STATIC_LIBS TRUE)

endif()


#-------------------------    STANDALONE DEPS    -----------------------

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

find_package(Libevent REQUIRED)
include_directories(${LIBEVENT_INCLUDE_DIRS})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_library(${SHARED_PROJECT_NAME} ${HEADERS} ${SOURCES})

add_executable(${SHARED_PROJECT_NAME}_example examples/crypto_example.cpp)
target_link_libraries( ${SHARED_PROJECT_NAME} ${OPENSSL_LIBRARIES})
target_link_libraries( ${SHARED_PROJECT_NAME}_example ${PROJECT_NAME} ${OPENSSL_LIBRARIES})


if (WIN32)
    target_link_libraries(${SHARED_PROJECT_NAME} ws2_32)
endif(WIN32)
