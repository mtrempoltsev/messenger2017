cmake_minimum_required(VERSION 3.4)

set(CORE_PROJECT_NAME m2-core CACHE STRING "m2-core project name")
project(${CORE_PROJECT_NAME})

set(M2_ROOT ${CMAKE_SOURCE_DIR}/../ CACHE STRING "m2-client root dir")
get_filename_component(SUBPROJECT_ROOT "${M2_ROOT}/core" ABSOLUTE)
get_filename_component(SHARED_ROOT "${M2_ROOT}/../shared" ABSOLUTE)
get_filename_component(CLI_SHARED_ROOT "${M2_ROOT}/shared" ABSOLUTE)
option(ENABLE_TESTS "Enable m2-core tests" OFF)


# -------------------------   sources   -------------------------
set(HEADERS
    stdafx.h

    include/config.h
    include/contact_manager.h
    include/core.h
    include/core_dispatcher.h
    include/error.h
    include/guid.h
    include/message_manager.h
    include/login_manager.h
    include/path_settings.h
    include/jobtype.h

    ../shared_client/chat.h
    ../shared_client/handlers.h
    ../shared_client/message.h
    ../shared_client/contact.h

    ../../shared/include/safe_log.h
    ../../shared/include/http_client.h
    ../../shared/include/http_connection.h
    ../../shared/include/crypto_common.h
    ../../shared/include/crypto_hash.h
    ../../shared/include/crypto_pki.h
    ../../shared/include/crypto_sym.h
    ../../shared/include/base64.h
    )

set(SOURCES
    src/stdafx.cpp

    src/config.cpp
    src/contact_manager.cpp
    src/core.cpp
    src/core_dispatcher.cpp

    src/guid.cpp
    src/error.cpp
    src/login_manager.cpp
    src/message_manager.cpp
    src/guid.cpp
    src/config.cpp
    src/login_manager.cpp
    src/path_settings.cpp

    ../../shared/src/safe_log.cpp
    ../../shared/src/http_client.cpp
    ../../shared/src/http_connection.cpp
    ../../shared/src/crypto_common.cpp
    ../../shared/src/crypto_hash.cpp
    ../../shared/src/crypto_pki.cpp
    ../../shared/src/crypto_sym.cpp
    ../../shared/src/base64.cpp
    )

if (ENABLE_TESTS)
    add_subdirectory (tests)
endif()

if(MSVC)
    set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})
endif()



# ----------------------- Dependencies ----------------------

include_directories(${SUBPROJECT_ROOT})
include_directories(${SUBPROJECT_ROOT}/include)
include_directories(${SUBPROJECT_ROOT}/../shared_client)

include_directories(${SHARED_ROOT}/include)

# ------------------------  3rd-parties  ------------------------

get_filename_component(M2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
get_filename_component(M2_GLOBAL_3RD_PARTY "${M2_ROOT}/../../3rd-party" ABSOLUTE)
get_filename_component(M2_3RD_PARTY "${M2_ROOT}/3rd-party" ABSOLUTE)

# ----------------------- LIBRARIES ----------------------------

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)


# --------------------------  PATHS ----------------------------

get_filename_component(M2_COTIRE "${M2_GLOBAL_3RD_PARTY}/cotire-1.7.10/CMake" ABSOLUTE)
set (CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY};${M2_GLOBAL_3RD_PARTY}/cmake;${M2_COTIRE}")

# -------------------------    DEPENDENCIES   --------------------------
# ----------------------------------------------------------
if (WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(BUILD_SHARED_LIBRARIES OFF)
    SET(CURL_STATICLIB ON)
    set(CMAKE_CXX_FLAGS -DCURL_STATICLIB)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -static-libs")
    find_library(SSL_LIB libssl.a REQUIRED)
    find_library(CRYPTO_LIB libcrypto.a REQUIRED)
    find_library(LIBEVENTS_LIB libevent.a REQUIRED)
    find_library(LIBEVENTS_CORE_LIB libevent_core.a REQUIRED)
    find_library(LIBEVENTS_EXTRA_LIB libevent_extra.a REQUIRED)
    find_library(CURL_LIB libcurl.a REQUIRED)
    message(STATUS "LIB: ${CRYPTO_LIB}")

endif()

# --------------------------------------------------------

find_package(Boost COMPONENTS REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
message(STATUS "CURL: ${CURL_INCLUDE_DIRS}")

find_package(Libevent REQUIRED)
include_directories(${LIBEVENT_INCLUDE_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(Threads)

#----------- precompiled -----------
include(cotire)
cotire(${PROJECT_NAME})


#---------------------------------------


message(STATUS "Libraries: ${Boost_LIBRARY_DIRS} ${CURL_LIBRARIES} ${LIBEVENT_LIBRARIES} ${OPENSSL_LIBRARIES}")
link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CURL_LIB}
        ${LIBEVENTS_LIB} ${LIBEVENTS_CORE_LIB} ${LIBEVENTS_EXTRA_LIB} ${SSL_LIB} ${CRYPTO_LIB} )

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 wldap32)

endif()

