cmake_minimum_required(VERSION 3.20)
project(cpp_foundation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp_foundation_demo
    src/main.cpp
    src/vector_math.cpp
    src/raii_demo.cpp
)

target_include_directories(cpp_foundation_demo PRIVATE include)

if(MSVC)
    target_compile_options(cpp_foundation_demo PRIVATE /W4 /utf-8)
else()
    target_compile_options(cpp_foundation_demo PRIVATE -Wall -Wextra)
endif()
