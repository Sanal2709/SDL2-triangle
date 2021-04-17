cmake_minimum_required(VERSION 3.1)
project(triangle)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE INCLUDES "*.h" "src/*.h")
file(GLOB_RECURSE SOURCES  "main.cpp" "src/*.cpp")

find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wpedantic -Wall -Wextra)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)