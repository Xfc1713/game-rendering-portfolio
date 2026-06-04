#include "gfx_window.h"

#include "gl_loader.h"

#include <GLFW/glfw3.h>

#include <iostream>

GLFWwindow* createRenderWindow(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "glfwInit failed\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "glfwCreateWindow failed\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!loadOpenGL33Core()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    glViewport(0, 0, width, height);
    return window;
}

void pollWindow(GLFWwindow* window) {
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

bool windowShouldClose(GLFWwindow* window) {
    return glfwWindowShouldClose(window) != 0;
}

void destroyWindow(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}
