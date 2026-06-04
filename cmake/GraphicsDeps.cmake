# GLFW：优先 MSYS2 已安装的包（避免 GitHub FetchContent 在国内失败）
include(FetchContent)

option(USE_SYSTEM_GLFW "使用系统/MSYS2 的 glfw3，不拉取 GitHub" OFF)

function(setup_glfw target_name)
    set(_glfw_linked FALSE)

    if(USE_SYSTEM_GLFW OR MINGW)
        find_package(glfw3 CONFIG QUIET)
        if(glfw3_FOUND)
            target_link_libraries(${target_name} PRIVATE glfw)
            set(_glfw_linked TRUE)
        endif()
    endif()

    if(NOT _glfw_linked)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.4
        )
        FetchContent_MakeAvailable(glfw)
        target_link_libraries(${target_name} PRIVATE glfw)
    endif()

    if(WIN32)
        target_link_libraries(${target_name} PRIVATE opengl32)
    else()
        find_package(OpenGL REQUIRED)
        target_link_libraries(${target_name} PRIVATE OpenGL::GL)
    endif()
endfunction()
