cmake_minimum_required(VERSION 3.4)

set(CORE_PROJECT_NAME m2-core CACHE STRING "m2-core project name")
project(${CORE_PROJECT_NAME})

set(M2_ROOT ${CMAKE_SOURCE_DIR}/../ CACHE STRING "m2-client root dir")
set(SUBPROJECT_ROOT "${M2_ROOT}/core")

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


# ------------------------  3rd-parties  ------------------------

get_filename_component(M2_ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)
set(M2_GLOBAL_3RD_PARTY "${M2_ROOT}/../../3rd-party")
set(M2_3RD_PARTY "${M2_ROOT}/3rd-party")

# ----------------------- LIBRARIES ----------------------------

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)

#----------- precompiled -----------
#list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cotire-1.7.10/CMake")
#list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cmake")
get_filename_component(M2_GLOBAL_3RD_PARTY "${M2_GLOBAL_3RD_PARTY}/cotire-1.7.10/CMake" ABSOLUTE)
set (CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}")
include(cotire)
message(${CMAKE_MODULE_PATH})
cotire(${PROJECT_NAME})


target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES} -lboost_system -lboost_filesystem)
target_link_libraries(${PROJECT_NAME}
    -lcurl -levent -lcrypto)
