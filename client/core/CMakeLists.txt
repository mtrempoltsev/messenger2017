cmake_minimum_required(VERSION 3.4)

project(${CORE_PROJECT_NAME})

set(SUBPROJECT_ROOT "${M2_ROOT}/core")


# -------------------------   sources   -------------------------
set(HEADERS
    stdafx.h

    include/core_dispatcher.h
    include/error.h
    include/guid.h
    include/config.h
    )

set(SOURCES
    src/stdafx.cpp

    src/core_dispatcher.cpp
    src/error.cpp
    src/guid.cpp
    src/config.cpp
    )

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

include_directories(${SUBPROJECT_ROOT})

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES})
