cmake_minimum_required(VERSION 3.4)

project(${CORE_PROJECT_NAME})

set(SUBPROJECT_ROOT "${M2_ROOT}/core")

option(ENABLE_TESTS "Enable m2-core tests" OFF)


# -------------------------   sources   -------------------------
set(HEADERS
    stdafx.h

    include/config.h
    include/contact.h
    include/core.h
    include/core_dispatcher.h
    include/error.h
    include/guid.h
    include/message.h
    include/login_manager.h
    include/path_settings.h
    include/jobtype.h
    ../shared/handlers.h
    )

set(SOURCES
    src/stdafx.cpp

    src/config.cpp
    src/contact.cpp
    src/core.cpp
    src/core_dispatcher.cpp

    src/guid.cpp
    src/message.cpp
    src/error.cpp

    src/message.cpp
    src/guid.cpp
    src/config.cpp
    src/path_settings.cpp
    )

if (ENABLE_TESTS)
add_subdirectory (tests)
endif()

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

include_directories(${SUBPROJECT_ROOT})
include_directories(${SUBPROJECT_ROOT}/include)
include_directories(${SUBPROJECT_ROOT}/../shared)

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES} -lboost_filesystem -lboost_system)
