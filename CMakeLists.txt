cmake_minimum_required(VERSION 3.28)
project(sesh C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT TARGET vexspoke)
    include(FetchContent)
    FetchContent_Declare(
        vexspoke
        GIT_REPOSITORY https://github.com/vexgraph-dev/vexspoke.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(vexspoke)
endif()

file(GLOB_RECURSE SESH_SOURCES
    "src/*.c"
)

if(SESH_SOURCES)
    add_library(sesh STATIC ${SESH_SOURCES})
    target_include_directories(sesh PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(sesh PUBLIC vexspoke)
else()
    add_custom_target(sesh)
endif()
