add_library(graphics_common STATIC
    src/gl_loader.cpp
    src/shader_util.cpp
    src/gfx_window.cpp
)

target_include_directories(graphics_common PUBLIC include)

include(../cmake/GraphicsDeps.cmake)
setup_glfw(graphics_common)

if(MSVC)
    target_compile_options(graphics_common PRIVATE /W4 /utf-8)
endif()
