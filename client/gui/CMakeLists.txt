cmake_minimum_required(VERSION 3.4)

project(${GUI_PROJECT_NAME})

set(SUBPROJECT_ROOT "${M2_ROOT}/gui")

find_package( Qt5Core REQUIRED )
find_package( Qt5Quick REQUIRED )
find_package( Qt5Gui REQUIRED )

set( CMAKE_AUTOMOC ON )

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(include/controlers)
include_directories(${Qt5Core_INCLUDE_DIRS})

# -------------------------   sources   -------------------------
set(HEADERS
    stdafx.h
    include/guiadapter.h
    include/controlers/registrationcontroler.h
    include/controlers/logincontroler.h
    include/chats_filter_proxy_model.h
    include/chats_model.h
    include/contacts_model.h
    include/messages_model.h
    include/models_elements.h
    )

set(SOURCES
    src/stdafx.cpp
    src/main.cpp
    src/guiadapter.cpp
    src/controlers/registrationcontroler.cpp
    src/controlers/logincontroler.cpp
    src/chats_filter_proxy_model.cpp
    src/chats_model.cpp
    src/contacts_model.cpp
    src/messages_model.cpp
    src/models_elements.cpp
    )

qt5_add_resources(RESOURCES application.qrc)

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

include_directories(${SUBPROJECT_ROOT})

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${CORE_PROJECT_NAME}
        Qt5::Core
        Qt5::Quick)
