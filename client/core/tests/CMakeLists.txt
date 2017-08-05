cmake_minimum_required(VERSION 3.4)

project(test-app)

add_compile_options(-std=c++11 -ggdb -O0)

# -------------------------   sources   -------------------------
set(SOURCES
    test.cpp
    )

include_directories(${SUBPROJECT_ROOT}/include)

# -------------------------    boost   --------------------------
find_package(Boost COMPONENTS
    REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable (${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES} m2-core)
