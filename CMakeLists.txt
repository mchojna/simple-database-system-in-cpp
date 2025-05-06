cmake_minimum_required(VERSION 3.29)
project(simple_database)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         11.0.2
)

FetchContent_MakeAvailable(fmt)

add_executable(simple_database main.cpp
        db/db.cpp
        db/db.hpp)
target_link_libraries(simple_database fmt)