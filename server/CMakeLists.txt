cmake_minimum_required(VERSION 3.4)

project(m2-server)

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
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_DEBUG     ${M2_BIN_DIR})
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_RELEASE   ${M2_BIN_DIR})
set(CMAKE_EXECUTABLE_OUTPUT_PATH                ${M2_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG       ${M2_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE     ${M2_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_PATH                  ${M2_LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG        ${M2_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE      ${M2_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY              ${M2_BIN_DIR})


#----------------------- compile options ------------------------
add_compile_options(-std=c++14)


# ------------------------  3rd-parties  ------------------------
set(M2_GLOBAL_3RD_PARTY "${M2_ROOT}/../3rd-party")
set(M2_3RD_PARTY        "${M2_ROOT}/3rd-party")


#----------- precompiled -----------
list(APPEND CMAKE_MODULE_PATH "${M2_GLOBAL_3RD_PARTY}/cotire-1.7.10/CMake")
include(cotire)


# -------------------------    boost   --------------------------
find_package(Boost COMPONENTS
    system
    filesystem

    REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

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
    stdafx.h

# data
        src/Data/stdafx.h

        src/Data/User/User.h
        src/Data/User/Users.h
        src/Data/Dialog/Dialog.h
        src/Data/Dialog/Dialogs.h
        src/Data/Dialog/UserDialogs.h
        src/Data/Message/Message.h
        src/Data/Index/IndexManager.h
        src/Data/Index/CashManager.hpp
        src/Data/Index/FileHandler.h

        src/Data/Data.hpp
)

set(SOURCES
    src/stdafx.cpp
    src/main.cpp

# data
        src/Data/User/User.cpp
        src/Data/User/Users.cpp
        src/Data/Dialog/Dialog.cpp
        src/Data/Dialog/Dialogs.cpp
        src/Data/Dialog/UserDialogs.cpp
        src/Data/Message/Message.cpp
        src/Data/Index/IndexManager.cpp
        src/Data/Index/FileHandler.cpp
        src/Data/stdafx.cpp)

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

include_directories(${SUBPROJECT_ROOT})

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES})
