cmake_minimum_required(VERSION 3.4)

project(${SHARED_PROJECT_NAME})


# -------------------------   sources   -------------------------
set(HEADERS
    include/http_client.h
    include/http_connection.h
    include/perform_result.h
    )

set(SOURCES
    src/http_client.cpp
    src/http_connection.cpp
    )

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(Libevent REQUIRED)
include_directories(${LIBEVENT_INCLUDE_DIRS})

find_package(Threads)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
    ${CURL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})
