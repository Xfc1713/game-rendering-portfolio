cmake_minimum_required(VERSION 3.20)
project(MiniForwardRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../graphics_common ${CMAKE_BINARY_DIR}/graphics_common)

add_executable(MiniForwardRenderer src/main.cpp)
target_link_libraries(MiniForwardRenderer PRIVATE graphics_common)

if(MSVC)
    target_compile_options(MiniForwardRenderer PRIVATE /W4 /utf-8)
endif()
