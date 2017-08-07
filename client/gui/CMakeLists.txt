cmake_minimum_required(VERSION 3.4)

project(${GUI_PROJECT_NAME})

set(SUBPROJECT_ROOT "${M2_ROOT}/gui")

find_package( Qt5Core REQUIRED )
find_package( Qt5Quick REQUIRED )
find_package( Qt5Gui REQUIRED )

set( CMAKE_AUTOMOC ON )

INCLUDE_DIRECTORIES(include)
include_directories(${Qt5Core_INCLUDE_DIRS})

# -------------------------   sources   -------------------------
set(HEADERS
    stdafx.h
    include/guiadapter.h
    include/registrationcontroler.h
    include/QmlCppInterface.h
    ../shared/handlers.h
    )

set(SOURCES
    src/stdafx.cpp
    src/main.cpp
    src/guiadapter.cpp
    src/registrationcontroler.cpp
    )

qt5_add_resources(RESOURCES application.qrc)

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

include_directories(${SUBPROJECT_ROOT})
include_directories(${SUBPROJECT_ROOT}/include)
include_directories(${SUBPROJECT_ROOT}/../core/include)
include_directories(${SUBPROJECT_ROOT}/../shared)

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package( Threads REQUIRED)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")

target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${CORE_PROJECT_NAME}
        Qt5::Core
        Qt5::Quick)
