cmake_minimum_required(VERSION 3.14)

project(Differential_Drive_Bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download Matplot++ as a project dependency
include(FetchContent)

set(MATPLOTPP_BUILD_WITH_PEDANTIC_WARNINGS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
    GIT_TAG v1.2.1
)

FetchContent_MakeAvailable(matplotplusplus)

# Project executables
add_executable(Differential_Drive_Bot
    main.cpp
    robot.cpp
)

# Link Matplot++ to executable
target_link_libraries(Differential_Drive_Bot
    PRIVATE matplot
)