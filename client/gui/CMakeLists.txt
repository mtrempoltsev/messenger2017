cmake_minimum_required(VERSION 3.4)

project(${GUI_PROJECT_NAME})

set(SUBPROJECT_ROOT "${M2_ROOT}/gui")


# -------------------------   sources   -------------------------
set(HEADERS
    stdafx.h

    ${SHARED_HEADERS}
    )

set(SOURCES
    src/stdafx.cpp

    src/main.cpp

    ${SHARED_SOURCES}
    )

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

include_directories(${SUBPROJECT_ROOT})

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${CORE_PROJECT_NAME})
