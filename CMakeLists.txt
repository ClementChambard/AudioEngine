cmake_minimum_required(VERSION 3.10)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Werror)

project(AudioEngine VERSION 1.0)

file(GLOB_RECURSE SRCS    src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)

add_executable("${PROJECT_NAME}" ${SRCS} ${HEADERS})

target_link_libraries("${PROJECT_NAME}" PUBLIC -lSDL2)
