cmake_minimum_required(VERSION 3.20)
project(opengl_tutorials LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../graphics_common ${CMAKE_BINARY_DIR}/graphics_common)

set(TUTORIAL_DEMOS
    01_triangle
    02_shader_color
    03_textured_quad
    04_mvp_cube
    05_lighting
)

foreach(demo ${TUTORIAL_DEMOS})
    add_executable(${demo} src/${demo}.cpp)
    target_link_libraries(${demo} PRIVATE graphics_common)
    if(MSVC)
        target_compile_options(${demo} PRIVATE /W4 /utf-8)
    endif()
endforeach()
