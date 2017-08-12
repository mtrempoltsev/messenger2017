#GUI
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
    include/controlers/chatscontroler.h
    include/controlers/messagescontroler.h
    include/models/chats_filter_proxy_model.h
    include/models/chats_model.h
    include/models/contacts_model.h
    include/models/messages_model.h
    include/models/models_elements.h
    )

set(SOURCES
    src/stdafx.cpp
    src/main.cpp
    src/guiadapter.cpp
    src/controlers/registrationcontroler.cpp
    src/controlers/logincontroler.cpp
    src/controlers/chatscontroler.cpp
    src/controlers/messagescontroler.cpp
    src/models/chats_filter_proxy_model.cpp
    src/models/chats_model.cpp
    src/models/contacts_model.cpp
    src/models/messages_model.cpp
    src/models/models_elements.cpp
    )

qt5_add_resources(RESOURCES application.qrc)

set_source_group("sources" ${SUBPROJECT_ROOT} ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

include_directories(${SUBPROJECT_ROOT})
include_directories(${SUBPROJECT_ROOT}/include)
include_directories(${SUBPROJECT_ROOT}/../shared_client)
include_directories(${SUBPROJECT_ROOT}/../core/include)


set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT stdafx.h)
cotire(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${CORE_PROJECT_NAME}
        Qt5::Core
        Qt5::Quick)

      set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
      find_package( Threads REQUIRED)
      set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
