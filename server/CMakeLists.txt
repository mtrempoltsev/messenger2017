cmake_minimum_required(VERSION 3.4)

project(m2-server)

set(SHARED_PROJECT_NAME m2-shared)
get_filename_component(M2_ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(M2_DEBUG ON)
    set(M2_BIN_DIR "${M2_ROOT}/bin/Debug")
    set(M2_LIB_DIR "${M2_ROOT}/bin/Debug")
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(M2_RELEASE ON)
    set(M2_BIN_DIR "${M2_ROOT}/bin/Release")
    set(M2_LIB_DIR "${M2_ROOT}/bin/Release")
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
else()
    message(FATAL_ERROR "Invalid CMAKE_BUILD_TYPE value (Debug|Release)")
endif()

message(STATUS "binaries output: " ${M2_BIN_DIR})
message(STATUS "libraries output: " ${M2_LIB_DIR})

# ---------------------------  paths  ---------------------------

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_DEBUG ${M2_BIN_DIR})
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_RELEASE ${M2_BIN_DIR})
set(CMAKE_EXECUTABLE_OUTPUT_PATH ${M2_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${M2_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${M2_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_PATH ${M2_LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${M2_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${M2_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${M2_BIN_DIR})

#----------------------- compile options ------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -fpermissive -Wcast-align -pedantic -fstack-protector-all -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align -Wno-format -Wno-long-long -Wno-unused-function -Wno-unused-parameter")


# ------------------------  3rd-parties  ------------------------
set(M2_GLOBAL_3RD_PARTY "${M2_ROOT}/../3rd-party")
set(M2_3RD_PARTY "${M2_ROOT}/3rd-party")

# ----------------------- source groups  ------------------------
macro(set_source_group group_name root_dir ${ARGN})
    get_filename_component(_path ${root_dir} ABSOLUTE)
    foreach(_source IN ITEMS ${ARGN})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${_path}" "" _source_trimmed "${_source_path}")
        set(_source_group "${group_name}${_source_trimmed}")
        string(REPLACE "/" "\\" _source_group "${_source_group}")
        source_group("${_source_group}" FILES "${_source}")
    endforeach()
endmacro()

# -------------------------   sources   -------------------------

set(HEADERS
    stdafx.h include/Data/stdafx.cpp)

file(GLOB_RECURSE HEADERS include/*.h
                          include/*.hpp)
file(GLOB_RECURSE SOURCES src/*.cpp)

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${DATA})

include_directories(..)
include_directories(include/)

#-------------------------    open SSL    -----------------------

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})


# -------------------------    boost   --------------------------
find_package(Boost COMPONENTS
    system
    filesystem
    random
    REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES})


#----------- precompiled -----------
list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cotire-1.7.10/CMake")
list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cmake")
include(cotire)


# -------------------------    boost   --------------------------
find_package(Boost COMPONENTS
        REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(Threads)

# --------------------------  shared  ---------------------------
set(SHARED_ROOT "${M2_ROOT}/../shared")
include_directories("${SHARED_ROOT}/include")
add_subdirectory(${SHARED_ROOT} shared)

target_link_libraries (${PROJECT_NAME} ${OPENSSL_LIBRARIES} m2-shared)
add_dependencies(${PROJECT_NAME} m2-shared)
